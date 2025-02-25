/*
* File: WhiteningFitler.cpp
* Created on: 11/2/2025
* Author : Moin Shaikh
*/



#include"WhiteningFilter.hpp"
#include<stdexcept>


namespace ImageProcessing
{
	WhiteningFilter::WhiteningFilter(float alpha, float cutOffFrequency) : alpha(alpha), cutoffFrequency(cutoffFrequency),
		filter(cv::Mat()), floatImage(cv::Mat()), fourierImage(cv::Mat())
	{

	}

	cv::Mat WhiteningFilter::applyTo(const cv::Mat &image, cv::Mat &filtered ) const
	{
		if(image.channels() > 1)
		{
			throw std::invalid_argument("WhiteningFilter: the image must have exactly one channel");
		}

		//Fourier Transformation
		image.convertTo(floatImage, CV_32F);
		cv::dft(floatImage, fourierImage, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);

		//whiteningFilter;
		int rows = fourierImage.rows;
		int cols = fourierImage.cols;
		const cv::Mat& filter = getFilter(cols, rows);


		if(fourierImage.isContinuous() && filter.isContinuous())
		{
			cols *= rows;
			rows = 1;
		}

		for(int row = 0; row<fourierImage.rows;++row)
		{
			cv::Vec2f* fourierRow = fourierImage.ptr<cv::Vec2f>(row);
			const float* filterRow = filter.ptr<float>(row);
			for(int col = 0; col < fourierImage.cols; ++col)
			{
				fourierRow[col][0] *= filterRow[col];
				fourierRow[col][1] *= filterRow[col];
			}
		}

		// inverse Fourier transformation
		dft(fourierImage, floatImage, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
		switch (image.type()) 
		{
		case CV_8U:
			floatImage.convertTo(filtered, CV_8U, 1, 127); break;
		case CV_16U:
			floatImage.convertTo(filtered, CV_16U, 1, (1 << 15) - 1); break;
		default: // supported: CV_8S, CV_16S, CV_32S, CV_32F, CV_64F
			floatImage.convertTo(filtered, image.type()); break;
		}
		
		return filtered;
	}

	void WhiteningFilter::applyInPlace(cv::Mat& image) const
	{
		applyTo(image, image);
	}

	const cv::Mat& WhiteningFilter::getFilter(int width, int height) const 
	{
		if (filter.cols != width || filter.rows != height) 
		{
			filter.create(height, width, CV_32F);
			float nyquistFrequency = 0.5;

			for (int row = 0; row < filter.rows; ++row) 
			{
				float* filterRow = filter.ptr<float>(row);
				for (int col = 0; col < filter.cols; ++col) 
				{
					int shiftedRow = (row + filter.rows / 2) % filter.rows;
					int shiftedCol = (col + filter.cols / 2) % filter.cols;
					float fx = -nyquistFrequency + shiftedCol * (2 * nyquistFrequency) / (filter.cols - 1);
					float fy = -nyquistFrequency + shiftedRow * (2 * nyquistFrequency) / (filter.rows - 1);
					float rho = sqrt(fx * fx + fy * fy);
					filterRow[col] = pow(rho, alpha);
					if (cutoffFrequency > 0)
						filterRow[col] *= exp(-pow(rho / cutoffFrequency, 4));
				}
			}
		}
		return filter;
	}
}