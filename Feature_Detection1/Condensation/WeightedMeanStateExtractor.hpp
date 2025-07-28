#pragma once
/*
* File: WeightedMeanStateExtractor.hpp
* Created on: 4/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_WEIGHTEDMEANSTATEEXTRACTOR_HPP_
#define FEATUREDETECTION_CONDENSATION_WEIGHTEDMEANSTATEEXTRACTOR_HPP_

#include"StateExtractor.hpp"

namespace Condensation
{
	/**
		* State extractor that determines the state by computing the weighted mean of the samples.
	*/
	class WeightedMeanStateExtractor : public StateExtractor {
	public:

		/**
		 * Constructs a new weighted mean state extractor.
		 */
		WeightedMeanStateExtractor();

		std::shared_ptr<Sample> extract(const std::vector<std::shared_ptr<Sample>>& samples);
	};
}

#endif //FEATUREDETECTION_CONDENSATION_WEIGHTEDMEANSTATEEXTRACTOR_HPP_