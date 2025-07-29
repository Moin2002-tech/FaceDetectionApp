#pragma once
/*
* StateExtractor.hpp
* Created on: 21/1/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_STATEEXTRACTOR_HPP_
#define FEATUREDETECTION_CONDENSATION_STATEEXTRACTOR_HPP_

#include"Sample.hpp"
#include<vector>

namespace Condensation
{

	/**
	 * Extractor of the estimated target state given weighted samples of a particle filter.
	*/
	class StateExtractor
	{
	public:
		virtual ~StateExtractor(){}

		/**
		 * Estimates the most probable target state from a set of samples.
		 *
		 * @param[in] samples The samples.
		 * @return The the most probable target state.
		*/
		virtual std::shared_ptr <Sample > extract(const std::vector<std::shared_ptr<Sample>>& sample) = 0;
	};
}


#endif //FEATUREDETECTION_CONDENSATION_STATEEXTRACTOR_HPP_