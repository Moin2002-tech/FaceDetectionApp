#pragma once

/*
* Texture.hpp
* Created on: 28/8/2024
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_RENDER_TEXTURE_HPP_
#define FEATUREDETECTION_RENDER_TEXTURE_HPP_

#include<opencv2/core.hpp>

namespace Render 
{

	class Texture 
	{
	public:
		void createFromFile(const std::string& filename, unsigned int mipmapNum = 0);
		std::vector<cv::Mat> mipmaps;
		unsigned char widthLog, heightLog;
	private:
		std::string fileName;
		unsigned int mipmapNum;

		inline bool isPowerOfTwo(int x) 
		{
			return !(x & (x - 1));
		}
	};

}

#endif //FEATUREDETECTION_RENDER_TEXTURE_HPP_
