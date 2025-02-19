#pragma once
/*
* Sampler.hpp
* Created on: 21/1/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_SAMPLER_HPP_
#define FEATUREDETECTION_CONDENSATION_SAMPLER_HPP_

#include<opencv2/core.hpp>
#include<memory>
#include<vector>

namespace Condensation
{
	class Sample;

	/**
	* Creates new samples.
	*/
	class Sampler
	{
	public:
	
		virtual ~Sampler(){}

		/**
		 * Initializes this sampler.
		 *
		 * @param[in] image The current image.
		*/
		virtual void init(const cv::Mat& image) = 0;

		/**
		 * Creates new samples.
		 *
		 * @param[in] samples The vector containing the samples of the previous time step.
		 * @param[in,out] newSamples The vector to insert the new samples into.
		 * @param[in] image The new image.
		 * @param[in] target The previous target state.
		*/

		virtual void sample(const std::vector<std::shared_ptr<Sample>> &samples,std::vector<std::shared_ptr<Sample>> &newSamples, 
			const cv::Mat& image, const std::shared_ptr<Sample> target) = 0;

	};
	}


#endif //FEATUREDETECTION_CONDENSATION_SAMPLER_HPP_