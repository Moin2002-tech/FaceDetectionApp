#pragma once
/*
* File: SimpleTransitionModel.hpp
* Created on: 2/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_CONDENSATION_SIMPLETRANSITIONMODEL_HPP_
#define FEATUREDETECTION_CONDENSATION_SIMPLETRANSITIONMODEL_HPP_

#include "TransitionModel.hpp"
#include "boost/random/normal_distribution.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/variate_generator.hpp"
#include <memory>

namespace Condensation
{
	/**
	* Simple transition model that diffuses position and velocity.
	*/
	class SimpleTransitionModel : public TransitionModel
	{
	public:

	

		/**
		 * Constructs a new simple transition model.
		 *
		 * @param[in] positionDeviation Standard deviation of the translation noise.
		 * @param[in] sizeDeviation Standard deviation of the scale change noise.
		*/
		explicit SimpleTransitionModel(double positionDeviation, double sizeDeviation);

		void init(const cv::Mat& image);

		void predict(std::vector<std::shared_ptr<Sample>>& samples, const cv::Mat& image, const std::shared_ptr<Sample> target);

		/**
		 * @return The standard deviation of the translation noise.
		*/
		inline double getPositionDeviation() const 
		{
			return positionDeviation;
		}

		/**
		 * @param[in] deviation The new standard deviation of the translation noise.
		*/
		inline void setPositionDeviation(double deviation)
		{
			this->positionDeviation = deviation;
		}

		/**
		 * @return The standard deviation of the scale change noise.
		*/
		inline double getSizeDeviation() const 
		{
			return sizeDeviation;
		}

		/**
		 * @param[in] deviation The new standard deviation of the scale change noise.
		*/
		inline void setSizeDeviation(double deviation) 
		{
			this->sizeDeviation = deviation;
		}

	private:
		double positionDeviation; ///< Standard deviation of the translation noise.
		double sizeDeviation;     ///< Standard deviation of the scale change noise.
		boost::variate_generator<boost::mt19937, boost::normal_distribution<>> generator; ///< Random number generator.
	};
}
#endif //FEATUREDETECTION_CONDENSATION_SIMPLETRANSITIONMODEL_HPP_
