#pragma once
/*
* MatrixUtils.hpp
* Created on: 28/8/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_RENDER_MATRIXUDTILS_HPP_
#define FEATUREDETECTION_RENDER_MATRIXUDTILS_HPP_

#include<opencv2/core.hpp>

namespace Render
{

	namespace utils
	{
		class MatrixUtils
		{
		public:
			static cv::Mat createRotationMatrix_X(float Angle);
			static cv::Mat CreateRotationMatrix_Y(float Angle);
			static cv::Mat createRotationMatrixZ(float angle);
			static cv::Mat createScalingMatrix(float sx, float sy, float sz);
			static cv::Mat createTranslationMatrix(float tx, float ty, float tz);
			static cv::Mat createOrthogonalProjectionMatrix(float l, float r, float b, float t, float n, float f);
			static cv::Mat createPerspectiveProjectionMatrix(float l, float r, float b, float t, float n, float f);
			static cv::Mat createPerspectiveProjectionMatrix(float verticalAngle, float aspectRatio, float nearPlane, float farPlane);


	
		};
		unsigned int getMaxPossibleMipmapsNum(unsigned int width, unsigned int height);	// TODO: This belongs more in a ImageUtils, TextureUtils, or whatever... => render::utils::texturing
	}

}


#endif // FEATUREDETECTION_RENDER_MATRIXUDTILS_HPP_