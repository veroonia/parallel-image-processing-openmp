#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    // Load image in grayscale
    Mat image = imread(
        "images/input/img1.avif",
        IMREAD_GRAYSCALE);

    if (image.empty())
    {
        cerr << "Failed to load image!\n";
        return 1;
    }

    cout << "Image loaded successfully!\n";
    cout << "Width: " << image.cols << "\n";
    cout << "Height: " << image.rows << "\n";

    // Save a copy of the image
    if (!imwrite("images/output/copy.png", image))
    {
        cerr << "Failed to save image!\n";
        return 1;
    }

    cout << "Image saved successfully!\n";

    return 0;
}
