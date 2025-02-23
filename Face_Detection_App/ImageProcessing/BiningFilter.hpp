#pragma once
/*
* File: BinningFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTON_IMAGEPROCESSING_BINNINGFILTER_HPP_
#define FEATUREDETECTON_IMAGEPROCESSING_BINNINGFILTER_HPP_


#include"ImageFilter.hpp"

namespace ImageProcessing
{
	/**
		 * Image filter whose result is an image containing bin information of each pixel.
	*/
	class BinningFilter : public ImageFilter 
	{
	public:

		virtual ~BinningFilter() {}

		/**
		 * @return The amount of bins.
		 */
		virtual unsigned int getBinCount() const = 0;
	};


}

#endif //FEATUREDETECTON_IMAGEPROCESSING_BINNINGFILTER_HPP_