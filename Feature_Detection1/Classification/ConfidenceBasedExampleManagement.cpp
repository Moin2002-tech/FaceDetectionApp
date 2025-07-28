/*
* ConfidenceBasedExampleManagement.hpp
* Created on : 15/ 11 / 2024
* Author : Moin Shaikh
*/


#include"ConfidenceBasedExampleManagement.hpp"
#include"BinaryClassifier.hpp"
#include<algorithm>

using cv::Mat;
using std::pair;
using std::vector;
using std::make_pair;
using std::shared_ptr;
using std::sort;


namespace Classification
{
	ConfidenceBasedExampleManagement::ConfidenceBasedExampleManagement(const shared_ptr<BinaryClassifier>& classifier,bool positive ,size_t capacity, size_t requiredSize) :
			VectorBasedExampleManagement(capacity,requiredSize), classifier(classifier), positive(positive), keep(1){}
	void ConfidenceBasedExampleManagement::setFirstExamplesToKeep(size_t keep)
	{
		this->keep = keep;
	}
	void ConfidenceBasedExampleManagement::add(const vector<Mat>& newExample)
	{
		// compute confidences of existing and new training examples and sort
		vector<pair<size_t, double>> existingConfidences;
		existingConfidences.reserve(examples.size());
		for(size_t i = keep;i < examples.size(); ++i)
		{
			pair<bool, double> result = classifier->getConfidence(examples[i]);
			double score = result.second;

			if(positive^result.first)
			{
				score = -score;
			}
			existingConfidences.emplace_back(make_pair(i, score));
		}
		vector<pair<size_t, double>> newConfidences;

		newConfidences.reserve(newExample.size());

		for (size_t i = 0; i < newExample.size(); ++i) 
		{
			pair<bool, double> result = classifier->getConfidence(newExample[i]);

			double score = result.second;

			if (positive ^ result.first)
			{
				score = -score;
			}
				
			newConfidences.emplace_back(make_pair(i, score));
		}

		std::sort(existingConfidences.begin(), existingConfidences.end(), [](pair<size_t, double> a, pair<size_t, double>b)
		{
				return a.second > b.second; //// descending order (high confidence first)
		}
		);

		sort(newConfidences.begin(), newConfidences.end(), [](pair<size_t, double> a, pair<size_t, double> b) {
			return a.second < b.second; // ascending order (low confidence first)
		});
		
		// add new examples until there is no more space, then replace existing examples that have higher confidence than new examples
		auto newConfidence = newConfidences.cbegin();
		while(examples.size() < examples.capacity() && newConfidence != newConfidences.cend())
		{
			examples.emplace_back(newExample[newConfidence->first]);
			++newConfidence;
		}
		auto existingConfidence = existingConfidences.cbegin();
		while (existingConfidence != existingConfidences.cend()
			&& newConfidence != newConfidences.cend()
			&& newConfidence->second < existingConfidence->second) {
			examples[existingConfidence->first] = newExample[newConfidence->first];
			++existingConfidence;
			++newConfidence;
		}
	}

}