
/*
* File: VlHogFilter.cpp
* Created on: 19/7/2025
* Author : Moin Shaikh
*/


#include "VlHogFilter.hpp"
namespace superVisedDescent 
{
	VlHogFilter::VlHogFilter(VlHogType hogType, int numCells, int cellSize, int numBins) : hogType(hogType),
		numCells(numCells),
		cellSize(cellSize),
		numBins(numBins)
	{

	}

	cv::Mat VlHogFilter::applyTo(const cv::Mat &image,cv::Mat &filtered) const
	{
		filtered = cv::Mat();
		return filtered;
	}

}

