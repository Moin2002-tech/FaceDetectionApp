#pragma once
/*
 * FaceTrackingApp.hpp
 *
 *  Created on: 18/2/2025
 *      Author: Moin Shaikh
*/

#ifndef FEATUREDETECTION_FACETRACKINGAPP_HPP_
#define FEATUREDETECTION_FACETRACKINGAPP_HPP_


#include"../ImageIO/ImageSource.hpp"
#include"../ImageIO/ImageSink.hpp"
#include"../Condensation/CondensationTracker.hpp"
#include"../Condensation/MeasurementModel.hpp"
#include "../Condensation/SimpleTransitionModel.hpp"
#include "../Condensation/ResamplingSampler.hpp"
#include "../Condensation/GridSampler.hpp"

#include<opencv2/highgui.hpp>
#include<memory>
#include<string>


using namespace imageio;
using namespace Condensation;




#endif /* FEATUREDETECTION_FACETRACKINGAPP_HPP_ */
