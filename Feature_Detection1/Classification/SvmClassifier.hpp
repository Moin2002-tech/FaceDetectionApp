#pragma once

/*
*SvmClassifier.hpp
* Created on : 15/ 11 / 2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_SVMCLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_SVMCLASSIFIER_HPP_

#include"VectorMachineClassifier.hpp"
#include<opencv2/core.hpp>
#include<string>
#include<vector>
#include<fstream>


namespace Classification
{
	/**
	* Classifier based on a Support Vector Machine.
	*/
	class SvmClassifier : public VectorMachineClassifier
	{
	public:
		/**
		 * Constructs a new SVM classifier.
		 *
		 * @param[in] kernel The kernel function.
		*/

		explicit SvmClassifier(std::shared_ptr<kernel> Kernel);
		bool classify(const cv::Mat& featureVector) const;
		std::pair<bool, double> getConfidence(const cv::Mat& featureVector) const;


		/**
		 * Determines the classification result given the distance of a feature vector to the decision hyperplane.
		 *
		 * @param[in] hyperplaneDistance The distance of a feature vector to the decision hyperplane.
		 * @return True if feature vectors of the given distance would be classified positively, false otherwise.
		*/
		bool classify(double hyperplaneDistance) const;

		/**
		 * Computes the classification confidence given the distance of a feature vector to the decision hyperplane.
		 *
		 * @param[in] hyperplaneDistance The distance of a feature vector to the decision hyperplane.
		 * @return A pair containing the binary classification result and the confidence of the classification.
		*/
		std::pair<bool, double> getConfidence(double hyperlaneDistance) const;
		/**
		 * Computes the distance of a feature vector to the decision hyperplane. This is the real distance without
		 * any influence by the offset for configuring the operating point of the SVM.
		 *
		 * @param[in] featureVector The feature vector.
		 * @return The distance of the feature vector to the decision hyperplane.
		*/
		double conputeHyperlaneDistance(const cv::Mat& featureVector) const;

		/**
		 * Changes the parameters of this SVM.
		 *
		 * @param[in] supportVectors The support vectors.
		 * @param[in] coefficients The coefficients of the support vectors.
		 * @param[in] bias The bias.
		*/
		void setSvmParameters( const std::vector<cv::Mat>& supportVector, const std::vector<float>& coeifficients, const double& bias);

		/**
		 * Creates a new SVM classifier from the parameters given in some Matlab file.
		 *
		 * @param[in] classifierFilename The name of the file containing the SVM parameters.
		 * @return The newly created SVM classifier.
		*/

		static std::shared_ptr<SvmClassifier> loadFromMATLAB(const std::string& classifierFileName);

		/**
		 * Creates a new SVM classifier from the parameters given in some text file.
		 *
		 * @param[in] classifierFilename The name of the file containing the SVM parameters.
		 * @return The newly created SVM classifier.
		*/
		static std::shared_ptr<SvmClassifier> loadFromText(const std::string& classifierFileName);

		/**
		* Stores the SVM parameters (kernel, bias, coefficients, support vectors) into a text file.
		 *
		 * @param[in] file The file output stream to store the parameters into.
		*/
		void store(std::ofstream& file);

		/**
		 * Creates a new SVM from parameters (kernel, bias, coefficients, support vectors) given in a text file.
		 *
		 * @param[in] file The file input stream to load the parameters from.
		 * @return The newly created SVM classifier.
		*/
		static std::shared_ptr<SvmClassifier> load(std::ifstream& file);

		/**
		 * @return The support vectors.
		*/
		const std::vector<cv::Mat>& getSupportVector() const 
		{
			return supportVector;
		}
		/**
		 * @return The coefficients of the support vectors.
		*/
		const std::vector<float>& getCoEffienct() const 
		{
			return coEffiencient;
		}

		void setSvmThreshold(double threshold)
		{ svmThreshold = threshold; }
	private:

		double svmThreshold = 0;
	 /**
	  * Stores the values of support vectors one after the other into a file stream. The vectors are seperated by
	  * newlines, while values are seperated by whitespaces.
	  *
	  * @param[in] file The file output stream.
	  * @param[in] supportVectors The support vectors.
	 */
		/*
		void storeSupportVectors(std::ofstream& file, const std::vector<cv::Mat>& supportVectors) {
		for (const cv::Mat& vector : supportVectors) {
			for (size_t row = 0; row < vector.rows; ++row) {
				const T* values = vector.ptr<T>(row);
				for (size_t col = 0; col < vector.cols; ++col) {
					for (size_t channel = 0; channel < vector.channels(); ++channel)
						file << *(values++) << ' ';
				}
			}
			file << '\n';
			}
	     }

		*/
		template <class T>
		void storeSupportVector(std::ofstream &fileName, const std::vector<cv::Mat> &supportVector)
		{
			std::ostringstream buffer;
			for(const cv::Mat& vector :supportVector)
			{
				for(size_t row=0; row <vector.rows;++row )
				{
					const T* values = vector.ptr<T>(row);
					for(size_t col = 0; col <vector.cols; ++col)
					{
						for(size_t channels = 0; channels < vector.channels(); ++channels)
						{
							fileName << *(values++) << ' ';
						}
					}
				}
				buffer << '\n';
			}
			fileName << buffer.str();
		}
		/**
		 * Loads the values of support vectors from a file stream. The vectors should be seperated by newlines, while
		 * the values should be seperated by whitespaces.
		 *
		 * @param[in] file The file input stream.
		 * @param[in] count The number of support vectors.
		 * @param[in] rows The row count of the support vectors' underlying matrix.
		 * @param[in] cols The column count of the support vectors' underlying matrix.
		 * @param[in] channels The channel count of the support vectors' underlying matrix.
		 * @param[in] depth The depth of the support vectors' underlying matrix.
		 * @param[out] supportVectors The support vectors.
		*/
		template <class T>
		static void loadSupportVector(std::ifstream& fileName, size_t count, int rows, int cols, int channels, int depth, std::vector<cv::Mat>& supportVector)
		{
			size_t dimension = rows * cols * channels;
			supportVector.reserve(count);
			for(size_t i =0 ; i<count; ++i)
			{
				cv::Mat vector(rows, cols, CV_MAKETYPE(depth, channels));
				T* values = vector.ptr<T>();
				for(size_t j=0; j< dimension; ++j)
				{
					fileName >> values[j];
					supportVector.emplace_back(vector);
				}
			}
		}
		std::vector<cv::Mat> supportVector;
		std::vector<float> coEffiencient;
		
	};
}

#endif //FEATUREDETECTION_CLASSIFICATION_SVMCLASSIFIER_HPP_
