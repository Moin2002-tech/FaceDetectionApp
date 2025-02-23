#pragma once
/*
* Mesh.hpp
* Created on: 28/8/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_RENDER_MESH_HPP_
#define FEATUREDETECTION_RENDER_MESH_HPP_


#include"Vertex.hpp"
#include"Texture.hpp"
#include"Triangle.hpp"


#include<vector>
#include<array>
#include<memory>
#include<string>

using std::string;
using std::shared_ptr;
using std::vector;
using std::array;

namespace Render
{

	class Mesh
	{
	public:
		vector<Vertex> vertex;
		// To not have to copy the vertices, for OpenGL, the option below would be better:
		// But what about the whole pipeline (e.g. SW-Rend.), is it beneficial having a Vertex (& Triangle)-class?
		//std::vector<cv::Vec3f> vertex;	// g: cv::Vec3f
		//std::vector<cv::Vec3f> normal;	// g: cv::Vec3f
		//std::vector<cv::Vec3f> texcrd;	// g: cv::Vec3f, texture coordinates uvw
		//std::vector<cv::Vec4f> color;	// g: cv::Vec4f, RGBA
		//material
		vector<std::array<int, 3>> tvi;
		// std::tuple<int, int, int> doesn't work. Use std::array<int, 3> or cv::Vec3i.
		//std::vector< std::array<int, 3> > tni;
		//std::vector< std::array<int, 3> > tti;	// triangle texcrd indices
		std::vector<std::array<int, 3>> tci;
		//tcm

		bool hasTexture = false;
		string textureName;

		shared_ptr<Texture> texture;

		// TODO Doc
		// obj with vertex-coloring (not officially supported but works eg in meshlab)
		static void writeObj(Mesh mesh, std::string filename);

	};




}

#endif //FEATUREDETECTION_RENDER_MESH_HPP_