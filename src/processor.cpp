#include "processor.hpp"

#include "gaussian.hpp"
#include "sobel.hpp"

#include <stdexcept>

namespace
{
void validateInput(const cv::Mat &input)
{
    if (input.empty())
    {
        throw std::invalid_argument("Input image must not be empty.");
    }

    if (input.type() != CV_8UC1)
    {
        throw std::invalid_argument("Input image must be an 8-bit grayscale image.");
    }
}

void validateFilter(Filter filter)
{
    if (filter != Filter::Gaussian && filter != Filter::Sobel)
    {
        throw std::invalid_argument("Unsupported image filter.");
    }
}

unsigned char processPixel(const cv::Mat &input, int y, int x, Filter filter)
{
    if (filter == Filter::Gaussian)
    {
        return gaussianPixel(input, y, x);
    }

    return sobelPixel(input, y, x);
}
}

void processSequential(
    const cv::Mat &input,
    cv::Mat &output,
    Filter filter)
{
    validateInput(input);
    validateFilter(filter);

    const cv::Mat source = input;
    output = source.clone();

    for (int y = 1; y < source.rows - 1; ++y)
    {
        for (int x = 1; x < source.cols - 1; ++x)
        {
            output.at<unsigned char>(y, x) = processPixel(source, y, x, filter);
        }
    }
}

void processParallel(
    const cv::Mat &input,
    cv::Mat &output,
    Filter filter,
    int numThreads)
{
    validateInput(input);
    validateFilter(filter);

    if (numThreads <= 0)
    {
        throw std::invalid_argument("Thread count must be greater than zero.");
    }

    const cv::Mat source = input;
    output = source.clone();

#pragma omp parallel for num_threads(numThreads) schedule(static)
    for (int y = 1; y < source.rows - 1; ++y)
    {
        for (int x = 1; x < source.cols - 1; ++x)
        {
            output.at<unsigned char>(y, x) = processPixel(source, y, x, filter);
        }
    }
}
