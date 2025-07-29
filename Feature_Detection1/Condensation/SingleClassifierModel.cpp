

#include "SingleClassifierModel.hpp"
#include "Sample.hpp"
#include "../ImageProcessing/Patch.hpp"
#include "../ImageProcessing/FeatureExtractor.hpp"
#include "../Classification/ProbabilisticClassifier.hpp"


using ImageProcessing::Patch;
using ImageProcessing::VersionedImage;
using ImageProcessing::FeatureExtractor;
using Classification::ProbabilisticClassifier;
using std::pair;
using std::shared_ptr;


namespace Condensation
{
	SingleClassifierModel::SingleClassifierModel(shared_ptr<FeatureExtractor> featureExtractor,
		shared_ptr<ProbabilisticClassifier> classifier) :
		featureExtractor(featureExtractor), classifier(classifier), cache() 
	{
	}

	void SingleClassifierModel::update(shared_ptr<VersionedImage> image) 
	{
		cache.clear();
		featureExtractor->update(image);
	}

	void SingleClassifierModel::evaluate(Sample& sample) const
	{
		shared_ptr<Patch> patch = featureExtractor->extract(sample.getX(), sample.getY(), sample.getWidth(), sample.getHeight());
		if (patch)
		{
			pair<bool, double> result = classify(patch);
			sample.setTarget(result.first);
			sample.setWeight(result.second);
		}
		else 
		{
			sample.setTarget(false);
			sample.setWeight(0);
		}
	}

	pair<bool, double> SingleClassifierModel::classify(shared_ptr<Patch> patch) const 
	{
		auto resIt = cache.find(patch);
		if (resIt == cache.end()) 
		{
			pair<bool, double> result = classifier->getProbability(patch->getData());
			cache.emplace(patch, result);
			return result;
		}
		return resIt->second;
	}
}