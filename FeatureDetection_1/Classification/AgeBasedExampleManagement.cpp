/*
* AgeBasedExampleManagement.cpp
* Created on: 23/11/2024
* Author : Moin Shaikh
*/

#include"AgeBasedExampleManagement.hpp"

using std::vector;
using cv::Mat;

namespace Classification 
{
	AgeBasedExampleManagement::AgeBasedExampleManagement(size_t capacity,size_t requiredSize) : 
		VectorBasedExampleManagement(capacity, requiredSize), insertPosition(0) {}


	void AgeBasedExampleManagement::add(const vector<Mat> &newExamples)
	{
		// add new training examples as long as there is space available
		auto example = newExamples.cbegin();
		for (; examples.size() < examples.capacity() && example != newExamples.cend();++example) 
		{
			examples.emplace_back(*example);
		}
		for(;example != newExamples.cend(); ++example)
		{
			examples.at(insertPosition) = *example;
			++insertPosition;
			if (insertPosition == examples.size())
			{
				insertPosition = 0;
			}
		}

	}
}
