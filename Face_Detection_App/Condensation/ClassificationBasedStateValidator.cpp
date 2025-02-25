

#include "ClassificationBasedStateValidator.hpp"
#include "Sample.hpp"
#include "../ImageProcessing/Patch.hpp"
#include "../ImageProcessing/VersionedImage.hpp"
#include "../ImageProcessing/FeatureExtractor.hpp"
#include "../Classification/BinaryClassifier.hpp"


using ImageProcessing::Patch;
using ImageProcessing::VersionedImage;
using ImageProcessing::FeatureExtractor;
using Classification::BinaryClassifier;
using std::vector;
using std::shared_ptr;
namespace Condensation
{
	ClassificationBasedStateValidator::ClassificationBasedStateValidator(
		shared_ptr<FeatureExtractor> extractor, shared_ptr<BinaryClassifier> classifier,
		vector<double> sizes,
		vector<double> displacements) :
		extractor(extractor), classifier(classifier), sizes(sizes), displacements(displacements) 
	{

	}

	bool ClassificationBasedStateValidator::isValid(const Sample& target,
		const vector<shared_ptr<Sample>>& samples, shared_ptr<VersionedImage> image) 
	{
		extractor->update(image);
		for (double scale : sizes) 
		{
			int size = static_cast<int>(std::round(scale * target.getSize()));
			for (double offsetX : displacements) 
			{
				int x = target.getX() + static_cast<int>(std::round(offsetX * size));
				for (double offsetY : displacements) 
				{
					int y = target.getY() + static_cast<int>(std::round(offsetY * size));
					Sample s(x, y, size);
					shared_ptr<Patch> patch = extractor->extract(s.getX(), s.getY(), s.getWidth(), s.getHeight());
					if (patch && classifier->classify(patch->getData()))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
}