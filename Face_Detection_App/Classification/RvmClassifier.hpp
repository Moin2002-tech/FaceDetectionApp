#pragma once
/*
* RvmClassifier.hpp
* Created on : 18/ 11 / 2024
* Author : Moin Shaikh
*/
#ifndef FEATUREDETECTION_CLASSIFICATION_RVMCLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_RVMCLASSIFIER_HPP_

#include"VectorMachineClassifier.hpp"
#include<opencv2/core.hpp>
#include<boost/property_tree/ptree.hpp>
#include<string>
#include<vector>

/**
 * Classifier based on a Reduced Vector Machine.
 * An RVM differs from an SVM in certain points:
 *  - An RVM consists of a smaller number of reduced vectors (that are usually
 *    not a subset of the training data anymore)
 *  - It has a different set of coefficients for every filter-level in the cascade.
 *    Example: RSV n has its own array of weights for all vectors 1 to n.
 *  - Each filter-level has its own threshold at which it classifies a patch.
 *    Usually, different threshold files are available that have different FAR/FRR properties.
 *  - It can be run cascaded, that is it rejects featureVector that are very unlikely to be
 *    positive very early.
 */
namespace Classification 
{
	class RvmClassifier : public VectorMachineClassifier
	{
	public:

		/**
		 * Constructs a new RVM classifier.
		 *
		 * @param[in] kernel The kernel function.
		*/
		explicit RvmClassifier(std::shared_ptr<kernel> kernel, bool cascadeCoEffiencient = true);
		~RvmClassifier();

		bool classify(const cv::Mat& featureVector) const;

		std::pair < bool, double > getConfidence(const cv::Mat& featureVector) const;

		/**
		 * Determines the classification result given the distance of a feature vector to the decision hyperplane.
		 *
		 * @param[in] levelAndDistance The index of the last used filter and distance of that filter level.
		 * @return True if feature vectors of the given distance would be classified positively, false otherwise.
		*/
		bool classify(std::pair<int, double> levelAndDistance) const;

		/**
		 * Computes the classification confidence given the distance of a feature vector to the decision hyperplane.
		 *
		 * @param[in] levelAndDistance The index of the last used filter and distance of that filter level.
		 * @return A pair containing the binary classification result and the confidence of the classification.
		*/
		std::pair<int, double> getConfidence(std::pair<int, double> levelAndDistance) const;

		/**
		 * Computes the distance of a feature vector to the decision hyperplane. This is the real distance without
		 * any influence by the offset for configuring the operating point of the RVM.
		 *
		 * @param[in] featureVector The feature vector.
		 * @return A pair with the index of the last used filter and the distance to the decision hyperplane of that filter level.
		*/

		std::pair<int, double> computeHyperPlaneDistance(const cv::Mat& featureVector) const;

		/**
		 * Computes the distance of a feature vector to the decision hyperplane. This is the real distance without
		 * any influence by the offset for configuring the operating point of the RVM.
		 *
		 * @param[in] featureVector The feature vector.
		 * @param[in] filterLevel The filter-level (i.e. the reduced vector in the cascade) at which to classify.
		 * @return The distance of the feature vector to the decision hyperplane.
		*/

		double computeHyperPlaneDistance(const cv::Mat& featureVector,const size_t &filterLevel) const;

		double computeHyperPlaneDistanceCached(const cv::Mat& featureVector, const size_t& filterLevel, std::vector<double> &filterEvalCache) const;

		/**
		 * Returns the number of filters (RSVs) this RVM is currently using for classifying.
		 *
		 * @return The number of filters used.
	    */
		unsigned int getNumFilterToUse(void) const;

		/**
		 * Sets the number of filters (RSVs) this RVM is currently using for classifying.
		 * If set to zero, all available filters will be used. If set higher than the number
		 * of available filters, then also all available filters will be used.
		 *
		 * @param[in] The number of filters to be used.
		*/
		void setNumfilterToUse(const unsigned int numFilter);


		/**
		 * Creates a new RVM classifier from a Matlab file (.mat) containing the classifier and
		 * a second .mat file containing the thresholds.
		 *
		 * @param[in] classifierFilename The name of the file containing the RVM vectors and weights.
		 * @param[in] thresholdsFilename The name of the file containing the RVM thresholds.
		 * @return The newly created RVM classifier.
		 */
		static std::shared_ptr<RvmClassifier> loadFromMATLAB(const std::string& classifierName, const std::string& thresholdFileName);

		/**
		 * Creates a new RVM classifier from the parameters given in the ptree sub-tree. Passes the
		 * loading to the respective loading routine (at the moment, only Matlab files are supported).
		 *
		 * @param[in] subtree The subtree containing the config information for this classifier.
		 * @return The newly created RVM classifier.
	   */
		static std::shared_ptr<RvmClassifier> load(const boost::property_tree::ptree& subtree);

		
	public:
		double bias;
		std::vector<cv::Mat> supportVector;
		std::vector<std::vector<float>> coEfficients;
		int numFilterToUse;
		std::vector<float> hierarchicalThresholds;
	};
}


#endif //FEATUREDETECTION_CLASSIFICATION_RVMCLASSIFIER_HPP_