#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <string>
#include <vector>

#include "benchmark.hpp"
#include "processor.hpp"

using namespace std;
using namespace cv;

namespace
{
const char *filterName(Filter filter)
{
    return filter == Filter::Gaussian ? "Gaussian" : "Sobel";
}

bool saveImage(const string &filePath, const Mat &image)
{
    if (!imwrite(filePath, image))
    {
        cerr << "Failed to save image: " << filePath << "\n";
        return false;
    }

    cout << "Saved image: " << filePath << "\n";
    return true;
}

void printBenchmarkResults(const vector<BenchmarkResult> &results)
{
    for (const BenchmarkResult &result : results)
    {
        cout << filterName(result.filter)
             << " | Threads: " << result.numThreads
             << " | Sequential: " << result.sequentialMilliseconds << " ms"
             << " | Parallel: " << result.parallelMilliseconds << " ms"
             << " | Speedup: " << result.speedup << "x"
             << " | Equivalent: "
             << (result.outputsEquivalent ? "yes" : "no") << "\n";
    }
}
}

int main()
{
    try
    {
        // Load image in grayscale
        Mat image = imread(
            "images/input/img1.png",
            IMREAD_GRAYSCALE);

        if (image.empty())
        {
            cerr << "Failed to load image!\n";
            return 1;
        }

        cout << "Image loaded successfully!\n";
        cout << "Width: " << image.cols << "\n";
        cout << "Height: " << image.rows << "\n";

        const vector<int> threadCounts = defaultThreadCounts();
        const int outputThreads = threadCounts.back();
        const vector<Filter> filters = {
            Filter::Gaussian,
            Filter::Sobel};

        vector<BenchmarkResult> allResults;

        for (const Filter filter : filters)
        {
            Mat sequentialOutput;
            Mat parallelOutput;

            processSequential(image, sequentialOutput, filter);
            processParallel(image, parallelOutput, filter, outputThreads);

            const string baseName =
                filter == Filter::Gaussian ? "gaussian" : "sobel";

            if (!saveImage(
                    "images/output/" + baseName + "_sequential.png",
                    sequentialOutput) ||
                !saveImage(
                    "images/output/" + baseName + "_parallel.png",
                    parallelOutput))
            {
                return 1;
            }

            const vector<BenchmarkResult> filterResults =
                benchmarkThreadCounts(image, filter, threadCounts);

            allResults.insert(
                allResults.end(),
                filterResults.begin(),
                filterResults.end());
        }

        printBenchmarkResults(allResults);

        for (const BenchmarkResult &result : allResults)
        {
            if (!result.outputsEquivalent)
            {
                cerr << "Sequential and parallel outputs are not equivalent for "
                     << filterName(result.filter) << " with "
                     << result.numThreads << " threads.\n";
                return 1;
            }
        }

        writeBenchmarkResults(
            "images/output/benchmark_results.csv",
            allResults);

        cout << "Benchmark results saved to "
             << "images/output/benchmark_results.csv\n";
        cout << "Run scripts/plot_results.py to create the performance chart.\n";
    }
    catch (const exception &error)
    {
        cerr << "Processing failed: " << error.what() << "\n";
        return 1;
    }

    cout << "Image processing completed successfully!\n";

    return 0;
}
