/*
 * LibSvmKernelParamSetter.hpp
 *
 *  Created on: 19.11.2013
 *      Author: poschmann
 */

#ifndef LIBSVMKERNELPARAMSETTER_HPP_
#define LIBSVMKERNELPARAMSETTER_HPP_

#include "../Classification/KernelVisitor.hpp"
#include "../Classification/LinearKernel.hpp"
#include "../Classification/PolynomialKernel.hpp"
#include "../Classification/RbfKernel.hpp"
#include "../Classification/HistogramIntersectionKernel.hpp"
#include "svm.h"

namespace libsvm {

class LibSvmKernelParamSetter : public Classification::KernelVisitor {
public:

	/**
	 * Constructs a new libSVM kernel parameter setter.
	 *
	 * @param[in] param The parameters for libSVM training. Will not be deleted on destruction.
	 */
	LibSvmKernelParamSetter(struct svm_parameter *param) : param(param) {}

	~LibSvmKernelParamSetter() {}

	void visit(const Classification::LinearKernel& kernel) {
		param->kernel_type = LINEAR;
	}

	void visit(const Classification::PolynomialKernel& kernel) {
		param->kernel_type = POLY;
		param->degree = kernel.getDegree();
		param->coef0 = kernel.getConstant();
		param->gamma = kernel.getAlpha();
	}

	void visit(const Classification::RbfKernel& kernel) {
		param->kernel_type = RBF;
		param->gamma = kernel.getGamma();
	}

	void visit(const Classification::histogramIntersectionKernel& kernel) {
		param->kernel_type = HIK;
	}

private:

	struct svm_parameter *param; ///< The parameters for libSVM training. Will not be deleted on destruction.
};

} /* namespace libsvm */
#endif /* LIBSVMKERNELPARAMSETTER_HPP_ */
