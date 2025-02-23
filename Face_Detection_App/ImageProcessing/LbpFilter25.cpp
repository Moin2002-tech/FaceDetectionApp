/*
* File: LbpFilter.cpp
* Created on: 15/2/2025
* Author : Moin Shaikh
*/

#include "LbpFilter25.hpp"

using cv::Mat;
using std::invalid_argument;

namespace ImageProcessing {

    LbpFilter::LbpFilter(Type type) : type(type) {
        if (type == Type::LBP8_UNIFORM) {
            int nonUniformIndex = 0;
            int emptyIndex = 1;
            for (unsigned int i = 0; i < map.size(); ++i) {
                if (isUniform(static_cast<uchar>(i)))
                    map[i] = emptyIndex++;
                else
                    map[i] = nonUniformIndex;
            }
        }
    }

    bool LbpFilter::isUniform(uchar code) const {
        int transitions = 0;
        int previousBit = (code >> 7) & 1;
        for (int pos = 0; pos < 8; ++pos) {
            int currentBit = (code >> pos) & 1;
            if (previousBit != currentBit) {
                transitions++;
                previousBit = currentBit;
            }
        }
        return transitions <= 2;
    }

    unsigned int LbpFilter::getBinCount() const {
        switch (type) {
        case Type::LBP8: return 256;
        case Type::LBP8_UNIFORM: return 59;
        case Type::LBP4:
        case Type::LBP4_ROTATED: return 16;
        default: throw std::runtime_error("LbpFilter: invalid type (should never occur)");
        }
    }

    Mat LbpFilter::applyTo(const Mat& image, Mat& filtered) const {
        if (image.channels() > 1)
            throw invalid_argument("LbpFilter: the image must have exactly one channel");

        switch (type) {
        case Type::LBP8: filtered = createLbpFilter<uchar>(image); break;
        case Type::LBP8_UNIFORM: filtered = createLbpFilter<uchar>(image); break;
        case Type::LBP4: filtered = createLbpFilter<uchar>(image); break;
        case Type::LBP4_ROTATED: filtered = createLbpFilter<uchar>(image); break;
        }

        if (type == Type::LBP8_UNIFORM) {
            int rows = filtered.rows;
            int cols = filtered.cols;
            if (filtered.isContinuous()) {
                cols *= rows;
                rows = 1;
            }
            for (int row = 0; row < rows; ++row) {
                uchar* values = filtered.ptr<uchar>(0);
                for (int col = 0; col < cols; ++col)
                    values[col] = map[values[col]];
            }
        }
        return filtered;
    }

    void LbpFilter::applyInPlace(Mat& image) const
    {
        applyTo(image, image);
    }

    template <typename T>
    Mat LbpFilter::createLbpFilter(const Mat& image) const 
    {
        Mat lbpImage(image.size(), CV_8UC1);
        for (int y = 1; y < image.rows - 1; ++y) {
            for (int x = 1; x < image.cols - 1; ++x) {
                T center = image.at<T>(y, x);
                uchar code = 0;
                code |= (image.at<T>(y - 1, x - 1) > center) << 7;
                code |= (image.at<T>(y - 1, x) > center) << 6;
                code |= (image.at<T>(y - 1, x + 1) > center) << 5;
                code |= (image.at<T>(y, x + 1) > center) << 4;
                code |= (image.at<T>(y + 1, x + 1) > center) << 3;
                code |= (image.at<T>(y + 1, x) > center) << 2;
                code |= (image.at<T>(y + 1, x - 1) > center) << 1;
                code |= (image.at<T>(y, x - 1) > center) << 0;
                lbpImage.at<uchar>(y, x) = code;
            }
        }
        return lbpImage;
    }
}
