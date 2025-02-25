#include "PartiallyAdaptiveCondensationTracker.hpp"
#include "Sample.hpp"
#include "Sampler.hpp"
#include "MeasurementModel.hpp"
#include "AdaptiveMeasurementModel.hpp"
#include "StateExtractor.hpp"
#include "../ImageProcessing/VersionedImage.hpp"




using ImageProcessing::VersionedImage;
using cv::Mat;
using cv::Rect;
using boost::optional;
using std::vector;
using std::shared_ptr;
using std::make_shared;


namespace Condensation
{
	PartiallyAdaptiveCondensationTracker::PartiallyAdaptiveCondensationTracker(shared_ptr<Sampler> sampler,
		shared_ptr<MeasurementModel> initialMeasurementModel, shared_ptr<AdaptiveMeasurementModel> measurementModel,
		shared_ptr<StateExtractor> extractor) :
		samples(),
		oldSamples(),
		state(),
		useAdaptiveModel(true),
		usedAdaptiveModel(false),
		image(make_shared<VersionedImage>()),
		sampler(sampler),
		initialMeasurementModel(initialMeasurementModel),
		measurementModel(measurementModel),
		extractor(extractor) {
	}

	optional<Rect> PartiallyAdaptiveCondensationTracker::process(const Mat& imageData) {
		image->setData(imageData);
		samples.swap(oldSamples);
		samples.clear();
		sampler->sample(oldSamples, samples, image->getData(), state);
		// evaluate samples and extract position
		if (useAdaptiveModel && measurementModel->isUsable()) {
			measurementModel->evaluate(image, samples);
			usedAdaptiveModel = true;
		}
		else {
			initialMeasurementModel->evaluate(image, samples);
			usedAdaptiveModel = false;
		}
		state = extractor->extract(samples);
		// update model
		if (useAdaptiveModel) {
			if (state)
				measurementModel->adapt(image, samples, *state);
			else
				measurementModel->adapt(image, samples);
		}
		// return position
		if (state)
			return optional<Rect>(state->getboundingBox());
		return optional<Rect>();
	}

	void PartiallyAdaptiveCondensationTracker::setUseAdaptiveModel(bool useAdaptive) {
		useAdaptiveModel = useAdaptive;
		if (!useAdaptive)
			measurementModel->reset();
	}

}