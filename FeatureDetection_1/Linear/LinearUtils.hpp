#pragma once
/*
* LinearUtils.hpp
* Created on: 23/9/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_LINEAR_LINEARCPP_LINEARUTILS_HPP_
#define FEATUREDETECTION_LINEAR_LINEARCPP_LINEARUTILS_HPP_

#include<opencv2/core.hpp>
#include<unordered_map>
#include<memory>
#include<vector>
#include"Linear.h"

struct feature_Node;
struct parameter;
struct problem;
struct model;


namespace LinearCPP
{
	/**
	* Deleter of the libLinear parameter.
	*/
	class ParameterDeleter
	{
	public:
		void operator()(struct parameter* param) const;
	};
	class problemDeleter
	{
	public:
		void operator()(struct problem* problem) const;
	};
	class modelDeleter
	{
	public:
		void operator()(struct model* model) const;
	};
	/**
	* Utility class for libLinear with functions for creating nodes and computing SVM outputs. Usable via
	* composition or inheritance.
	*/
	class linearUtils
	{
	public:
	
		linearUtils();

		virtual ~linearUtils();
		/**
		 * Creates a new Linear node from the given feature vector data.
		 *
		 * @param[in] vector The feature vector.
		 * @param[in] bias Flag that indicates whether there should be a bias.
		 * @return The newly created libLinear node.
		*/
		std::unique_ptr<struct feature_node[]> createNodes(const cv::Mat& vector, bool bias);
		/**
		 * Computes the SVM output given a Linear node.
		 *
		 * @param[in] model The libLinear model.
		 * @param[in] x The libLinear node.
		 * @return The SVM output value.
		*/
		double computeSvmOutput(const struct model* model, const struct feature_node* x) const;

		/**
		 * Extracts the support vectors from a libLinear model.
		 *
		 * @param[in] model The libLinear model.
		 * @return The support vectors.
		 */
		std::vector<cv::Mat> extractSupportVectors(const struct model* model) const ;

		/**
		 * Extracts the coefficients from a libLinear model.
		 *
		 * @param[in] model The libLinear model.
		 * @return The coefficients.
		 */
		std::vector<float> extractCoefficients(struct model* model) const;

		/**
		 * Extracts the bias from a libLinear model.
		 *
		 * @param[in] model The libLinear model.
		 * @return The bias.
		 */
		double extractBias(struct model* model) const;

		/**
		 * @return The amount of dimensions of the feature vectors.
		 */
		size_t getDimensions() const;
	
		/**
		 * Fills a libLinear node with the data of a feature vector.
		 *
		 * @param[in,out] node The libLinear node.
		 * @param[in] vector The feature vector.
		 * @param[in] size The size of the data.
		*/
	private:
		template <class T>
		void fillNode(struct feature_node* node, const cv::Mat& vector, size_t size) const;

		/**
		 * Fills a vector with the data of a libSVM node.
		 *
		 * @param[in,out] vector The vector.
		 * @param[in] node The libSVM node.
		 * @param[in] size The size of the data.
		*/
		template<class T>
		void extractWeightVector(cv::Mat& vector, const struct model* model) const;

		mutable int matRows;  ///< The row count of the support vector data.
		mutable int matCols;  ///< The column count of the support vector data.
		mutable int matType;  ///< The type of the support vector data.
		mutable int matDepth; ///< The depth of the support vector data.
		mutable size_t dimensions; ///< The amount of dimensions of the feature vectors.
	};
}




#endif //FEATUREDETECTION_LINEAR_LINEARCPP_LINEARUTILS_HPP_