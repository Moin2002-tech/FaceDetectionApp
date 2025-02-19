/*
* Triangle.cpp
* Created on: 28/8/2024
* Author : Moin Shaikh
*/

#include "Triangle.hpp"

namespace Render {

	Triangle::Triangle(void)
	{
	}

	Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2)
	{
		vertex[0] = v0;
		vertex[1] = v1;
		vertex[2] = v2;
	}

	Triangle::~Triangle(void)
	{
	}

} /* namespace render */