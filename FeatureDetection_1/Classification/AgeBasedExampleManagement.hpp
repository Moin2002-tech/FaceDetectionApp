#pragma once
/*
* AgeBasedExampleManagement.hpp
* Created on :20/ 11 / 2024
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CLASSIFICATION_AGEBASED_EXAMPLEMANAGEMENT_HPP_
#define FEATUREDETECTION_CLASSIFICATION_AGEBASED_EXAMPLEMANAGEMENT_HPP_

#include"VectorBasedExampleManagement.hpp"
#include<memory>
#include<utility>

namespace Classification 
{
	/**
	 * Example storage that, when reaching maximum size, replaces the oldest training examples with
	 * new ones.
    */
	class AgeBasedExampleManagement : public VectorBasedExampleManagement
	{
	public:
		/**
		 * Constructs a new age based example management.
		 *
		 * @param[in] capacity Maximum amount of stored training examples.
		 * @param[in] requiredSize Minimum amount of training examples required for training.
		*/

		AgeBasedExampleManagement(size_t capacity, size_t requiredSize = 1);

		void add(const std::vector<cv::Mat>& newExample);

	private:
		size_t insertPosition;  ///< The insertion index of new examples.
	};

}
#endif // !FEATUREDETECTION_CLASSIFICATION_AGEBASED_EXAMPLEMANAGEMENT_HPP_

