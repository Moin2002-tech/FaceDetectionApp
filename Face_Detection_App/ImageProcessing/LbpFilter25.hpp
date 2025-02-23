#pragma once
/*
* File: LbpFilter.hpp
* Created on: 10/2/2025
* Author : Moin Shaikh
*/

#ifndef FEATUREDETECTON_IMAGEPROCESSING_LBPFILTER_HPP_
#define FEATUREDETECTON_IMAGEPROCESSING_LBPFILTER_HPP_

#include "BiningFilter.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <array>
#include <stdexcept>

namespace ImageProcessing
{

    class LbpFilter : public BinningFilter 
    {
    public:

        enum class Type {
            LBP8,
            LBP8_UNIFORM,
            LBP4,
            LBP4_ROTATED
        };

        explicit LbpFilter(Type type = Type::LBP8);

        using ImageFilter::applyTo;

        cv::Mat applyTo(const cv::Mat& image, cv::Mat& filtered) const;

        void applyInPlace(cv::Mat& image) const;

        unsigned int getBinCount() const;

    private:

        bool isUniform(uchar code) const;

        template <typename T>
        cv::Mat createLbpFilter(const cv::Mat& image) const;

        Type type;
        std::array<uchar, 256> map;
    };
}
#endif