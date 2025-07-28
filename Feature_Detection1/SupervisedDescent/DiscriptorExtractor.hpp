#pragma once
/*
* File: DiscriptorExtractor.hpp
* Created on: 17/7/2025
* Author : Moin Shaikh
*/



#ifndef FEATUREDETECTION_SUPERVISEDDESCENT_DISCRIPTOREXTRACTOR_HPP
#define FEATUREDETECTION_SUPERVISEDDESCENT_DISCRIPTOREXTRACTOR_HPP

#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/features2d.hpp>
#include<opencv2/opencv.hpp>

#include<boost/lexical_cast.hpp>

#include<string>
#include<iostream>
#include<vector>

extern "C" {
#include"hog.h"
}
#include"VlHogFilter.hpp"

namespace superVisedDescent
{
	/**
	 * Base-class for extracting descriptors at given points
	 * in an image.
	 * Todo: Check how this integrates with our FeatureExtractors
	 * in libImageProcessing.
	*/

	class DescriptorExtractor
	{
	public:

		virtual ~DescriptorExtractor() {}
		// returns a Matrix, as many rows as points, 1 descriptor = 1 row
		// the default argument is quite ugly in an inheritance hierarchy

		virtual cv::Mat getDescriptor(const cv::Mat& image, std::vector<cv::Point2f>& locator, int windowSizeHalf = 0) = 0;

		// returns a string with its parameters (to be written to a model-file)
		virtual std::string getParameterString() const = 0;
	};


	class SIFTDiscriptorExtractor : public DescriptorExtractor
	{
		// c'tor with param diameter & orientation? (0.0f = right?)
		// and store them as private vars.
		// However, it might be better to store the parameters separately, to be able to share a FeatureDescriptorExtractor over multiple Sdm cascade levels

	public:
		cv::Mat DescriptorExtractor::getDescriptor(const cv::Mat& image, std::vector<cv::Point2f> &locator,int windowSizeHalf)
		{
			cv::Mat grayScaledImage;
			if(image.channels() == 3)
			{
				cv::cvtColor(image, grayScaledImage, cv::COLOR_BGR2GRAY);
			}
			else
			{
				grayScaledImage = image;
			}

			cv::Ptr<cv::SIFT> sift = cv::SIFT::create();
			std::vector<cv::KeyPoint> keyPoints;
			float size = 32.0f;
			float angle = 0.0f;

			for(const auto& loc : locator)
			{
				keyPoints.emplace_back(cv::KeyPoint(loc, size, angle));
			}

			cv::Mat siftDiscriptor;
			sift->compute(grayScaledImage, keyPoints, siftDiscriptor);

			return siftDiscriptor;
		}

		std::string getParameterString() const 
		{
			return std::string();
		};
	};

	class VlHogDiscriptorExtractor : public DescriptorExtractor
	{
	public:
		enum class VlHogType
		{
			DalalTriggs,
			Uoctti
		};

		inline VlHogDiscriptorExtractor(VlHogType hogType) : hogType(hogType)
		{

		}

		VlHogDiscriptorExtractor(VlHogType hogType, int numCells,int cellSize, int numBins) : hogType(hogType), numCells(numCells), cellSize(cellSize),numBins(numBins)
		{
			
		}

	// Maybe split the class in an AdaptiveVlHog and a VlHogDesc...?
	// Or better solution with less code duplication?

		cv::Mat DescriptorExtractor::getDescriptor(const cv::Mat& image, std::vector<cv::Point2f>& locator, int windowSizeHalf)
		{
			cv::Mat grayScaledImage;
			if(image.channels() == 3)
			{
				cv::cvtColor(image, grayScaledImage, cv::COLOR_BGR2GRAY);
			}
			else
			{
				grayScaledImage = image;
			}
			VlHogVariant vlHogVariant;
			switch (hogType)
			{
			case VlHogDiscriptorExtractor::VlHogType::DalalTriggs:
				vlHogVariant = VlHogVariant::VlHogVariantDalalTriggs;
				break;

			case VlHogDiscriptorExtractor::VlHogType::Uoctti:
				vlHogVariant = VlHogVariant::VlHogVariantUoctti;
				break;

			default:
				break;
			}

			int patchWidthHalf;
			bool adaptivePatchSize = false;
			if(windowSizeHalf > 0)
			{
				adaptivePatchSize = true;
			}
			if(adaptivePatchSize)
			{
				// adaptive:
			//int NUM_CELL = 3; // number of cells in the local patch for local feature extraction, i.e.a 3x3 grid
				patchWidthHalf = windowSizeHalf;
				cellSize = 10; // One cell is 10x10
				numCells = 3; // Always 3 for adaptive, 3 * 10 = 30, i.e. always a 30x30 patch
				numBins = 9; // always 4? Or 9 = default of vl_hog ML?
				// Q: When patch < 30, don't resize. If < 30, make sure it's even?
				// Q: 3 cells might not be so good when the patch is small, e.g. does a 2x2 cell make sense?
			}
			else 
			{
				// traditional:
				patchWidthHalf = numCells * (cellSize / 2); // patchWidthHalf: Zhenhua's 'numNeighbours'. cellSize: has nothing to do with HOG. It's rather the number of HOG cells we want.
			}

			//int hogDim1 = (numNeighbours * 2) / hogCellSize; 
			// // i.e. how many times does the hogCellSize fit into our patch
			//int hogDim2 = hogDim1; // as our patch is quadratic, those two are the same
			//int hogDim3 = 16;
			// // VlHogVariantUoctti: Creates 4+3*numOrientations dimensions. DT: 4*numOri dimensions.
			//int hogDims = hogDim1 * hogDim2 * hogDim3;
			//Mat hogDescriptors(locations.size(), hogDims, CV_32FC1); 
			// // better allocate later, when we know ww/hh/dd?

			cv::Mat hogDescriptors; // We'll get the dimensions later from vl_hog_get_*

			for(int i = 0; i< locator.size(); ++i)
			{
				int x = cvRound(locator[i].x);
				int y = cvRound(locator[i].y);
				cv::Mat roiImage;
				if (x - patchWidthHalf < 0 || y - patchWidthHalf < 0 || x + patchWidthHalf >= image.cols || y + patchWidthHalf >= image.rows)
				{
					// The feature extraction location is too far near a border.
					// We extend the image (add a black canvas)
					// and then	
					//extract from this larger image.

					int borderLeft = (x - patchWidthHalf) < 0 ? std::abs(x - patchWidthHalf) : 0; // Our x and y are center.
					int borderTop = (y - patchWidthHalf) < 0 ? std::abs(y - patchWidthHalf) : 0;
					int borderRight = (x + patchWidthHalf) >= image.cols ? std::abs(image.cols - (x + patchWidthHalf)) : 0;
					int borderBottom = (y + patchWidthHalf) >= image.rows ? std::abs(image.rows - (y + patchWidthHalf)) : 0;

					cv::Mat extendedImage = image.clone();
					cv::copyMakeBorder(extendedImage, extendedImage, borderTop, borderBottom, borderLeft, borderRight, cv::BORDER_CONSTANT, cv::Scalar(0));
					cv::Rect roi((x - patchWidthHalf) + borderLeft, (y - patchWidthHalf) + borderRight, patchWidthHalf * 2, patchWidthHalf * 2); // Rect: x y w h. x and y are top-left corner.
					roiImage = extendedImage(roi).clone(); // clone because we need a continuous memory block

				}
				else
				{
					cv::Rect roi(x - patchWidthHalf, y - patchWidthHalf, patchWidthHalf * 2, patchWidthHalf * 2); // x y w h. Rect: x and y are top-left corner. Our x and y are center. Convert.
					// we have exactly the same window as the matlab code.
					// extract the patch and supply it to vl_hog
					roiImage = image(roi).clone(); // clone because we need a continuous memory block
				}

				roiImage.convertTo(roiImage, CV_32FC1); // because vl_hog_put_image expects a float* (values 0.f-255.f)
				if (adaptivePatchSize) 
				{
					cv::resize(roiImage, roiImage, cv::Size(30, 30)); // actually we shouldn't resize when the image is smaller than 30, but Zhenhua does it
					// in his Matlab code. If we don't resize, we probably have to adjust the HOG parameters.
				}

				// vl_hog_new: numOrientations=hogParameter.numBins, transposed (=col-major):false)
				VlHog* hog = vl_hog_new(vlHogVariant, numBins, false); // VlHogVariantUoctti seems to be default in Matlab.
				vl_hog_put_image(hog, (float*)roiImage.data, roiImage.cols, roiImage.rows, 1, cellSize); // (the '1' is numChannels)
				vl_size ww = vl_hog_get_width(hog); // we could assert that ww == hh == numCells
				vl_size hh = vl_hog_get_height(hog);
				vl_size dd = vl_hog_get_dimension(hog); // assert ww=hogDim1, hh=hogDim2, dd=hogDim3
				//float* hogArray = (float*)malloc(ww*hh*dd*sizeof(float));
				cv::Mat hogArray(1, ww * hh * dd, CV_32FC1); // safer & same result. Don't use C-style memory management.
				//vl_hog_extract(hog, hogArray); // just interpret hogArray in col-major order to get the same n x 1 vector as in matlab. (w * h * d)
				vl_hog_extract(hog, hogArray.ptr<float>(0));
				vl_hog_delete(hog);
				cv::Mat hogDescriptor(hh * ww * dd, 1, CV_32FC1);
				// Stack the third dimensions of the HOG descriptor of this patch one after each other in a column-vector
				for (int j = 0; j < dd; ++j) {
					//Mat hogFeatures(hh, ww, CV_32FC1, hogArray + j*ww*hh);
					cv::Mat hogFeatures(hh, ww, CV_32FC1, hogArray.ptr<float>(0) + j * ww * hh); // Creates the same array as in Matlab. I might have to check this again if hh!=ww (non-square)
					hogFeatures = hogFeatures.t(); // Necessary because the Matlab reshape() takes column-wise from the matrix while the OpenCV reshape() takes row-wise.
					hogFeatures = hogFeatures.reshape(0, hh * ww); // make it to a column-vector
					cv::Mat currentDimSubMat = hogDescriptor.rowRange(j * ww * hh, j * ww * hh + ww * hh);
					hogFeatures.copyTo(currentDimSubMat);
				}
				//free(hogArray); // not necessary - we use a Mat.
				//features = [features; double(reshape(tmp, [], 1))];
				// B = reshape(A,m,n) returns the m-by-n matrix B whose elements are taken column-wise from A
				// Matlab (& Eigen, OpenGL): Column-major.
				// OpenCV: Row-major.
				// (access is always (r, c).)
				//Mat currentFeaturesSubrange = hogDescriptors.rowRange(i*hogDims, i*hogDims + hogDims);
				//hogDescriptor.copyTo(currentFeaturesSubrange);
				hogDescriptor = hogDescriptor.t(); // now a row-vector
				hogDescriptors.push_back(hogDescriptor);
			}
			// hogDescriptors needs to have dimensions numLandmarks x hogFeaturesDimension, where hogFeaturesDimension is e.g. 3*3*16=144
			return hogDescriptors;

		}
		std::string DescriptorExtractor::getParameterString() const 
		{
			return std::string("numCells " + boost::lexical_cast<std::string>(numCells) + " cellSize " + boost::lexical_cast<std::string>(cellSize) + " numBins " + boost::lexical_cast<std::string>(numBins));
		}

	private:
		VlHogType hogType;
		int numCells;
		int cellSize;
		int numBins;
	};
}

#endif