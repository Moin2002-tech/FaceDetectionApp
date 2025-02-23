#pragma once

/*
* SoftwareRenderer.hpp
* Created on: 28/8/2024
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTION_RENDER_SOFTWARERENDERER_HPP_
#define FEATUREDETECTION_RENDER_SOFTWARERENDERER_HPP_
#include"MatrixUtils.hpp"
#include"Mesh.hpp"
#include"Triangle.hpp"

#include<opencv2/core.hpp>
#include<boost/optional.hpp>

#ifdef WITH_RENDER_QOPENGL
#include <QMatrix4x4>
#endif

namespace Render 
{

	/**
 * Desc
 * Coordinate systems:
 * When specifying the vertices: +x = right, +y = up, we look into -z.
 * So z = 0.5 is in front of 0.0.
 * Z-Buffer:
 *
 */

	class SoftwareRenderer {
	public:
		SoftwareRenderer(unsigned int viewPortWidth, unsigned int viewPortHeight);
		bool backFaceculling;
		bool doTexture;
		
		#ifdef WITH_RENDER_QOPENGLd
		std::pair<cv::Mat, cv::Mat> render(Mesh mesh, QMatrix4x4 mvp);
		#endif
		// Note: returns a reference (Mat) to the framebuffer, not
		// a clone! I.e. if you don't want your image to get
		// overwritten by a second call to render(...), you have to
		// clone.
		std::pair<cv::Mat, cv::Mat> render(Mesh mesh, cv::Mat mvp);
		cv::Vec3f projectVertex(cv::Vec4f vertex, cv::Mat mvp);

		void enableTexturing(bool doTexture) {
			this->doTexture = doTexture;
		};

		void setCurrentTexture(std::shared_ptr<Texture> texture) {
				currentTexture = texture;
		};




	private:
		cv::Mat colorBuffer;
		cv::Mat depthBuffer;
		unsigned int viewportWidth = 640;
		unsigned int viewportHeight = 480;
		float aspect;


		std::shared_ptr<Texture> currentTexture;
		std::shared_ptr<Texture> mipmaps;

		float dudx, dudy, dvdx, dvdy; // partial derivatives of U/V coordinates with respect to X/Y pixel's screen coordinates

		// Todo: Split this function into the general (core-part) and the texturing part.
		// Then, utils::extractTexture can re-use the core-part.
		boost::optional<TriangleToRasterize> processProspectiveTri(Vertex v0, Vertex v1, Vertex v2);

		void rasterTriangle(TriangleToRasterize triangle);

		std::vector<Vertex> clipPolygonToPlaneIn4D(const std::vector<Vertex>& vertices, const cv::Vec4f& planeNormal);

		cv::Vec3f tex2D(const cv::Vec2f& texCoord);

		cv::Vec3f tex2D_linear_mipmap_linear(const cv::Vec2f& texCoord);

		cv::Vec2f texCoord_wrap(const cv::Vec2f& texCoord);

		cv::Vec3f tex2D_linear(const cv::Vec2f& imageTexCoord, unsigned char mipmapIndex);

		float clamp(float x, float a, float b); // Todo: Document! x, a, b?
	
	};
}

#endif //FEATUREDETECTION_RENDER_SOFTWARERENDERER_HPP_
