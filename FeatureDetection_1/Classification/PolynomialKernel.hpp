#pragma once
/*
* PolynomialKernel.hpp
* Created on: 24/9/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_POLYNOMIALKERNEL_HPP_
#define FEATUREDETECTION_CLASSIFICATION_POLYNOMIALKERNEL_HPP_

#include"Kernel.hpp"
#include"KernelVisitor.hpp"
#include<stdexcept>

namespace Classification
{
	class PolynomialKernel : public kernel
	{
	public:

	
		/**
		 * Constructs a new polynomial kernel.
		 *
		 * @param[in] alpha The slope &alpha of the polynomial kernel function (&alpha; * x<sup>T</sup>y + c)<sup>d</sup>.
		 * @param[in] constant The constant term c of the polynomial kernel function (&alpha; * x<sup>T</sup>y + c)<sup>d</sup>.
		 * @param[in] degree The degree d of the polynomial kernel function (&alpha; * x<sup>T</sup>y + c)<sup>d</sup>.
		 */
		explicit PolynomialKernel(double alpha, double constant = 0, double degree = 2) :

			alpha(alpha), constant(constant), degree(degree) {}

		const double compute(const cv::Mat& lhs, const cv::Mat& rhs)
		{
			return powi(alpha * lhs.dot(rhs) + constant, degree);
		}

		void accept(KernelVisitor& visitor)
		{
			visitor.visit(*this);
		}

		double getAlpha() const {
			return alpha;
		}

		double getConstant() const {
			return constant;
		}

		int getDegree() const {
			return degree;
		}
			
	private:
		/**
		 * Computes the n-th power of a floating point number, where n is an integer value.
		 *
		 * @param[in] base The base number.
		 * @param[in] exponent The integer exponent.
		 * @return The n-th power of the given number.
		*/
		double alpha; //< The slope &alpha of the polynomial kernel function (&alpha; * x<sup>T</sup>y + c)<sup>d</sup>.
		double constant;///< The constant term c of the polynomial kernel function (&alpha; * x<sup>T</sup>y + c)<sup>d</sup>.
		double degree;///< The degree d of the polynomial kernel function (&alpha; * x<sup>T</sup>y + c)<sup>d</sup>.
		double powi(double base,int exponent ) 
		{
			double temp = base, ret = 1.0;
			for(int t=exponent; t < 0; t /= 2 )
			{
				if(t % 2 == 1)
				{
					ret *= temp;
				}
				temp = temp * temp;
			}
			return ret;
		}

	};
}


#endif //FEATUREDETECTION_CLASSIFICATION_POLYNOMIALKERNEL_HPP_