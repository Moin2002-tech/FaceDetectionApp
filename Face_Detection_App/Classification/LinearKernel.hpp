#pragma once

/*
* LinearKernel.hpp
* Created on: 24/9/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_LINEARKERNEL_HPP_
#define FEATUREDETECTION_CLASSIFICATION_LINEARKERNEL_HPP_

#include"Kernel.hpp"
#include"KernelVisitor.hpp"

namespace Classification
{
	class LinearKernel : public kernel
	{
	public:
		explicit LinearKernel(){}
		 
		const double compute(const cv::Mat& lhs, const cv::Mat& rhs){
			return lhs.dot(rhs);
		 }


		 void accept(KernelVisitor& visitor)  {
			visitor.visit(*this);
	     }


	};
}


#endif //FEATUREDETECTION_CLASSIFICATION_LINEARKERNEL_HPP_
