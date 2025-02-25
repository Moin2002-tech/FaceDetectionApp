/*
* FilteringClassifierExtractor.hpp
* Created on: 20/2/2025
* Author : Moin Shaikh
*/

#include "FilteringClassifierModel.hpp"
#include "SingleClassifierModel.hpp"
#include "Sample.hpp"
#include "../ImageProcessing/Patch.hpp"
#include "../ImageProcessing/FeatureExtractor.hpp"
#include "../Classification/BinaryClassifier.hpp"


using ImageProcessing::Patch;
using ImageProcessing::VersionedImage;
using ImageProcessing::FeatureExtractor;
using Classification::BinaryClassifier;
using Classification::ProbabilisticClassifier;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unordered_map;


namespace Condensation
{
	FilteringClassifierModel::FilteringClassifierModel(
		shared_ptr<FeatureExtractor> filterFeatureExtractor, shared_ptr<BinaryClassifier> filter,
		shared_ptr<FeatureExtractor> featureExtractor, shared_ptr<ProbabilisticClassifier> classifier, Behavior behavior) :
		behavior(behavior),
		measurementModel(make_shared<SingleClassifierModel>(featureExtractor, classifier)),
		featureExtractor(filterFeatureExtractor),
		filter(filter),
		cache() 
	{

	}

	FilteringClassifierModel::FilteringClassifierModel(
		shared_ptr<FeatureExtractor> filterFeatureExtractor, shared_ptr<BinaryClassifier> filter,
		shared_ptr<MeasurementModel> measurementModel, Behavior behavior) :
		behavior(behavior),
		measurementModel(measurementModel),
		featureExtractor(filterFeatureExtractor),
		filter(filter),
		cache()
	{

	}

	FilteringClassifierModel::~FilteringClassifierModel()
	{

	}

	void FilteringClassifierModel::update(shared_ptr<VersionedImage> image)
	{
		cache.clear();
		featureExtractor->update(image);
		measurementModel->update(image);
	}

	void FilteringClassifierModel::evaluate(Sample& sample) const
	{
		if (behavior == Behavior::RESET_WEIGHT)
		{
			if (passesFilter(sample))
			{
				measurementModel->evaluate(sample);
			}
			else 
			{
				sample.setTarget(false);
				sample.setWeight(0);
			}
		}
		else
		{ // behavior == Behavior::KEEP_WEIGHT
			measurementModel->evaluate(sample);
			if (sample.isTarget() && !passesFilter(sample))
			{
				sample.setTarget(false);
			}
		}
	}

	void FilteringClassifierModel::evaluate(shared_ptr<VersionedImage> image, vector<shared_ptr<Sample>>& samples) 
	{
		if (behavior == Behavior::RESET_WEIGHT)
		{
			update(image);
			for (shared_ptr<Sample> sample : samples) 
			{
				if (passesFilter(*sample)) 
				{
					measurementModel->evaluate(*sample);
				}
				else 
				{
					sample->setTarget(false);
					sample->setWeight(0);
				}
			}
		}
		else
		{ // behavior == Behavior::KEEP_WEIGHT
			cache.clear();
			featureExtractor->update(image);
			measurementModel->evaluate(image, samples);
			for (shared_ptr<Sample> sample : samples) 
			{
				if (sample->isTarget() && !passesFilter(*sample))
					sample->setTarget(false);
			}
		}
	}

	bool FilteringClassifierModel::passesFilter(const Sample& sample) const
	{
		shared_ptr<Patch> patch = featureExtractor->extract(sample.getX(), sample.getY(), sample.getWidth(), sample.getHeight());
		return patch && passesFilter(patch);
	}

	bool FilteringClassifierModel::passesFilter(const shared_ptr<Patch> patch) const
	{
		auto resIt = cache.find(patch);
		if (resIt == cache.end()) 
		{
			bool result = filter->classify(patch->getData());
			cache.emplace(patch, result);
			return result;
		}
		return resIt->second;
	}
}