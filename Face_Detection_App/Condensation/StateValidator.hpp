#pragma once
/*
* StateValidator.hpp
* Created on: 19/1/2025
* Author : Moin Shaikh
*/
#ifndef FEATUREDETECTION_CONDENSATION_STATEVALIDATOR_HPP_
#define FEATUREDETECTION_CONDENSATION_STATEVALIDATOR_HPP_

namespace ImageProcessing
{
	class VersionedImage;
}
#include<vector>
#include<memory>


namespace Condensation
{

	class Sample;
	/**
	* Validator of the target state.
	*/

	class StateValidator
	{
	public:
		/**
			* Validator of the target state.
		*/
		virtual ~StateValidator(){}
		/**
		 * Validates the given target state.
		 *
		 * @param[in] target Target state.
		 * @param[in] samples Samples that were used for extracting the target state.
		 * @param[in] image Current image.
		 * @return True if the given state is valid, false otherwise.
		*/
		virtual bool isValid(const Sample& target, const std::vector < std::shared_ptr < Sample > >& samples, std::shared_ptr < ImageProcessing::VersionedImage > image) = 0;


	
	};
}


#endif //FEATUREDETECTION_CONDENSATION_STATEVALIDATOR_HPP_
