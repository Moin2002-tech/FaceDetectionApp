/*
* UnlimitedExampleManagement.cpp
* Created on :	18/ 12 / 2024
* Author : Moin Shaikh
*/

#include"UnlimitedExampleManagement.hpp"


namespace Classification
{
	UnlimitedExampleManagement::UnlimitedExampleManagement(size_t requiredSize) : VectorBasedExampleManagement(10, requiredSize) 
	{

	}

	void UnlimitedExampleManagement::add(const std::vector< cv::Mat>& newExample)
	{
		for(const cv::Mat& example : newExample)
		{
			examples.emplace_back(example);
		}

	}
} 