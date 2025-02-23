#pragma once
/*
* ResamplingSampler.hpp
* Created on: 21/1/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CONDENSATION_RESAMPLINGSAMPLER_HPP_
#define FEATUREDETECTION_CONDENSATION_RESAMPLINGSAMPLER_HPP_


#include"Sampler.hpp"
#include<boost/random/mersenne_twister.hpp>
#include<boost/random/uniform_int.hpp>
#include<boost/random/uniform_real.hpp>
#include<memory>

using boost::mt19937;
using boost::uniform_int;
using boost::uniform_real;
using std::shared_ptr;
using std::vector;

namespace Condensation
{
	class ResamplingAlgorithm;
	class TransitionModel;

	/**
	 * Creates new samples by resampling the previous ones and moving them according to a transition model.
	 * Additionally, some samples are randomly sampled across the image.
	*/

	
	class ResamplingSampler : public Sampler
	{
	public:
		/**
		 * Constructs a new resampling sampler.
		 *
		 * @param[in] count The number of samples.
		 * @param[in] randomRate The percentage of samples that should be equally distributed.
		 * @param[in] resamplingAlgorithm The resampling algorithm.
		 * @param[in] transitionModel The transition model.
		 * @param[in] minSize The minimum size of a sample.
		 * @param[in] maxSize The maximum size of a sample.
		*/
		ResamplingSampler(unsigned int count, double randomRate, shared_ptr<ResamplingAlgorithm> resamplingAlgorithm,
			shared_ptr<TransitionModel> transitionModel, int minSize, int maxSize);
		void init(const cv::Mat& image) override;


		void sample(const vector<shared_ptr<Sample>>& samples, vector<shared_ptr<Sample>>& newSamples,
			const cv::Mat& image, const shared_ptr<Sample> target) override;


		/**
		 * @return The number of samples.
		*/
		inline int getCount()
		{
			return count;
		}

		/**
		* @param[in] The new number of samples.
		*/
		inline void setCount(unsigned int count)
		{
			this->count = count;
		}

		/**
			* @return The percentage of samples that should be equally distributed.
		*/

		inline double getRandomRate()
		{
			return randomRate;
		}

		/**
			* @param[in] The new percentage of samples that should be equally distributed.
		*/
		inline void setRandomRate(double randomRate)
		{
			this->randomRate = randomRate;
		}
	private:

		/**
		 * Randomly samples new values for a sample.
		 *
		 * @param[in,out] sample The sample.
		 * @param[in] image The image.
		*/

		void sampleValues(Sample& sample,const cv::Mat& image);

		unsigned int count; //////< The number of samples.
		double randomRate;  ///< The percentage of samples that should be equally distributed.
		std::shared_ptr<ResamplingAlgorithm> resamplingAlgorithm; ///< The resampling algorithm.
		std::shared_ptr<TransitionModel> transitionModel;         ///< The transition model.

		int minSize; ///< The minimum size of a sample.
		int maxSize; ///< The maximum size of a sample.

		boost::mt19937 generator; ///< Random number generator.
		boost::uniform_int<> intDistribution;   ///< Uniform integer distribution.
		boost::uniform_real<> realDistribution; ///< Uniform real distribution.

	};


}

#endif //FEATUREDETECTION_CONDENSATION_RESAMPLINGSAMPLER_HPP_