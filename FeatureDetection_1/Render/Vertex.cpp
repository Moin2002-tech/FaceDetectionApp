/*
* Vertex.cpp
* Created on: 4/9/2024
* author : Moin Shaikh
*/

#include"Vertex.hpp"

namespace Render 
{
	Vertex::Vertex() 
	{

	}

	Vertex::Vertex(const cv::Vec4f& position, const cv::Vec3f& color, const cv::Vec2f& texCoord)
	{
		this->position = position;
		this->color = color;
		this->texcrd = texCoord;
	}

}
