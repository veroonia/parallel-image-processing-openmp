#include "gaussian.hpp"

using namespace cv;

uchar gaussianPixel(const Mat &input, int y, int x)
{
    // Gaussian weights for the 3x3 area
    const int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}};

    int sum = 0;

    // Go through the 3x3 pixels around the current pixel
    for (int ky = -1; ky <= 1; ky++)
    {
        for (int kx = -1; kx <= 1; kx++)
        {
            // Get the value of the neighboring pixel
            int pixel = input.at<uchar>(y + ky, x + kx);

            // Apply the Gaussian weight and add it to the sum
            sum += pixel * kernel[ky + 1][kx + 1];
        }
    }

    // Calculate and return the new blurred pixel value
    return static_cast<uchar>((sum + 8) / 16);
}
