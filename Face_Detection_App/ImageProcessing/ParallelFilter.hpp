#pragma once
/*
* File: ParallelFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/
#ifndef FEATUREDETECTON_IMAGEPROCESSING_PARALLELFILTER_HPP_
#define FEATUREDETECTON_IMAGEPROCESSING_PARALLELFILTER_HPP_

#include"ImageFilter.hpp"
#include<memory>
#include<vector>

namespace ImageProcessing
{
	/**
	 * Image filter that applies several filters on the input image and combines the results by merging the result's channels
	 * into a single image. The size and depth of the filter results have to be equal.
	*/

	class ParallelFilter : public ImageFilter {
	public:

		/**
		 * Constructs a new parallel image filter without filters.
		 */
		ParallelFilter();

		/**
		 * Constructs a new parallel image filter.
		 *
		 * @param[in] filters The image filters whose results should be combined.
		 */
		explicit ParallelFilter(std::vector<std::shared_ptr<ImageFilter>> filters);

		/**
		 * Constructs a new parallel image filter that combines the results of two filters.
		 *
		 * @param[in] filter1 The first image filter.
		 * @param[in] filter2 The second image filter.
		 */
		ParallelFilter(std::shared_ptr<ImageFilter> filter1, std::shared_ptr<ImageFilter> filter2);

		/**
		 * Constructs a new parallel image filter that combines the results of three filters.
		 *
		 * @param[in] filter1 The first image filter.
		 * @param[in] filter2 The second image filter.
		 * @param[in] filter3 The third image filter.
		 */
		ParallelFilter(std::shared_ptr<ImageFilter> filter1, std::shared_ptr<ImageFilter> filter2, std::shared_ptr<ImageFilter> filter3);

		/**
		 * Adds a new filter whose result should be combined with other filters.
		 *
		 * @param[in] filter The new filter.
		 */
		void add(std::shared_ptr<ImageFilter> filter);

		using ImageFilter::applyTo;

		cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

	private:

		std::vector<std::shared_ptr<ImageFilter>> filters; ///< The image filters whose results should be combined.
	};

}


#endif //FEATUREDETECTON_IMAGEPROCESSING_PARALLELFILTER_HPP_