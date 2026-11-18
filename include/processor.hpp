#pragma once

#include <opencv2/core.hpp>

enum class Filter
{
    Gaussian,
    Sobel
};

void processSequential(
    const cv::Mat &input,
    cv::Mat &output,
    Filter filter);

void processParallel(
    const cv::Mat &input,
    cv::Mat &output,
    Filter filter,
    int numThreads);
