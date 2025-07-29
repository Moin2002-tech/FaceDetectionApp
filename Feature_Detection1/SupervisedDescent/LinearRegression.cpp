#include "LinearRegression.hpp"
#include"LandmarkBasedSupervisedDecentTraining.hpp"
#include"../logging/LoggerFactory.hpp"
#include"../logging/Logger.hpp"
#include<armadillo>

#include<fstream>
#include<random>

//#include<Eigen/src/Core/Matrix.h>

namespace superVisedDescent
{
	cv::Mat LinearRegression::alignMean(cv::Mat& mean, cv::Rect& faceBox, float scalingX, float scalingY, float translationX, float translationY)
	{
		// Initial estimate x_0: Center the mean face at the [-0.5, 0.5] x [-0.5, 0.5] square (assuming the face-box is that square)
		// More precise: Take the mean as it is (assume it is in a space [-0.5, 0.5] x [-0.5, 0.5]), and just place it in the face-box as
		// if the box is [-0.5, 0.5] x [-0.5, 0.5]. (i.e. the mean coordinates get upscaled)
		Mat alignedMean = mean.clone();
		Mat alignedMeanX = alignedMean.colRange(0, alignedMean.cols / 2);
		Mat alignedMeanY = alignedMean.colRange(alignedMean.cols / 2, alignedMean.cols);
		alignedMeanX = (alignedMeanX * scalingX + 0.5f + translationX) * faceBox.width + faceBox.x;
		alignedMeanY = (alignedMeanY * scalingY + 0.5f + translationY) * faceBox.height + faceBox.y;
		return alignedMean;
	}
	float LinearRegression::calculateMeanTranslation(cv::Mat& groundTruth, cv::Mat& estimate)
	{
		cv::Scalar gtMean = cv::mean(groundTruth);
		cv::Scalar estMean = cv::mean(estimate);

		return (estMean[0] - gtMean[0]);
	}

	float LinearRegression::calculateScaleRatio(cv::Mat& groundTruth, cv::Mat& estimate)
	{
		double gtMinimum, gtMaximum;
		cv::minMaxIdx(groundTruth, &gtMinimum, &gtMaximum);
		double x0Minimum, x0Maximum;
		cv::minMaxIdx(estimate, &x0Minimum, &x0Maximum);

		return (x0Maximum - x0Minimum) / (gtMaximum - gtMinimum);
	}

	void LinearRegression::saveShapeInstanceToMatlab(cv::Mat& shapeInstance, std::string& filename)
	{
		int numLandmarks;
		if (shapeInstance.rows > 1) 
		{
			numLandmarks = shapeInstance.rows / 2;
		}
		else 
		{
			numLandmarks = shapeInstance.cols / 2;
		}
		std::ofstream myfile;
		myfile.open(filename);
		myfile << "x = [";
		for (int i = 0; i < numLandmarks; ++i) 
		{
			myfile << shapeInstance.at<float>(i) << ", ";
		}
		myfile << "];" << std::endl << "y = [";
		for (int i = 0; i < numLandmarks; ++i)
		{
			myfile << shapeInstance.at<float>(i + numLandmarks) << ", ";
		}
		myfile << "];" << std::endl;
		myfile.close();
	}

	void LinearRegression::DrawLandmark(cv::Mat& image, cv::Mat& landmarks, cv::Scalar color)
	{
		auto numLandmarks = std::max(landmarks.cols, landmarks.rows) / 2;
		for (int i = 0; i < numLandmarks; ++i) 
		{
			cv::circle(image, cv::Point2f(landmarks.at<float>(i), landmarks.at<float>(i + numLandmarks)), 2, color);
		}
	}

	cv::Mat LinearRegression::getPerturbedShape(cv::Mat modelMean, AlignmentStatistics alignmentStatistics, cv::Rect detectedFace)
	{
		std::mt19937 engine;
		std::random_device rd;
		engine.seed(rd());

		std::normal_distribution<float> rndN_T_X(alignmentStatistics.tx.mu, alignmentStatistics.tx.sigma);
		std::normal_distribution<float> rndN_T_Y(alignmentStatistics.ty.mu, alignmentStatistics.ty.sigma);

		GaussParameter scaleVariance;
		scaleVariance.mu = (alignmentStatistics.sx.mu + alignmentStatistics.sy.mu);
		scaleVariance.sigma = (alignmentStatistics.sy.sigma + alignmentStatistics.sy.sigma);
		std::normal_distribution<float> rndN_scale(scaleVariance.mu, scaleVariance.sigma);

		double rnd_scale = rndN_scale(engine);
		Mat sampleShape = alignMean(modelMean, detectedFace, rnd_scale, rnd_scale, rndN_T_X(engine), rndN_T_Y(engine));

		return sampleShape;
	}

	cv::Mat LinearRegression::linearRegression(cv::Mat& A, cv::Mat& b, RegularizationType regularizationType, float lambda, bool regularizeAffineComponent)
	{
		logging::Logger logger = logging::LoggerFactory::Instance()->getLogger("superviseddescent");
		std::chrono::time_point<std::chrono::system_clock> start, end;
		int elapsed_mseconds;

		Mat AtA = A.t() * A;

		switch (regularizationType)
		{
		case RegularizationType::Manual:
			break;
		case RegularizationType::Automatic:
			lambda = lambda * cv::norm(AtA) / A.rows;
			break;
		case RegularizationType::EigenvalueThreshold:
		{
			std::chrono::time_point<std::chrono::system_clock> eigenTimeStart = std::chrono::system_clock::now();
			lambda = calculateEigenvalueThreshold(AtA);
			std::chrono::time_point<std::chrono::system_clock> eigenTimeEnd = std::chrono::system_clock::now();
			elapsed_mseconds = std::chrono::duration_cast<std::chrono::milliseconds>(eigenTimeEnd - eigenTimeStart).count();
			logger.debug("Automatic calculation of lambda for AtA took " + lexical_cast<string>(elapsed_mseconds) + "ms.");
		}
		break;
		default:
			break;
		}
		logger.debug("Setting lambda to: " + lexical_cast<string>(lambda));

		Mat regulariser = Mat::eye(AtA.rows, AtA.rows, CV_32FC1) * lambda;
		if (!regularizeAffineComponent) 
		{
			regulariser.at<float>(regulariser.rows - 1, regulariser.cols - 1) = 0.0f;
		}

		Mat AtAReg = AtA + regulariser;

		if (!AtAReg.isContinuous())
		{
			std::string msg("Matrix is not continuous. This should not happen as we allocate it directly.");
			logger.error(msg);
			throw std::runtime_error(msg);
		}

		// ARMADILLO PART (instead of Eigen)
		// Wrap AtAReg as Armadillo matrix
		arma::fmat AtAReg_arma(reinterpret_cast<float*>(AtAReg.data), AtAReg.rows, AtAReg.cols, false, true);

		std::chrono::time_point<std::chrono::system_clock> inverseTimeStart = std::chrono::system_clock::now();

		// Convert to double for stable rank/inverse checks
		arma::mat AtAReg_d = arma::conv_to<arma::mat>::from(AtAReg_arma);

		// LU Decomposition
		arma::mat L, U, P;
		bool success = arma::lu(L, U, P, AtAReg_d);
		arma::uword rankOfAtAReg = arma::rank(AtAReg_d);
		logger.trace("Rank of the regularized AtA: " + lexical_cast<string>(rankOfAtAReg));

		if (arma::det(AtAReg_d) != 0.0) {
			logger.debug("The regularized AtA is invertible.");
		}
		else {
			string msg("The regularized AtA is not invertible (its rank is " + lexical_cast<string>(rankOfAtAReg) + ", full rank would be " + lexical_cast<string>(AtAReg.rows) + "). Increase lambda (or use the pseudo-inverse, which is not implemented yet).");
			logger.error(msg);
#ifndef _DEBUG
			//throw std::runtime_error(msg);
#endif
		}

		arma::mat AtARegInv_d = arma::inv(AtAReg_d);
		cv::Mat AtARegInvFullLU(AtARegInv_d.n_rows, AtARegInv_d.n_cols, CV_32FC1);
		// Copy back to OpenCV format (convert double to float)
		for (int i = 0; i < AtARegInv_d.n_rows; ++i) 
		{
			for (int j = 0; j < AtARegInv_d.n_cols; ++j) 
			{
				AtARegInvFullLU.at<float>(i, j) = static_cast<float>(AtARegInv_d(i, j));
			}
		}

		std::chrono::time_point<std::chrono::system_clock> inverseTimeEnd = std::chrono::system_clock::now();
		elapsed_mseconds = std::chrono::duration_cast<std::chrono::milliseconds>(inverseTimeEnd - inverseTimeStart).count();
		logger.debug("Inverting the regularized AtA took " + lexical_cast<string>(elapsed_mseconds) + "ms.");

		// Final solve: AtARegInv * A^T * b
		Mat AtARegInvAt = AtARegInvFullLU * A.t();
		Mat AtARegInvAtb = AtARegInvAt * b;
		return AtARegInvAtb;
	}

	float LinearRegression::calculateEigenvalueThreshold(cv::Mat& matrix)
	{
		logging::Logger logger = logging::LoggerFactory::Instance()->getLogger("supervisedDecent");

		if (!matrix.isContinuous())
		{
			std::string message("matrix is Not continuous. Please make sure it continous. ");
			logger.error(message);
			throw std::runtime_error(message);
		}
		
		// Wrap OpenCV matrix in Armadillo matrix without copying
		arma::fmat AtA_arma(reinterpret_cast<float*>(matrix.data), matrix.rows, matrix.cols, false, true);

		// Calculate the eigenvalues of AtA. This is only for output purposes and not needed. Would make sense to remove this as it might be time-consuming.
		arma::vec eigval;
		arma::eig_sym(eigval, arma::conv_to<arma::mat>::from(AtA_arma)); // ✅ works
		// Convert to double precision for stability

        // Update the eigenvalue calculation to ensure proper conversion to double precision
     
        arma::mat AtA_d(reinterpret_cast<double*>(matrix.data), matrix.rows, matrix.cols, false, true); // Convert to double precision
        arma::eig_sym(eigval, AtA_d); // Use the double precision matrix
		logger.trace("Smallest eigenvalue of AtA: " + lexical_cast<string>(eigval[0]));

		// Full pivot LU decomposition equivalent
		 AtA_d = arma::conv_to<arma::mat>::from(AtA_arma); // ✅ Works
		// convert to double
		arma::mat L, U, P;
		bool success = arma::lu(L, U, P, AtA_d);

		// Estimate rank by counting significant diagonal elements in U
		arma::uword rank = arma::rank(AtA_d);
		logger.trace("Rank of AtA: " + lexical_cast<string>(rank));

		bool invertible = arma::det(AtA_d) != 0;
		if (invertible) {
			logger.trace("AtA is invertible.");
		}
		else {
			logger.trace("AtA is not invertible.");
		}

		// Estimate threshold similar to Eigen
		// Eigen uses: abs(lu.maxPivot()) * lu.threshold()
		// We'll use max(abs(diag(U))) * epsilon
		double maxPivot = arma::abs(U.diag()).max();
		double threshold = 2 * maxPivot * std::numeric_limits<double>::epsilon();

		return static_cast<float>(threshold);
	}







}

