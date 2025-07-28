#include<iostream>
#include"../ImageProcessing/Filtering/HistogramFiltering.hpp"
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>



using namespace ImageProcessing::Filtering;


using cv::Mat;


int main()
{
	cv::Mat image = cv::imread("./Feature_Detection1/binning Implement/pic.jpg",cv::IMREAD_GRAYSCALE);
	cv::Mat filteredImage;
	cv::Mat diplayedImage;
	cv::imshow("Original Image", image);
	int bincount = 1;
	float upperbound = 1.0f;
	
	image.convertTo(image, CV_32F, 1.0 / 255.0); // Convert to float for filtering
	HistogramFilter hist(bincount, upperbound);
	hist.applyTo(image, filteredImage);
	
	cv::Mat display;
	cv::Mat Image8UC;
	//normalize
	cv::normalize(filteredImage, display, 0, 255, cv::NORM_MINMAX);
	display.convertTo(Image8UC, CV_8U);
	//std::cout << filteredImage;
	cv::imshow("Hist", Image8UC);
	cv::waitKey(0);

}