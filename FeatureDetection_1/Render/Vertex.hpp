#pragma once


/*
* Vertex.hpp
* Created on: 28/8/2024
* aurthor : Moin Shaikh
*/


#ifndef FEATUREDETECTION_RENDER_VERTEX_HPP_
#define FEATUREDETECTION_RENDER_VERTEX_HPP_

#include<opencv2/core.hpp>

namespace Render 
{
	class Vertex 
	{
	public:
		Vertex();
		Vertex(const cv::Vec4f& position, const cv::Vec3f& color, const cv::Vec2f& TexCoord);

		cv::Vec4f position;	///< doc.
		cv::Vec3f color;	///< RGB-format
		cv::Vec2f texcrd;	///< doc.

	};
	
}



#endif //FEATUREDETECTION_RENDER_VERTEX_HPP_