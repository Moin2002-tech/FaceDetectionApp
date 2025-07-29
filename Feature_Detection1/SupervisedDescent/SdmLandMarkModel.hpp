#pragma once

/*
* File: SdmLandMarkModel.hpp
* Created on: 17/7/2025
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_SUPERVISEDDESCENT_SDMLANDMARKMODEL_HPP
#define FEATUREDETECTION_SUPERVISEDDESCENT_SDMLANDMARKMODEL_HPP


#include"DiscriptorExtractor.hpp"
#include"../ImageIO/LandmarkCollection.hpp"

#include<opencv2/core.hpp>
#include<opencv2/features2d.hpp>
#include<opencv2/imgproc.hpp>

#include<boost/lexical_cast.hpp>
#include<boost/algorithm/string.hpp>
#include<boost/filesystem/path.hpp>

extern "C" 
{
#include"hog.h"
}

using cv::Mat;
using cv::Scalar;
using std::vector;
using std::string;
using boost::filesystem::path;
using boost::lexical_cast;

namespace superVisedDescent
{
	/**
	* A class representing a landmark model trained
	* with the supervised descent method.
	*
	* Todo: Write something about how the landmarks are represented?
	*/

	struct HogParameter
	{
		int cellSize;
		int numBins;
	};
	class SdmLandmarkModel
	{
	public:
		// should consider deleting this
		SdmLandmarkModel();


		/**
		* Constructs a new SdmModel.
		*
		* @param[in] a b
		*/
		SdmLandmarkModel(cv::Mat& meanLandmark,
			std::vector<std::string>& landmarkIdentifier,
			std::vector<cv::Mat>& regressorData,
			std::vector<std::shared_ptr<DescriptorExtractor>>& descriptorExtractor,
			std::vector<std::string>& descriptorTypes);

		int getNumLandmarks() const;

		int getNumCascadeSteps() const;

		HogParameter getHogParameters(int cascadeLevel) 
		{
			return hogParameters[cascadeLevel];
		}

		/**
		* Returns the mean of the shape- and color model
		* as a Mesh.
		*
		* @return The mean of the model.
		*/
		// returns a copy. col-vec. ext always col, internal row

		cv::Mat getMeanShape() const;


		// returns  a header that points to the original data
		cv::Mat getRegressorData(int cascadeLevel);

		std::shared_ptr<DescriptorExtractor> getDescriptorExtractor(int cascadeLevel);

		std::string getDescriptorType(int cascadeLevel);

		std::vector<cv::Point2f> getMeanAsPoints() const;

		/**
		 * Get the model's points as a LandmarkCollection.
		 * If nothing passed, return the mean.
		 *
		 * @param[in] modelInstance Todo.
		 * @return Todo.
		*/
		imageio::LandmarkCollection getAsLandmarks(cv::Mat modelInstance = cv::Mat()) const;

		cv::Point2f getLandmarkAsPoint(std::string& landmarks, cv::Mat modelInstance = cv::Mat()) const;

		void save(boost::filesystem::path &filename, std::string comment = "");

		/**
		* Load a SdmLandmarkModel model TODO a property tree node in a config file.
		* The function uses the file extension to determine which load
		* Throws a std::... ....
		*
		* @param[in] configTree A node of a ptree.
		* @return A morphable model.
		*/

		static SdmLandmarkModel load(boost::filesystem::path &filename);


	private:
		cv::Mat meanLandmarks; // 1 x numLandmarks*2. First all the x-coordinates, then all the y-coordinates.
		std::vector<std::string> landmarkIdentifier; //
		std::vector<cv::Mat> regressorData; // Holds the training data, one cv::Mat for each cascade level. Every Mat is (numFeatureDim+1) x numLandmarks*2 (for x & y)

		std::vector<HogParameter> hogParameters;
		std::vector<std::shared_ptr<DescriptorExtractor>> descriptorExtractors;
		std::vector<std::string> descriptorTypes; //
	};



	class SdmLandmarkFitting 
	{
	private:
		SdmLandmarkModel model;
	public:
		SdmLandmarkFitting(SdmLandmarkModel model) : model(model)
		{

		}


		// out: aligned modelShape
		// in: Rect, ocv with tl x, tl y, w, h (?) and calcs center
		// directly modifies modelShape
		// could move to parent-class
		// assumes mean -0.5, 0.5 and just places inside FB

		cv::Mat alignRigid(cv::Mat &modelShape,cv::Rect &faceBox) const
		{
			if(modelShape.cols != 1)
			{
				throw std::runtime_error("The supplied model shape does not have one column (i.e. it doesn't seem to be a column-vector).");
			}
			int startRow = 0;
			int startCol = 0;
			cv::Mat xCoords = modelShape.rowRange(startRow, modelShape.rows / 2);
			cv::Mat yCoords = modelShape.rowRange(modelShape.rows / 2, modelShape.rows);
			
			xCoords = (xCoords + 0.5f) * faceBox.width + faceBox.x;
			yCoords = (yCoords + 0.5f) * faceBox.height + faceBox.y;

			return modelShape;
		}
		
		cv::Mat optimize(cv::Mat &modelShape, cv::Mat &image)
		{

			for(int cascadeStep = 0; cascadeStep < model.getNumCascadeSteps();++cascadeStep)
			{
				vector<cv::Point2f> points;
				for (int i = 0; i < model.getNumLandmarks(); ++i) 
				{ // in case of HOG, need integers?
					points.emplace_back(cv::Point2f(modelShape.at<float>(i),
						modelShape.at<float>(i + model.getNumLandmarks()))
					);
				}

				Mat currentFeatures;
				float dynamicFaceSizeDistance = 0.0f;

				if(true)
				{
					cv::Vec2f point1(modelShape.at<float> (8), modelShape.at<float>(8 + model.getNumLandmarks()));
					cv::Vec2f point2(modelShape.at<float>(9), modelShape.at<float>(9 + model.getNumLandmarks()));
					cv::Vec2f anchor1 = (point1 + point2) / 2.0f;
					cv::Vec2f point3(modelShape.at<float>(11), modelShape.at<float>(11 + model.getNumLandmarks()));
					cv::Vec2f point4(modelShape.at<float>(12), modelShape.at<float>(12 + model.getNumLandmarks()));
					cv::Vec2f anchor2 = (point3 + point4) / 2.0f;

					dynamicFaceSizeDistance = cv::norm(anchor1- anchor2);
					float windowSize = dynamicFaceSizeDistance / 2.0f;
					float windowHalfSize = windowSize / 2;
					windowHalfSize = std::round(windowHalfSize * (1 / (1 + std::exp((cascadeStep + 1) - model.getNumCascadeSteps())))); // this is (step - numStages), numStages is 5 and step goes from 1 to 5. Because our step goes from 0 to 4, we add 1.
					
					int NUM_CELL = 3;
					int windowSizeHalfi = static_cast<int>(windowHalfSize) + NUM_CELL - (static_cast<int>(windowHalfSize) % NUM_CELL);

					currentFeatures = model.getDescriptorExtractor(cascadeStep)->getDescriptor(image, points, windowSizeHalfi);

				}
				else 
				{
					currentFeatures = model.getDescriptorExtractor(cascadeStep)->getDescriptor(image, points);
				}
				currentFeatures = currentFeatures.reshape(0, currentFeatures.cols * model.getNumLandmarks()).t();
				Mat regressorData = model.getRegressorData(cascadeStep);
				//Mat deltaShape = regressorData.rowRange(0, regressorData.rows - 1).t() * currentFeatures + regressorData.row(regressorData.rows - 1).t();
				Mat deltaShape = currentFeatures * regressorData.rowRange(0, regressorData.rows - 1) + regressorData.row(regressorData.rows - 1);
				if (true) 
				{ // adaptive
					modelShape = modelShape + deltaShape.t() * dynamicFaceSizeDistance;
				}
				else
				{
					modelShape = modelShape + deltaShape.t();
				}

			}
			return modelShape;
		}

	};
}

#endif