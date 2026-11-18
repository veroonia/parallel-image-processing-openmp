
#include <iostream>
#include <opencv2/opencv.hpp>
#include "gaussian.hpp"

using namespace std;
using namespace cv;

int main()
{
    // Load the original image
    Mat input = imread(
        "images/input/img1.avif",
        IMREAD_GRAYSCALE);

    if (input.empty())
    {
        cerr << "Failed to load image!\n";
        return 1;
    }

    Mat output = input.clone();

    // Apply Gaussian Blur to each non-border pixel
    for (int y = 1; y < input.rows - 1; y++)
    {
        for (int x = 1; x < input.cols - 1; x++)
        {
            output.at<uchar>(y, x) =
                gaussianPixel(input, y, x);
        }
    }

    // Save the blurred image
    if (!imwrite("images/output/gaussian.png", output))
    {
        cerr << "Failed to save image!\n";
        return 1;
    }

    cout << "Gaussian Blur applied successfully!\n";

    // Check the Gaussian calculation with a known result
    Mat test = Mat::zeros(3, 3, CV_8UC1);
    test.at<uchar>(1, 1) = 160;

    int result = gaussianPixel(test, 1, 1);

    cout << "Test result: " << result << "\n";
    cout << "Expected result: 40\n";

    if (result != 40)
    {
        cerr << "Gaussian calculation test failed!\n";
        return 1;
    }

    cout << "Gaussian calculation test passed!\n";

    return 0;
}
