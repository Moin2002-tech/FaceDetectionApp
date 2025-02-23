#pragma once
/*
* VectorMachineClassifier.hpp
* Created on : 15/ 11 / 2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_VECTOR_MACHINECLASSIFIER_HPP_
#define FEATUREDETECTION_CLASSIFICATION_VECTOR_MACHINECLASSIFIER_HPP_

#include"BinaryClassifier.hpp"
#include"Kernel.hpp"

#include<memory>
#include<string>

namespace Classification
{
	class VectorMachineClassifier : public BinaryClassifier
	{
	public:
		/**
		 * Constructs a new vector machine classifier with a default threshold of zero.
		 *
		 * @param[in] kernel The kernel function.
		*/
		explicit VectorMachineClassifier(std::shared_ptr<kernel> Kernel);
		
		virtual ~VectorMachineClassifier();

		/**
		* @return The threshold to compare the hyperplane distance against for determining the label.
		*/
	
		float getThreshold() const
		{
			return threshold;
		}
		/**
		* @return The kernel function.
		*/

		std::shared_ptr<kernel> getKernel() const
		{
			return Kernel;
		}

		//const std::shared_ptr<kernel> getKernel() const 
		///{
			//return Kernel;
		//}





		/**
		* @return The bias that is subtracted from the sum over all scaled kernel values.
		*/
		float getBias() const 
		{
			return bias;
		}
	protected:
		float threshold;///< The threshold to compare the hyperplane distance against for determining the label.
		std::shared_ptr<kernel> Kernel; ///< The kernel function.
		float bias;///< The bias that is subtracted from the sum over all scaled kernel values.
	};
}

#endif //FEATUREDETECTION_CLASSIFICATION_VECTOR_MACHINECLASSIFIER_HPP_