#pragma once
/*
ExampleManagement.hpp
* Created on: 14/10/2024
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CLASSIFICATION_EMPTYEXAMPLEMANAGEMENT_HPP_
#define FEATUREDETECTION_CLASSIFICATION_EMPTYEXAMPLEMANAGEMENT_HPP_

#include"ExampleManagement.hpp"
#include<memory>

namespace Classification
{
	/**
	* Examples storage with a size of zero. Will never store any training 
	* examples and state that there are
	* enough examples for training.
	*/
	class EmptyExampleManagement : public ExampleManagement
	{
	public:
		/**
		* Constructs a new empty example management.
		*/

		EmptyExampleManagement() {}

		void add(const std::vector<cv::Mat>& newExamples) {}

		void clear() {}

		size_t size() const {
			return 0;
		}

		bool hasRequiredSize() const {
			return true;
		}
		std::unique_ptr<ExampleManagement::ExampleIterator> iterator() const {
			return std::unique_ptr<ExampleIterator>(new EmptyIterator());

		}
	private:
		class EmptyIterator : public ExampleIterator 
		{
		public:
			EmptyIterator() {

			}
			bool hasNext() const {
				return false;
			}
			const cv::Mat& next() {
				throw std::runtime_error("EmptyExampleManagement::EmptyIterator::next: there is no element to iterate over");
			}

		};
	};
	

}

#endif