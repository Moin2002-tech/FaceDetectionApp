#pragma once


/*
* PartiallyAdaptiveTracking.hpp
* Created on: 21/2/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_PARTIALLYADAPTIVETRACKING_HPP_
#define FEATUREDETECTION_PARTIALLYADAPTIVETRACKING_HPP_


#include "../ImageIO/ImageSource.hpp"
#include "../ImageIO/ImageSink.hpp"
#include "../Classification/Kernel.hpp"
#include "../Classification/TrainableSvmClassifier.hpp"
#include "../Classification/TrainableProbabilisticClassifier.hpp"
#include "../Condensation/PartiallyAdaptiveCondensationTracker.hpp"
#include "../Condensation/AdaptiveMeasurementModel.hpp"
#include "../Condensation/MeasurementModel.hpp"
#include "../Condensation/SimpleTransitionModel.hpp"
#include "../Condensation/ResamplingSampler.hpp"
#include "../Condensation/GridSampler.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "boost/property_tree/ptree.hpp"
#include <memory>
#include <string>


using namespace imageio;
using namespace Condensation;
using namespace Classification;
using cv::Mat;
using boost::property_tree::ptree;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;



class PartiallyAdaptiveTracking
{
public:
	PartiallyAdaptiveTracking(unique_ptr<ImageSource> imageSource, unique_ptr<ImageSink> imageSink, ptree config);
	virtual ~PartiallyAdaptiveTracking();

	void run();
	void stop();

	
private:

	static void adaptiveChanged(int state, void* userData);
	static void samplerChanged(int state, void* userData);
	static void sampleCountChanged(int state, void* userdata);
	static void randomRateChanged(int state, void* userdata);
	static void positionDeviationChanged(int state, void* userdata);
	static void sizeDeviationChanged(int state, void* userdata);
	static void drawSamplesChanged(int state, void* userdata);


	shared_ptr<kernel> createKernel(ptree config);

	shared_ptr<TrainableSvmClassifier> createTrainableSvm(shared_ptr<kernel> kernel, ptree config);
	shared_ptr<TrainableProbabilisticClassifier> createClassifier(shared_ptr<TrainableSvmClassifier> trainableSvm, ptree config);

	void initTracking(ptree config);
	void initGui();
	void drawDebug(Mat& image);

	static const string videoWindowName;
	static const string controlWindowName;

	unique_ptr<ImageSource> imageSource;
	unique_ptr<ImageSink> imageSink;

	bool running;
	bool paused;
	bool drawSamples;

	unique_ptr<PartiallyAdaptiveCondensationTracker> tracker;
	shared_ptr<MeasurementModel> staticMeasurementModel;
	shared_ptr<AdaptiveMeasurementModel> adaptiveMeasurementModel;
	shared_ptr<SimpleTransitionModel> transitionModel;
	shared_ptr<ResamplingSampler> resamplingSampler;
	shared_ptr<GridSampler> gridSampler;
};

#endif //FEATUREDETECTION_PARTIALLYADAPTIVETRACKING_HPP_