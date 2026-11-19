#include "sobel.hpp"

#include <cmath>

unsigned char sobelPixel(const cv::Mat &input, int y, int x)
{
    const int kernelX[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    const int kernelY[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    int gradientX = 0;
    int gradientY = 0;

    for (int ky = -1; ky <= 1; ++ky)
    {
        for (int kx = -1; kx <= 1; ++kx)
        {
            const int pixel = input.at<unsigned char>(y + ky, x + kx);
            gradientX += pixel * kernelX[ky + 1][kx + 1];
            gradientY += pixel * kernelY[ky + 1][kx + 1];
        }
    }

    const double magnitude = std::sqrt(
        static_cast<double>(gradientX * gradientX + gradientY * gradientY));

    return cv::saturate_cast<unsigned char>(magnitude);
}
