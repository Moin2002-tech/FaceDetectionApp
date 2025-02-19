#pragma once
/*
* KernelVisitor.hpp
* Created on: 24/9/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_KERNELVISITOR_HPP_
#define FEATUREDETECTION_CLASSIFICATION_KERNELVISITOR_HPP_


namespace Classification 
{
	class LinearKernel;
	class PolynomialKernel;
	class RbfKernel;
	class histogramIntersectionKernel;

	class KernelVisitor
	{
	public:
		virtual ~KernelVisitor() {}


		virtual void visit(const LinearKernel &kernel) = 0;

		virtual void visit(const PolynomialKernel& polyKernel) = 0;

		virtual void visit(const RbfKernel& RbfKernel) = 0;

		virtual void visit(const histogramIntersectionKernel& histoKernel) = 0;

	};

}

#endif //FEATUREDETECTION_CLASSIFICATION_KERNELVISITOR_HPP_