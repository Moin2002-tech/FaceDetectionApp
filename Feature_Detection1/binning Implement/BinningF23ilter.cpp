
#include<iostream>
#include"../ImageProcessing/ColorBiningFilter.hpp"
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
using namespace ImageProcessing;

/* 
* 
* int main()
{
	std::string path = "./Feature_Detection1/binning Implement/pic.jpg";
	cv::Mat image = cv::imread(path);
	cv::Mat CV_8UC3221;
	image.convertTo(CV_8UC3221, CV_8UC3);
	cv::Mat filtered;
	ImageProcessing::ColorBinningFilter color(30);
	color.applyTo(CV_8UC3221, filtered);
	filtered.convertTo(filtered, CV_8U);
	cv::imshow("filtered", filtered);
	cv::waitKey();

}
*
*
*/
