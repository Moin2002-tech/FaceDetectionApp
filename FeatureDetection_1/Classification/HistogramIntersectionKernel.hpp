#pragma once

/*
* PolynomialKernel.hpp
* Created on: 25/9/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_HISTOGRAM_INTERSECTION_KERNEL_HPP_
#define FEATUREDETECTION_CLASSIFICATION_HISTOGRAM_INTERSECTION_KERNEL_HPP_



#include"Kernel.hpp"
#include"KernelVisitor.hpp"
#include<stdexcept>

namespace Classification
{
	/**
	* Histogram intersection kernel of the form k(x, y) = sum<sub>i</sub>(min(x<sub>i</sub>, y<sub>i</sub>)).
	*/
	class histogramIntersectionKernel : public kernel
	{

	public:
		/**
		* Constructs a new histogram intersection kernel.
		*/
		
		histogramIntersectionKernel(){}

		const double compute(const cv::Mat & lhs,const cv::Mat &rhs)
		{
			if (!lhs.isContinuous() || !rhs.isContinuous())
				throw std::invalid_argument("HistogramIntersectionKernel: arguments have to be continuous");
			if (lhs.flags != rhs.flags)
				throw std::invalid_argument("HistogramIntersectionKernel: arguments have to have the same type");
			if (lhs.rows * lhs.cols != rhs.rows * rhs.cols)
				throw std::invalid_argument("HistogramIntersectionKernel: arments have to have the same lenth");
			return computeSumOfMinimums(lhs, rhs);

			
		}

		void accept(const KernelVisitor Visitor);
	private:
		/**
		 * Computes the sum over the minimums of two vectors.
		 *
		 * @param[in] lhs The first vector.
		 * @param[in] rhs The second vector.
		 * @return The sum of the squared differences.
		*/
		double computeSumOfMinimums(const cv::Mat& lhs, const cv::Mat& rhs) const {
			switch (lhs.depth()) {
			case CV_8U: return computeSumOfMinimums_uchar(lhs, rhs);
			case CV_32S: return computeSumOfMinimums_any<int>(lhs, rhs);
			case CV_32F: return computeSumOfMinimums_any<float>(lhs, rhs);
			}
			throw std::invalid_argument("HistogramIntersectionKernel: arguments have to be of depth CV_8U, CV_32S or CV_32F");
		}
		/**
		 * Computes the sum over the minimums of two vectors of unsigned characters.
		 *
		 * @param[in] lhs The first vector.
		 * @param[in] rhs The second vector.
		 * @return The sum of the minimums.
		*/
		int computeSumOfMinimums_uchar(const cv::Mat& lhs, const cv::Mat& rhs) const {
			const uchar* lvalues = lhs.ptr<uchar>();
			const uchar* rvalues = rhs.ptr<uchar>();
			int sum = 0;
			size_t size = lhs.total() * lhs.channels();
			for (size_t i = 0; i < size; ++i)
				sum += std::min(lvalues[i], rvalues[i]);
			return sum;
		}
		/**
		 * Computes the sum over the minimums of two vectors of arbitrary value types.
		 *
		 * @param[in] lhs The first vector.
		 * @param[in] rhs The second vector.
		 * @return The sum of the minimums.
			*/
		template<class T>
		float computeSumOfMinimums_any(const cv::Mat& lhs, const cv::Mat& rhs) const {
			const T* lvalues = lhs.ptr<T>();
			const T* rvalues = rhs.ptr<T>();
			float sum = 0;
			size_t size = lhs.total() * lhs.channels();
			for (size_t i = 0; i < size; ++i)
				sum += std::min(lvalues[i], rvalues[i]);
			return sum;
		}
	};
}
#endif //FEATUREDETECTION_CLASSIFICATION_HISTOGRAM_INTERSECTION_KERNEL_HPP_