#pragma once
/*
* Utils.hpp
* Created on: 28/8/2024
* Author : Moin Shaikh
*/


#ifndef FEATUREDETECTION_RENDER_MESHUTIL_HPP_
#define FEATUREDETECTION_RENDER_MESHUTIL_HPP_

#include"Mesh.hpp"
#include<memory>
#include<opencv2/core.hpp>

namespace Render{


	namespace util {

		class MeshUtils
		{
		public:
			static Mesh createCube();
			static Mesh createPlane();
			static Mesh createPyramid();
			static std::shared_ptr<Mesh> createTriangle();

			static cv::Mat drawTexCoords(Mesh);

			static bool isPointInTriangle(cv::Point2f point, cv::Point2f triV0, cv::Point2f triV1, cv::Point2f triV2);
		};

		cv::Mat extractTexture(Mesh mesh, cv::Mat mvpMatrix, int viewportWidth, int viewportHeight, cv::Mat image, cv::Mat depthBuffer);

	} /* namespace utils */

}

#endif