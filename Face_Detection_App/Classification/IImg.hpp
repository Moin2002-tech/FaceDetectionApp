#pragma once

/*
* IIMG.hpp
* Created on : 18/ 11 / 2024
* Author : Moin Shaikh
*/
#ifndef FEATUREDETECTION_CLASSIFICATION_IIMG_HPP_ 
#define FEATUREDETECTION_CLASSIFICATION_IIMG_HPP_ 

namespace Classification
{
	/**
	* Integral Image class that represents (stores) integral images and is able to calculate them for a patch.
	*/
	class IImg 
	{
	public:
		float* data;
		int w, h, size, rowsize, colorDepth;
		IImg(void);
		IImg(int, int, int);
		~IImg();
		void calIImgPatch(const unsigned char*, bool); // Calculate the integral image of a patch (needed for WVM histogram equalization)


	};
}
#endif //FEATUREDETECTION_CLASSIFICATION_IIMG_HPP_