/*
* Texture.hpp
* Created on: 3/9/2024
* Author : Moin Shaikh
*/

#include"Texture.hpp"
#include"MatrixUtils.hpp"

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

#include<iostream>


namespace Render 
{
	void Texture::createFromFile(const std::string& fileName, unsigned int mipmapNum) 
	{
		cv::Mat image;
		try 
		{
			cv::imread(fileName);
		}
		catch(std::exception &e)
		{
			std::cout << "OpenCV exception Caught during loading Image " << e.what() << "\n";
		}
		if (image.empty())
		{
			std::cout << "ERROR :: Image is Empty :: imread: Loading Image " << fileName << "\n";
		}

		this->mipmapNum = (mipmapNum == 0 ? Render::utils::getMaxPossibleMipmapsNum(image.cols, image.rows) : mipmapNum);

		if (this->mipmapNum > 1) 
		{
			if (!isPowerOfTwo(image.cols) || !isPowerOfTwo(image.rows)) 
			{
			
				std::cout << "Error: Couldn't generate mipmaps for image: " << fileName << std::endl;
				exit(EXIT_FAILURE);

			}
		}

		image.convertTo(image, CV_8UC4);	// Most often, the input img is CV_8UC3. Img is BGR. Add an alpha channel
		cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);

		int currWidth = image.cols;
		int currHeight = image.rows;
		for (int i = 0; i < this->mipmapNum; i++)
		{
			if (i == 0) {
				mipmaps.push_back(image);
			}
			else {
				cv::Mat currMipMap(currHeight, currWidth, CV_8UC4);
				cv::resize(mipmaps[i - 1], currMipMap, currMipMap.size());
				mipmaps.push_back(currMipMap);
			}

			if (currWidth > 1)
				currWidth >>= 1;
			if (currHeight > 1)
				currHeight >>= 1;
		}
		this->fileName = fileName;
		this->widthLog = (uchar)(std::log(mipmaps[0].cols) / CV_LOG2 + 0.0001f); // std::epsilon or something? or why 0.0001f here?
		this->heightLog = (uchar)(std::log(mipmaps[0].rows) / CV_LOG2 + 0.0001f); // Changed std::logf to std::log because it doesnt compile in linux (gcc 4.8). CHECK THAT
	

	}
}