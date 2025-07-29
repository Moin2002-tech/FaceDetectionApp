#pragma once
/*
* MaxWeightStateExtractor.hpp
* Created on: 21/1/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CONDENSATION_MAXWEIGHTSTATEEXTRACTOR_HPP_
#define FEATUREDETECTION_CONDENSATION_MAXWEIGHTSTATEEXTRACTOR_HPP_
#include"StateExtractor.hpp"
/**
 * State extractor that uses the state of the sample with the highest weight.
*/
namespace Condensation
{
	class MaxWeightStateExtractor : public StateExtractor
	{
	public:
		/**
		 * Constructs a new max weight state extractor.
		*/
	MaxWeightStateExtractor();


		std::shared_ptr <Sample > extract(const std::vector<std::shared_ptr<Sample>>& sample) override;


	};

}

#endif // FEATUREDETECTION_CONDENSATION_MAXWEIGHTSTATEEXTRACTOR_HPP_
