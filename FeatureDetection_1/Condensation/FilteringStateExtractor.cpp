/*
* FilteringStateExtractor.hpp
* Created on: 20/2/2025
* Author : Moin Shaikh
*/


#include"FilteringStateExtractor.hpp"
#include"Sample.hpp"

using std::vector;
using std::shared_ptr;

namespace Condensation
{
	FilteringStateExtractor::FilteringStateExtractor(shared_ptr<StateExtractor> extractor) : extractor(extractor) {}

	shared_ptr<Sample> FilteringStateExtractor::extract(const vector<shared_ptr<Sample>>& samples)
	{
		vector<shared_ptr<Sample>> objects;
		for (const shared_ptr<Sample>& sample : samples) {
			if (sample->isTarget())
			{
				objects.push_back(sample);
			}
		}
		return extractor->extract(objects);
	}
}