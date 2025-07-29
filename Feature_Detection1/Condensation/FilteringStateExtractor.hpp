#pragma once

/*
* FilteringStateExtractor.hpp
* Created on: 2/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_FILTERINGSTATEEXTRACTOR_HPP_
#define FEATUREDETECTION_CONDENSATION_FILTERINGSTATEEXTRACTOR_HPP_

#include"StateExtractor.hpp"
#include<memory>

namespace Condensation
{
	/**
	 * State extractor that filters the samples leaving only those that represent the target and
	 * delegates to another extractor.
	*/
	class FilteringStateExtractor : public StateExtractor
	{
	public:
		/**
		 * Constructs a new filtering state extractor.
		 *
		 * @param[in] extractor The wrapped state extractor.
		*/
		explicit FilteringStateExtractor(std::shared_ptr<StateExtractor> extractor);

		std::shared_ptr<Sample> extract(const std::vector<std::shared_ptr<Sample>>& samples);
	private:
		std::shared_ptr<StateExtractor> extractor;  ///< The wrapped state extractor.


	};
}


#endif //FEATUREDETECTION_CONDENSATION_FILTERINGSTATEEXTRACTOR_HPP_