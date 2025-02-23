#pragma once


/*
* Hdf5Utils.hpp
* Created on: 4/9/2024
* Author : Moin Shaikh
*/




#ifndef FEATUREDETECTION_MORPHOBLE_MODEL_HDF5UTILS_HPP_
#define FEATUREDETECTION_MORPHOBLE_MODEL_HDF5UTILS_HPP_


#include"highfive/highfive.hpp"
#include<opencv2/core.hpp>
#include<vector>
#include"../Render/Mesh.hpp"

namespace MorphableModels 
{
	namespace utils 
	{ 
		class Hdf5Utils 
		{ 
		public:
			
			static HighFive::File openFile(const std::string& filename);

			static HighFive::Group openPath(HighFive::File& file, const std::string& path);

			static cv::Mat readMatrixFloat(const HighFive::Group& group, const std::string& name);

			static void readMatrixInt(const HighFive::Group& group, const std::string& name, cv::Mat& matrix);

			static void readVector(const HighFive::Group& group, const std::string& name, std::vector<float>& vector);

			static std::string readString(const HighFive::Group& group, const std::string& name); 

			static bool existsObjectWithName(const HighFive::Group& group, const std::string& name); 

			static Render::Mesh readFromHdf5(const std::string& filename);
		}; 
		
	}
}

#endif //FEATUREDETECTION_MORPHOBLE_MODEL_HDF5UTILS_HPP_