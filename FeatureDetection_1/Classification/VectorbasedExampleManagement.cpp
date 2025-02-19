/*
* VectorbasedExampleManagement.cpp
* Created on :	18/ 12 / 2024
* Author : Moin Shaikh
*/



#include"VectorBasedExampleManagement.hpp"

namespace Classification
{
	VectorBasedExampleManagement::VectorBasedExampleManagement(size_t capacity, size_t requiredSize) :examples(),requiredSize(requiredSize) 
	{
		examples.reserve(capacity);
	}

	VectorBasedExampleManagement::~VectorBasedExampleManagement() 
	{

	}

	void VectorBasedExampleManagement::clear() 
	{
		examples.clear();
	}

	size_t VectorBasedExampleManagement::size() const
	{
		return examples.size();
	}



	bool VectorBasedExampleManagement::hasRequiredSize() const 
	{
		return examples.size() >= requiredSize;
	}

	std::unique_ptr<ExampleManagement::ExampleIterator> VectorBasedExampleManagement::iterator() const
	{
		return std::unique_ptr<VectorIterator>(new VectorIterator(examples));
	}

	VectorBasedExampleManagement::VectorIterator::VectorIterator(const std::vector<cv::Mat>& examples) : current(examples.cbegin()), end(examples.cend()) 
	{

	}

	bool VectorBasedExampleManagement::VectorIterator::hasNext() const 
	{
		return current != end;
	}


	const cv::Mat& VectorBasedExampleManagement::VectorIterator::next() {
		const cv::Mat& example = *current;
		++current;
		return example;
	}
}
