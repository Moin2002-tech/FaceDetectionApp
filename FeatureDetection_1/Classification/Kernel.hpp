#pragma once
/*
* Kernel.hpp
* Created on: 23/9/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_KERNEL_HPP_
#define FEATUREDETECTION_CLASSIFICATION_KERNEL_HPP_

#include<opencv2/core.hpp>

namespace Classification
{
	class KernelVisitor;
	/**
	* A kernel function that computes the dot product of two vectors in a potentially high or infinite dimensional space.
	*/
	class kernel
	{
	public:
		virtual ~kernel(){}

		/**
		 * Computes the kernel value (dot product in a potentially high dimensional space) of two given vectors.
		 *
		 * @param[in] lhs The first vector.
		 * @param[in] rhs The second vector.
		 * @return The kernel value of the two vectors.
		 */

		virtual const double compute(const cv::Mat& lhs, const cv::Mat& rhs) = 0;
		/**
		 * Accepts a visitor.
		 *
		 * @param[in] visitor The visitor.
		*/
		virtual void accept(KernelVisitor& visitor) = 0;
	};

}


#endif //FEATUREDETECTION_CLASSIFICATION_KERNEL_HPP_