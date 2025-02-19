#pragma once
/*
* VectorBasedExampleManagement.hpp
* Created on : 15/ 11 / 2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CLASSIFICATION_VECTOR_BASED_EXAMPLEMANAGEMENT_HPP_
#define FEATUREDETECTION_CLASSIFICATION_VECTOR_BASED_EXAMPLEMANAGEMENT_HPP_

#include"ExampleManagement.hpp"

	/**
	* Example management that stores the training examples in a single vector.
	*/

namespace Classification
{
	class VectorBasedExampleManagement : public ExampleManagement
	{
	public:
		/**
		 * Constructs a new vector based example management.
		 *
		 * @param[in] capacity Maximum amount of stored training examples.
		 * @param[in] requiredSize Minimum amount of training examples required for training.
		*/
		
		VectorBasedExampleManagement(size_t capacity, size_t requiredSize = 1);

		virtual ~VectorBasedExampleManagement() = 0;
		bool hasRequiredSize() const;
		void clear();
		size_t size() const;

		std::unique_ptr<ExampleManagement::ExampleIterator>iterator() const;


	protected:

		std::vector<cv::Mat> examples; ///< Stored training examples.
		size_t requiredSize; ///< Minimum amount of training examples required for training.

	private:
		/**
			* Example iterator that iterates over the training examples inside a vector.
		*/
		class VectorIterator : public ExampleIterator {
		public:

			/**
			 * Constructs a new vector based example iterator.
			 *
			 * @param[in] examples Training examples to iterate over.
			 */
			VectorIterator(const std::vector<cv::Mat>& examples);

			bool hasNext() const;

			const cv::Mat& next();

		private:

			std::vector<cv::Mat>::const_iterator current; ///< Iterator that points to the current training example.
			std::vector<cv::Mat>::const_iterator end;     ///< Iterator that points behind the last training example.
		};

	};
}

#endif //FEATUREDETECTION_CLASSIFICATION_VECTOR_BASED_EXAMPLEMANAGEMENT_HPP_