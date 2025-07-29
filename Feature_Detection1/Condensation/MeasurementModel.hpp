#pragma once
/*
* MeasurementModel.hpp
* Created on: 22/1/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_MEASUREMENTMODEL_HPP_
#define FEATUREDETECTION_CONDENSATION_MEASUREMENTMODEL_HPP_


#include<opencv2/core.hpp>
#include<vector>
using std::vector;
using cv::Mat;
using cv::Rect;

namespace ImageProcessing
{
	class VersionedImage;
}

namespace Condensation
{
	class Sample;

	/**
		* Measurement model for samples.
	*/

	class MeasurementModel

	{
	public:
		virtual ~MeasurementModel(){}

		/**
		 * Updates this model so all subsequent calls to evaluate use the data of the new image.
		 *
		 * @param[in] image The new image data.
		*/

		virtual void update(std::shared_ptr<ImageProcessing::VersionedImage> image) = 0;


		/**
		 * Changes the weight of the sample according to the likelihood of an object existing at that positions an image.
		 *
		 * @param[in] sample The sample whose weight will be changed according to the likelihood.
		*/
		virtual void evaluate(Sample& sample) const = 0;
		/**
		 * Changes the weights of samples according to the likelihood of an object existing at that positions an image. Can
		 * be used instead of update and calls to evaluate for each sample individually.
		 *
		 * @param[in] image The image.
		 * @param[in] samples The samples whose weight will be changed according to the likelihoods.
		*/

		virtual void evaluate(std::shared_ptr<ImageProcessing::VersionedImage> image, std::vector<std::shared_ptr<Sample>>& samples)
		{
			update(image);
			for (std::shared_ptr<Sample> sample : samples)
			{
				evaluate(*sample);
			}
			
		}


	};
}
#endif //FEATUREDETECTION_CONDENSATION_MEASUREMENTMODEL_HPP_