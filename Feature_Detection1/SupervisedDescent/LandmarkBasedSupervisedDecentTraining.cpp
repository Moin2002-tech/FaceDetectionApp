

#include "LandmarkBasedSupervisedDecentTraining.hpp"
#include "LinearRegression.hpp"
#include"../logging/LoggerFactory.hpp"
#include"hog.h"

#include<chrono>
#include<fstream>
#include<random>

#include<opencv2/imgproc.hpp>


namespace superVisedDescent
{


	cv::Mat LandmarkBaseSupervisedDecentTraining::calculateMean(cv::Mat& landmark, 
		AlignGroundtruth alignGroundTruth,
		MeanNormalization meanNormalization,
		std::vector<cv::Rect> faceBoxes)
	{
		logging::Logger logger = logging::LoggerFactory::Instance()->getLogger("superviseddecent");
		if(landmark.empty())
		{
			string message("No Landmark Provided to calculate The mean");
			logger.error(message);
			throw std::runtime_error(message);
		}
			// Calculate the mean-shape of all training images:
		// 1) Prepare the ground-truth landmarks:
		//    Note: We could do some Procrustes and align all shapes to some calculated mean-shape. But actually just the mean calculated like this is a good approximation.
		switch (alignGroundtruth)
		{
		case AlignGroundtruth::NONE:
			break;
		case AlignGroundtruth::NORMALIZED_FACEBOX:
			if(faceBoxes.size() != landmark.rows)
			{
				string message("AlignGroundTruth is set to NORMALIZED_FACEBOX but facebox.size() not equal to landmark.size(). please provide a face box for each set of landmark. !");;
				logger.error(message);
				throw std::runtime_error(message);
			}
			for(auto currentImage =0 ;currentImage < landmark.rows;++currentImage)
			{
				Mat transformed = transformLandmarksNormalized(
					landmark.row(currentImage),
					faceBoxes[currentImage]
				);
				transformed.copyTo(landmark.row(currentImage));
			}
			break;
		default:
			string message("AlignGroundTruth is set to an unknown value, please set to an appropriate option provided");
			logger.error(message);
			throw std::runtime_error(message);
			break;
		}

		Mat modelMean;
		cv::reduce(landmark, modelMean, 0, cv::REDUCE_AVG);
		switch(meanNormalization)
		{
		case MeanNormalization::NONE:
			break;
		case MeanNormalization::UNIT_SUM_SQUARED_NORMS:
			modelMean = meanNormalizationUnitSumSquaredNorms(modelMean);
			break;
		default:
			string message("MeanNormalization is set to an unknown value, please set to an appropriate option provided");
			logger.error(message);
			throw std::runtime_error(message);
			break;
		}

		return modelMean;
	}

	AlignmentStatistics LandmarkBaseSupervisedDecentTraining::calculateAligmentStatistics(
		std::vector<cv::Rect> &trainingFaceboxes,
		cv::Mat& groundtruthLandmarks,
		cv::Mat& initialShapeEstimateX0)
	{
		int numTrainingData = groundtruthLandmarks.rows;
		Mat delta_tx(numTrainingData, 1, CV_32FC1);
		Mat delta_ty(numTrainingData, 1, CV_32FC1);
		Mat delta_sx(numTrainingData, 1, CV_32FC1);
		Mat delta_sy(numTrainingData, 1, CV_32FC1);

		int numModelLandmarks = groundtruthLandmarks.cols / 2;
		int currentImage = 0;
		LinearRegression regression;
		for(auto currentImage = 0; currentImage < numTrainingData ;++currentImage)
		{
			cv::Rect detectedFaces = trainingFaceboxes[currentImage];
			// calculate the centroid and the min-max bounding-box (for the width/height) of the ground-truth and the initial estimate x0:
			Mat groundtruth_x = groundtruthLandmarks.row(currentImage).colRange(0, numModelLandmarks);
			Mat groundtruth_y = groundtruthLandmarks.row(currentImage).colRange(numModelLandmarks, numModelLandmarks * 2);

			Mat initialShapeEstimateX0_x = initialShapeEstimateX0.row(currentImage).colRange(0, numModelLandmarks);
			Mat initialShapeEstimateX0_y = initialShapeEstimateX0.row(currentImage).colRange(numModelLandmarks, numModelLandmarks * 2);

			delta_tx.at<float>(currentImage) = regression.calculateMeanTranslation(groundtruth_x, initialShapeEstimateX0_x) / detectedFaces.width; // mean translation in relation to the V&J face-box
			delta_ty.at<float>(currentImage) = regression.calculateMeanTranslation(groundtruth_y, initialShapeEstimateX0_y) / detectedFaces.height;

			delta_sx.at<float>(currentImage) = regression.calculateScaleRatio(groundtruth_x, initialShapeEstimateX0_x);
			delta_sy.at<float>(currentImage) = regression.calculateScaleRatio(groundtruth_y, initialShapeEstimateX0_y);
		}
		AlignmentStatistics alignmentStatistics;
		Mat mmu_t_x, mmu_t_y, mmu_s_x, mmu_s_y, msigma_t_x, msigma_t_y, msigma_s_x, msigma_s_y;
		cv::meanStdDev(delta_tx, mmu_t_x, msigma_t_x);
		cv::meanStdDev(delta_ty, mmu_t_y, msigma_t_y);
		cv::meanStdDev(delta_sx, mmu_s_x, msigma_s_x);
		cv::meanStdDev(delta_sy, mmu_s_y, msigma_s_y);
		alignmentStatistics.tx.mu = mmu_t_x.at<double>(0);
		alignmentStatistics.ty.mu = mmu_t_y.at<double>(0);
		alignmentStatistics.sx.mu = mmu_s_x.at<double>(0);
		alignmentStatistics.sy.mu = mmu_s_y.at<double>(0);
		alignmentStatistics.tx.sigma = msigma_t_x.at<double>(0);
		alignmentStatistics.ty.sigma = msigma_t_y.at<double>(0);
		alignmentStatistics.sx.sigma = msigma_s_x.at<double>(0);
		alignmentStatistics.sy.sigma = msigma_s_y.at<double>(0);

		return alignmentStatistics;

	}

	cv::Mat LandmarkBaseSupervisedDecentTraining::rescaleModel(cv::Mat& modelMean, const AlignmentStatistics& alignmentStatistics)
	{
		Mat modelMean_x = modelMean.colRange(0, modelMean.cols / 2);
		Mat modelMean_y = modelMean.colRange(modelMean.cols / 2, modelMean.cols);
		modelMean_x = (modelMean_x - alignmentStatistics.tx.mu) / alignmentStatistics.sx.mu;
		modelMean_y = (modelMean_y - alignmentStatistics.ty.mu) / alignmentStatistics.sy.mu;
		return modelMean;
	}


	SdmLandmarkModel LandmarkBaseSupervisedDecentTraining::train(std::vector<cv::Mat>& trainingImages, std::vector<cv::Mat>& trainingGroundtruthLandmarks, std::vector<cv::Rect>& trainingFaceboxes, std::vector<std::string>& modelLandmarks, std::vector<std::string>& descriptorTypes, std::vector<std::shared_ptr<DescriptorExtractor>>& descriptorExtractors)
	{
		logging::Logger logger = logging::LoggerFactory::Instance()->getLogger("superviseddecent");
		std::chrono::time_point<std::chrono::system_clock> start, end;
		int  elasped_mseconds;

		// TODO: Don't initialize with numSamples... Instead, push_back. Because
		// Sampling params: DiscardX0Sample. 
		// 1) PERTURB_GT, 2) PERTURB_(X0|DETECTOR_ESTIMATE)
		// NO!!! Perturb the GT by +-mu (mu!=0) makes no sense at all in detection! (maybe in tracking?)

		// Note: In tracking, i can't calculate the mu/sigma normalized wrt the FD face-box anymore. but pixel doesnt make sense either. So use the ied or min/max of the model. (ied bad idea, might not have eyes)

		int numImages = trainingImages.size();
		int numModelLandmarks = modelLandmarks.size();

		Mat groundtruthLandmarks(numImages, 2 * numModelLandmarks, CV_32FC1);
		// Just copy from vector<Mat> to one big Mat:
		for (auto currentImage = 0; currentImage < trainingGroundtruthLandmarks.size(); ++currentImage)
		{
			Mat groundtruthLms = trainingGroundtruthLandmarks[currentImage];
			groundtruthLms.copyTo(groundtruthLandmarks.row(currentImage));
		}

		// Calculate the mean:
		Mat modelMean = calculateMean(groundtruthLandmarks, alignGroundtruth, meanNormalization, trainingFaceboxes);
		//saveShapeInstanceToMatlab(modelMean, "mean.txt");

		// FURTHER MEAN STUFF: Align the mean, calc variance, realign it. Move to calculateScaledMean(...) ?
		// All a flag for this? rescaleMeanToData?
		// Do the initial alignment: (different methods? depending if mean normalized or not?)
		Mat initialShapeToRescaleMean = Mat::zeros(groundtruthLandmarks.rows, groundtruthLandmarks.cols, CV_32FC1); // numTrainingImages x (2 * numModelLandmarks)
		// aligns mean + fb to be x0
		LinearRegression regression;
		for (auto currentImage = 0; currentImage < groundtruthLandmarks.rows; ++currentImage) {
			cv::Rect detectedFace = trainingFaceboxes[currentImage]; // Caution: Depending on flags selected earlier, we might not have detected faces yet!
			Mat initialShapeEstimateX0 = regression.alignMean(modelMean, detectedFace);
			initialShapeEstimateX0.copyTo(initialShapeToRescaleMean.row(currentImage));
		}

		// Calculate the mean and variances of the translational and scaling differences between the initial and true landmark locations. (used for generating the samples)
		// This also includes the scaling/translation necessary to go from the unit-sqnorm normalized mean to one in a reasonably sized one w.r.t. the face-box.
		// This means we have to divide the stddev we draw by 2. The translation is ok though.
		// Todo: We should directly learn a reasonably normalized mean during the training!
		AlignmentStatistics alignmentStatistics = calculateAligmentStatistics(trainingFaceboxes, groundtruthLandmarks, initialShapeToRescaleMean);
		// Rescale the model-mean (only necessary if our mean is not normalized to V&J face-box directly in first steps)
		modelMean = rescaleModel(modelMean, alignmentStatistics);

		// Re-align the new mean to the data
		for (auto currentImage = 0; currentImage < groundtruthLandmarks.rows; ++currentImage) {
			cv::Rect detectedFace = trainingFaceboxes[currentImage]; // Caution: Depending on flags selected earlier, we might not have detected faces yet!
			Mat initialShapeEstimateX0 = regression.alignMean(modelMean, detectedFace);
			initialShapeEstimateX0.copyTo(initialShapeToRescaleMean.row(currentImage));
		}
		// Note/Todo: tx_mu and ty_mu are not zero - why? See bug #66.
		alignmentStatistics = calculateAligmentStatistics(trainingFaceboxes, groundtruthLandmarks, initialShapeToRescaleMean);

		// 3. For every training image:
		// Store the initial shape estimate (x_0) of the image (using the rescaled mean), plus generate 10 samples and store them as well
		// Do the initial alignment: (different methods? depending if mean normalized or not?)
		Mat initialShapes; // = Mat::zeros((numSamplesPerImage + 1) * trainingImages.size(), 2 * numModelLandmarks, CV_32FC1); // 10 samples + the original data = 11
		// aligns mean + fb to be x0. Note: fills in a matrix that's bigger (i.e. numSamplesPerImage as big)
		for (auto currentImage = 0; currentImage < trainingImages.size(); ++currentImage) {
			cv::Rect detectedFace = trainingFaceboxes[currentImage]; // Caution: Depending on flags selected earlier, we might not have detected faces yet!
			// Align the model to the current face-box. (rigid, only centering of the mean). x_0
			Mat initialShapeEstimateX0 = regression.alignMean(modelMean, detectedFace);
			initialShapes.push_back(initialShapeEstimateX0);
			Mat img = trainingImages[currentImage];
			regression.DrawLandmark(img, initialShapeEstimateX0);
			cv::rectangle(img, detectedFace, Scalar(0.0f, 0.0f, 255.0f));
			// c) Generate Monte Carlo samples? With what variance? x_0^i (maybe make this step 3.)
			// sample around initialShapeThis, store in initialShapes
			//		Save the samples, all in a matrix
			//		Todo 1) don't use pixel variance, but a scale-independent one (normalize by IED?)
			//			 2) calculate the variance from data (gt facebox?)
			for (int sample = 0; sample < numSamplesPerImage; ++sample) {
				Mat shapeSample = regression.getPerturbedShape(modelMean, alignmentStatistics, detectedFace);
				initialShapes.push_back(shapeSample);
				regression.DrawLandmark(img, shapeSample);
				// Check if the sample goes outside the feature-extractable region?
				// TODO: The scaling needs to be done in the normalized facebox region?? Try to write it down?
				// Better do the translation in the norm-FB as well to be independent of face-size? yes we do that now. Check the Detection-code though!
			}
		}

		// 4. For every shape (sample plus the original image): (loop through the matrix)
		//			a) groundtruthShape, initialShapes
		//				deltaShape = ground - initial
		// Duplicate each row in groundtruthLandmarks for every sample, store in groundtruthShapes
		Mat groundtruthShapes = duplicateGroundtruthShapes(groundtruthLandmarks, numSamplesPerImage); // will be (numSamplesPerImage+1)*numTrainingImages x 2*numModelLandmarks

		// We START here with the real algorithm, everything before was data preparation and calculation of the mean

		std::vector<cv::Mat> regressorData; // output

		// Prepare the data for the first cascade step learning. Starting from the mean initialization x0, deltaShape = gt - x0
		Mat deltaShape = groundtruthShapes - initialShapes;
		// Calculate and print our starting error:
		double avgErrx0 = cv::norm(deltaShape, cv::NORM_L1) / (deltaShape.rows * deltaShape.cols); // TODO: Doesn't say much, need to normalize by IED! But maybe not at training time, should work with all landmarks
		logger.debug("Training: Average pixel error starting from the mean initialization: " + lexical_cast<string>(avgErrx0));

		// 6. Learn a regressor for every cascade step
		for (int currentCascadeStep = 0; currentCascadeStep < numCascadeSteps; ++currentCascadeStep) {
			logger.debug("Training regressor " + lexical_cast<string>(currentCascadeStep));
			// b) Extract the features at all landmark locations initialShapes (Paper: SIFT, 32x32 (?))
			//int featureDimension = 128;
			Mat featureMatrix;// = Mat::ones(initialShapes.rows, (featureDimension * numModelLandmarks) + 1, CV_32FC1); // Our 'A'. The last column stays all 1's; it's for learning the offset/bias
			start = std::chrono::system_clock::now();
			int currentImage = 0;
			for (const auto& image : trainingImages) {
				Mat img = image;
				for (int sample = 0; sample < numSamplesPerImage + 1; ++sample) {
					vector<cv::Point2f> keypoints;
					for (int lm = 0; lm < numModelLandmarks; ++lm) {
						float px = initialShapes.at<float>(currentImage * (numSamplesPerImage + 1) + sample, lm);
						float py = initialShapes.at<float>(currentImage * (numSamplesPerImage + 1) + sample, lm + numModelLandmarks);
						keypoints.emplace_back(cv::Point2f(px, py));
					}
					Mat featureDescriptors = descriptorExtractors[currentCascadeStep]->getDescriptor(img, keypoints);
					// concatenate all the descriptors for this sample horizontally (into a row-vector)
					featureDescriptors = featureDescriptors.reshape(0, featureDescriptors.cols * numModelLandmarks).t();
					featureMatrix.push_back(featureDescriptors);
				}
				++currentImage;
			}
			// 5. Add one row to the features
			Mat biasColumn = Mat::ones(initialShapes.rows, 1, CV_32FC1);
			cv::hconcat(featureMatrix, biasColumn, featureMatrix); // Other options: 1) Generate one bigger Mat and use copyTo (memory would be continuous then) or 2) implement a FeatureDescriptorExtractor::getDimension()
			end = std::chrono::system_clock::now();
			elasped_mseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			logger.debug("Total time for extracting the feature descriptors: " + lexical_cast<string>(elasped_mseconds) + "ms.");

			// Perform the linear regression, with the specified regularization
			start = std::chrono::system_clock::now();

			Mat R =regression.linearRegression(featureMatrix, deltaShape, RegularizationType::Automatic);
			regressorData.push_back(R);
			end = std::chrono::system_clock::now();
			elasped_mseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			logger.debug("Total time for solving the least-squares problem: " + lexical_cast<string>(elasped_mseconds) + "ms.");

			// output (optional):
			for (auto currentImage = 0; currentImage < trainingImages.size(); ++currentImage) {
				Mat img = trainingImages[currentImage];
				Mat output = img.clone();
				for (int sample = 0; sample < numSamplesPerImage + 1; ++sample) {
					int currentRowInAllData = currentImage * (numSamplesPerImage + 1) + sample;
					// gt:
					for (int i = 0; i < numModelLandmarks; ++i) {
						cv::circle(output, cv::Point2f(groundtruthShapes.at<float>(currentRowInAllData, i), groundtruthShapes.at<float>(currentRowInAllData, i + numModelLandmarks)), 2, Scalar(255.0f, 0.0f, 0.0f));
					}
					// x_step: x at the position where we learn the features
					for (int i = 0; i < numModelLandmarks; ++i) {
						cv::circle(output, cv::Point2f(initialShapes.at<float>(currentRowInAllData, i), initialShapes.at<float>(currentRowInAllData, i + numModelLandmarks)), 2, Scalar(210.0f, 255.0f, 0.0f));
					}
					// could output x_new: The one after applying the learned R.
					Mat shapeStep = featureMatrix.row(currentRowInAllData) * R;
					Mat x_new = initialShapes.row(currentRowInAllData) + shapeStep;
					for (int i = 0; i < numModelLandmarks; ++i) {
						cv::circle(output, cv::Point2f(x_new.at<float>(i), x_new.at<float>(i + numModelLandmarks)), 2, Scalar(255.0f, 185.0f, 0.0f));
					}
				}
			}

			// Prepare the data for the next step (and to output the error):
			Mat shapeStep = featureMatrix * R;
			initialShapes = initialShapes + shapeStep;
			deltaShape = groundtruthShapes - initialShapes;
			// the error:
			double avgErr = cv::norm(deltaShape, cv::NORM_L1) / (deltaShape.rows * deltaShape.cols); // TODO: Doesn't say much, need to normalize by IED! But maybe not at training time, should work with all landmarks
			logger.debug("Average pixel error after applying all learned regressors: " + lexical_cast<string>(avgErr));
		}
		// Do the following:
		// * (later: GenericSupervisedDescentTraining? abstract base-class or not?)
		// - time measurement / output for mean calc etc
		// - update ML script for ZF's model
		// - more params, mean etc, my mean...
		// - draw curves

		// delete any unnecessary descriptorExtractors, in case the user selected a numCascadeSteps that's smaller than the number of provided descriptorExtractors.
		descriptorTypes.erase(begin(descriptorTypes) + numCascadeSteps, std::end(descriptorTypes));
		descriptorExtractors.erase(begin(descriptorExtractors) + numCascadeSteps, std::end(descriptorExtractors));

		SdmLandmarkModel model(modelMean, modelLandmarks, regressorData, descriptorExtractors, descriptorTypes);
		return model;
	}

	cv::Mat LandmarkBaseSupervisedDecentTraining::transformLandmarksNormalized(cv::Mat landmarks, cv::Rect box)
	{
		// Todo: Quick doc
		Mat transformed(landmarks.rows, landmarks.cols, landmarks.type());
		int numLandmarks = landmarks.cols / 2;
		for (int i = 0; i < numLandmarks; ++i) { // Todo: This could be done without a for-loop I think, by splitting landmarks in a x- and y-Mat
			float normalizedX = ((landmarks.at<float>(i) - box.x) / static_cast<float>(box.width)) - 0.5f;
			float normalizedY = ((landmarks.at<float>(i + numLandmarks) - box.y) / static_cast<float>(box.height)) - 0.5f;
			transformed.at<float>(i) = normalizedX;
			transformed.at<float>(i + numLandmarks) = normalizedY;
		}
		return transformed;
	}

	cv::Mat LandmarkBaseSupervisedDecentTraining::meanNormalizationUnitSumSquaredNorms(cv::Mat& modelMean)
	{
		int numLandmarks =  modelMean.cols / 2;
		Mat modelMeanX = modelMean.colRange(0, numLandmarks);
		Mat modelMeanY = modelMean.colRange(numLandmarks, 2 * numLandmarks);
		// calculate the centroid:
		Scalar cX = cv::mean(modelMeanX);
		Scalar cY = cv::mean(modelMeanY);
		// move all points to the centroid
		modelMeanX = modelMeanX - cX[0];
		modelMeanY = modelMeanY - cY[0];
		// scale so that the average norm is 1/numLandmarks (i.e.: the total norm of all vectors added is 1).
		// note: that doesn't make too much sense, because it follows that the more landmarks we use, the smaller the mean-face will be.
		float currentTotalSquaredNorm = 0.0f;
		for (int p = 0; p < numLandmarks; ++p)
		{
			float x = modelMeanX.at<float>(p);
			float y = modelMeanY.at<float>(p);
			currentTotalSquaredNorm += (x * x + y * y);
		}
		// multiply every vectors coordinate by the sqrt of the currentTotalSquaredNorm
		modelMean /= std::sqrt(currentTotalSquaredNorm);
		return modelMean;

	}




}

