#pragma once
/*
* UnlimitedExampleManagement.hpp
* Created on :20/ 11 / 2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_UNLIMITEDEXAMPLE_MANAGEMENT_HPP_ 
#define FEATUREDETECTION_CLASSIFICATION_UNLIMITEDEXAMPLE_MANAGEMENT_HPP_

#include"VectorBasedExampleManagement.hpp"

/**
 * Example storage that never replaces existing training examples (unless cleared).
 */
namespace Classification
{
	class UnlimitedExampleManagement : public VectorBasedExampleManagement
	{
	public:
		/**
		 * Constructs a new unlimited example management.
		 *
		 * @param[in] requiredSize Minimum amount of training examples required for training.
		*/
		explicit UnlimitedExampleManagement(size_t requiredSize = 1);

		void add(const std::vector<cv::Mat>& newExamples);
	};
}
#endif //FEATUREDETECTION_CLASSIFICATION_UNLIMITEDEXAMPLE_MANAGEMENT_HPP_