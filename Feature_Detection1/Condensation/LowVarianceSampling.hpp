#pragma once

/*
* LowVarianceSampling.hpp
* Created on: 2/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_LOWVARIANCESAMPLING_HPP_
#define FEATUREDETECTION_CONDENSATION_LOWVARIANCESAMPLING_HPP_

#include<boost/random/mersenne_twister.hpp>
#include<boost/random/uniform_01.hpp>
#include"ResamplingAlgorithm.hpp"

namespace Condensation
{

	/**
		* Low variance sampling algorithm.
	*/
	class LowVarianceSampling : public ResamplingAlgorithm
	{
	public:

		LowVarianceSampling();

		void resample(const std::vector<std::shared_ptr<Sample>>& samples,
			size_t count, std::vector<std::shared_ptr<Sample>>& newSamples);

	private:

		/**
		 * Computes the sum of the sample weights.
		 *
		 * @param[in] samples The samples.
		 * @return The sum of the sample weights.
		*/
		double computeWeightSum(const std::vector<std::shared_ptr<Sample>>& samples);

		boost::mt19937 generator;         ///< Random number generator.
		boost::uniform_01<> distribution; ///< Uniform real distribution.
	};
}


#endif //FEATUREDETECTION_CONDENSATION_LOWVARIANCESAMPLING_HPP_