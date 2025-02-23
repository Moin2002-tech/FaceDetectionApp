/*
* FramebasedExampleManagement.hpp
* Created on : 28/ 11 / 2024
* Author : Moin Shaikh
*/

#include"FrameBasedExampleManagement.hpp"

using cv::Mat;
using std::vector;
using std::unique_ptr;

namespace Classification
{
	FrameBasedExampleManagement::FrameBasedExampleManagement(size_t frameCapacity, size_t requiredSize) :
		examples(frameCapacity), oldestEntry(0), requiredSize(requiredSize) {}


	void FrameBasedExampleManagement::add(const vector<cv::Mat>& newExample)
	{
		examples.at(oldestEntry).clear();
		for (const Mat& example : newExample)
		{
			examples.at(oldestEntry).emplace_back(example);
		}
		++oldestEntry;
		if (oldestEntry >= examples.size())
		{
			oldestEntry = 0;
		}
	}
	void FrameBasedExampleManagement::clear()
	{
		for (vector<Mat>& frame : examples)
		{
			frame.clear();
			oldestEntry = 0;
		}
	}
	size_t FrameBasedExampleManagement::size() const
	{
		size_t count = 0;

		for(const vector<Mat> &frames: examples)
		{
			count += frames.size();
		}
		return count;
	}

	bool FrameBasedExampleManagement::hasRequiredSize() const
	{
		return size() >= requiredSize;
	}
	unique_ptr<ExampleManagement::ExampleIterator> FrameBasedExampleManagement::iterator() const
	{
		return unique_ptr<FrameIterator>(new FrameIterator(examples));
	}
	FrameBasedExampleManagement::FrameIterator::FrameIterator(const vector<vector<Mat>>& examples) :

		currentFrame(examples.cbegin()), endFrame(examples.cend()), current(currentFrame->begin()), end(currentFrame->end()) 
	{

	}
	bool FrameBasedExampleManagement::FrameIterator::hasNext() const 
	{
		return current != end || currentFrame != endFrame;
	}

	const Mat& FrameBasedExampleManagement::FrameIterator::next()
	{
		if(current == end)
		{
			++currentFrame;
			current = currentFrame->begin();
			end = currentFrame->end();
		}
		const Mat& example = *current;
		++current;
		return example;
	}

}