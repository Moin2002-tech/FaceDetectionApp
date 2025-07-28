#pragma once
/*
* AdaptiveMeasurementModel.hpp
* Created on: 22/1/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_CONDENSATION_ADAPTIVEMEASUREMENTMODEL_HPP_
#define FEATUREDETECTION_CONDENSATION_ADAPTIVEMEASUREMENTMODEL_HPP_

#include"MeasurementModel.hpp"
namespace Condensation
{

	class AdaptiveMeasurementModel : public MeasurementModel
	{
	public:
		virtual ~AdaptiveMeasurementModel() {}

		using MeasurementModel::update;
		using MeasurementModel::evaluate;

		/**
		 * @return True if this measurement model may be used, false otherwise.
		*/
		virtual bool isUsable() const = 0;

		/**
		 * Initializes this model to the given target.
		 *
		 * @param[in] image The image.
		 * @param[in] target The initial target position and sample.
		 * @return True if the model was updated, false otherwise.
		*/

		virtual bool initialize(std::shared_ptr<ImageProcessing::VersionedImage> image, Sample& target) = 0;
		/**
		 * Adapts this model to the given target.
		 *
		 * @param[in] image The image.
		 * @param[in] samples The weighted samples.
		 * @param[in] target The estimated target position.
		 * @return True if the model was updated, false otherwise.
		*/

		virtual bool adapt(std::shared_ptr<ImageProcessing::VersionedImage> image, const std::vector<std::shared_ptr<Sample>>& samples, const Sample& target) = 0;


		/**
		 * Adapts this model in case the target was not found.
		 *
		 * @param[in] image The image.
		 * @param[in] samples The weighted samples.
		 * @return True if the model was updated, false otherwise.
		*/
		virtual bool adapt(std::shared_ptr<ImageProcessing::VersionedImage> image, const std::vector<std::shared_ptr<Sample>>& samples) = 0;

		/**
			* Resets this model to its original state.
		*/

		virtual void reset() = 0;
	};
}


#endif //FEATUREDETECTION_CONDENSATION_ADAPTIVEMEASUREMENTMODEL_HPP_