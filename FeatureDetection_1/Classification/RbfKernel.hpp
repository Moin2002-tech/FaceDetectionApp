#pragma once


/*
* PolynomialKernel.hpp
* Created on: 25/9/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_RBFKERNEL_HPP_
#define FEATUREDETECTION_CLASSIFICATION_RBFKERNEL_HPP_

#include"Kernel.hpp"
#include"KernelVisitor.hpp"
#include<stdexcept>




namespace Classification
{


	class RbfKernel : public kernel
	{
		/**
		 * A radial basis function kernel of the form k(x, y) = exp(-&gamma; * |u - v|²).
		*/
	
	public:

		/**
		 * Constructs a new RBF kernel.
		 *
		 * @param[in] gamma The parameter &gamma; of the radial basis function exp(-&gamma; * |u - v|²).
		*/
		explicit RbfKernel(double Gamma) : Gamma(Gamma){}
		const double compute(const cv::Mat& lhs, const cv::Mat& rhs)
		{
			if (!lhs.isContinuous() || !rhs.isContinuous())
				throw std::invalid_argument("RbfKernel: arguments have to be continuous");
			if (lhs.flags != rhs.flags)
				throw std::invalid_argument("RbfKernel: arguments have to have the same type");
			if (lhs.total() != rhs.total())
				throw std::invalid_argument("RbfKernel: arguments have to have the same length");
			return exp(-Gamma * computeSumOfSquareDifference(lhs, rhs));
		}
		void accept(KernelVisitor& visitor){
			visitor.visit(*this);
		}
		/**
		* @return The parameter &gamma; of the radial basis function exp(-&gamma; * |u - v|²).
		*/
		double getGamma() const {
			return Gamma;
		}


	private:
		double Gamma; ///< The parameter &gamma; of the radial basis function exp(-&gamma; * |u - v|²).

		/**
		 * Computes the sum of the squared differences of two vectors.
		 *
		 * @param[in] lhs The first vector.
		 * @param[in] rhs The second vector.
		 * @return The sum of the squared differences.
		*/
		double computeSumOfSquareDifference(const cv::Mat &lhs,const cv::Mat &rhs) const
		{
			switch (lhs.depth())
			{
			case CV_8U: return computeSumOfSquaredDifferences_uchar(lhs, rhs);
			case CV_32S: return computeSumOfSquaredDifferences_any<int>(lhs, rhs);
			case CV_32F: return computeSumOfSquaredDifferences_any<float>(lhs, rhs);

			}
			throw std::invalid_argument("RbfKernel: arguments have to be of depth CV_8U, CV_32S or CV_32F");
		}

		/**
		 * Computes the sum of the squared differences of two vectors of unsigned characters.
		 *
		 * @param[in] lhs The first vector.
		 * @param[in] rhs The second vector.
		 * @return The sum of the squared differences.
		*/
		int computeSumOfSquaredDifferences_uchar(const cv::Mat& lhs, const cv::Mat& rhs) const
		{
			const uchar* lValues = lhs.ptr<uchar>();
			const uchar* rValues = rhs.ptr<uchar>();
			int sum = 0;
			size_t size = lhs.total() * lhs.channels();
			for (size_t i = 0; i < size; ++i) {
				int diff = lValues[i] - rValues[i];
				sum += diff * diff;
			}
			return sum;
		}
		/**
			 * Computes the sum of the squared differences of two vectors of arbitrary value types.
			 *
			 * @param[in] lhs The first vector.
			 * @param[in] rhs The second vector.
			 * @return The sum of the squared differences.
		*/

		template <class T>
		float computeSumOfSquaredDifferences_any(const cv::Mat &lhs,const cv::Mat &rhs) const
		{
			const T* lValues = lhs.ptr<T>();
			const T* rValues = rhs.ptr<T>();
			float sum = 0;
			size_t size = lhs.total() * lhs.channels();
			for (size_t i = 0; i  < size; ++i)
			{
				int diff = lValues[i] - rValues[i];
				sum += diff * diff;
			}
			return sum;
		}

		
	};
}

#endif //FEATUREDETECTION_CLASSIFICATION_RBFKERNEL_HPP_