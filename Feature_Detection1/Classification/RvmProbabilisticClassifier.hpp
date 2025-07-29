#pragma once
/*
* RvmProbabilisticClassifier.hpp
* Created on : 18/ 11 / 2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_RVMPROBABILISTICCLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_RVMPROBABILISTICCLASSIFIER_HPP_

#include"ProbabilisticClassifier.hpp"
#include"boost/property_tree/ptree.hpp"
#include<memory>
#include<utility>
namespace Classification
{
	class RvmClassifier;

	/**
	 * RVM classifier that produces pseudo-probabilistic output. The hyperplane distance of a feature vector will be transformed
	 * into a probability using a logistic function p(x) = 1 / (1 + exp(a + b * x)) with x being the hyperplane distance and a and
	 * b being parameters.
	*/
	class ProbabilisticRvmClassifier : public ProbabilisticClassifier
	{
	public:
		/**
		* Constructs a new empty probabilistic RVM classifier.
		*/

		ProbabilisticRvmClassifier();

		/**
		 * Constructs a new probabilistic RVM classifier.
		 *
		 * @param[in] svm The actual RVM.
		 * @param[in] logisticA Parameter a of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
		 * @param[in] logisticB Parameter b of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
		 */
		explicit ProbabilisticRvmClassifier(std::shared_ptr<RvmClassifier> rvm, double logisticA = 0.00556, double logisticB = -2.85);

		bool classify(const cv::Mat& featureVector) const;
		std::pair<bool, double> getConfidence(const cv::Mat& featureVector) const;

		std::pair<bool, double> getProbability(const cv::Mat& featureVector) const ;


		/**
		 * Computes the probability for being positive given the distance of a feature vector to the decision hyperplane.
		 *
		 * @param[in] levelAndDistance The index of the last used filter and distance of that filter level.
		 * @return A pair containing the binary classification result and a probability between zero and one for being positive.
		*/

		std::pair<bool, double> getProbability(std::pair<int, double> levelAndDistance) const;

		//std::pair<bool, double> classifyCached(const cv::Mat& featureVector);

		/**
		 * Changes the logistic parameters of this probabilistic SVM.
		 *
		 * @param[in] logisticA Parameter a of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
		 * @param[in] logisticB Parameter b of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
		*/

		void setLogisticParameter(double logisticA, double logisticB);

		/**
		 * Loads the logistic function's parameters from the Matlab file and returns them.
		 *
		 * @param[in] logisticFilename The name of the Matlab-file containing the logistic function's parameters.
		 * @return A pair (a, b) containing the logistic parameters.
		*/

		static std::shared_ptr<ProbabilisticRvmClassifier> load(boost::property_tree::ptree& subtree);
		/**
		 * @return The actual RVM.
		*/
		static std::pair<double, double> loadSigmoidParamsFromMATLAB(const std::string& logisticFileName);
		/**
		 * Creates a new probabilistic RVM classifier from the parameters given in the ptree sub-tree. Loads the logistic function's
		 * parameters, then passes the loading to the underlying RVM which loads the vectors and thresholds
		 * from the Matlab file.
		 *
		 * @param[in] subtree The subtree containing the config information for this classifier.
		 * @return The newly created probabilistic RVM classifier.
		*/
		std::shared_ptr<RvmClassifier> getRvm() {
			return rvm;
		}

		/**
		 * @return The actual RVM.
		*/
		const std::shared_ptr<RvmClassifier> getRvm() const {
			return rvm;
		}
	
		std::shared_ptr<RvmClassifier> rvm; ///< The actual RVM.
		double logisticA; ///< Parameter a of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
	
		double logisticB; ///< Parameter b of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
	};



}


#endif //FEATUREDETECTION_CLASSIFICATION_RVMPROBABILISTICCLASSIFIER_HPP_

