#include "benchmark.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace
{
using Clock = std::chrono::steady_clock;

double elapsedMilliseconds(Clock::time_point start, Clock::time_point end)
{
    return std::chrono::duration<double, std::milli>(end - start).count();
}

bool imagesEquivalent(const cv::Mat &first, const cv::Mat &second)
{
    return first.size() == second.size() &&
           first.type() == second.type() &&
           cv::norm(first, second, cv::NORM_INF) == 0.0;
}

const char *filterName(Filter filter)
{
    return filter == Filter::Gaussian ? "gaussian" : "sobel";
}
}

std::vector<int> defaultThreadCounts()
{
    return {1, 2, 4, 8};
}

BenchmarkResult benchmarkFilter(
    const cv::Mat &input,
    Filter filter,
    int numThreads)
{
    if (numThreads <= 0)
    {
        throw std::invalid_argument("Thread count must be greater than zero.");
    }

    cv::Mat sequentialOutput;
    cv::Mat parallelOutput;

    const Clock::time_point sequentialStart = Clock::now();
    processSequential(input, sequentialOutput, filter);
    const Clock::time_point sequentialEnd = Clock::now();

    const Clock::time_point parallelStart = Clock::now();
    processParallel(input, parallelOutput, filter, numThreads);
    const Clock::time_point parallelEnd = Clock::now();

    const double sequentialMilliseconds =
        elapsedMilliseconds(sequentialStart, sequentialEnd);
    const double parallelMilliseconds =
        elapsedMilliseconds(parallelStart, parallelEnd);

    BenchmarkResult result{
        filter,
        numThreads,
        sequentialMilliseconds,
        parallelMilliseconds,
        parallelMilliseconds > 0.0
            ? sequentialMilliseconds / parallelMilliseconds
            : 0.0,
        imagesEquivalent(sequentialOutput, parallelOutput)};

    return result;
}

std::vector<BenchmarkResult> benchmarkThreadCounts(
    const cv::Mat &input,
    Filter filter,
    const std::vector<int> &threadCounts)
{
    std::vector<BenchmarkResult> results;
    results.reserve(threadCounts.size());

    for (const int numThreads : threadCounts)
    {
        if (numThreads <= 0)
        {
            throw std::invalid_argument("Thread counts must be greater than zero.");
        }

        results.push_back(benchmarkFilter(input, filter, numThreads));
    }

    return results;
}

void writeBenchmarkResults(
    const std::string &filePath,
    const std::vector<BenchmarkResult> &results)
{
    std::ofstream output(filePath);
    if (!output)
    {
        throw std::runtime_error("Could not open benchmark output file: " + filePath);
    }

    output << "filter,threads,sequential_ms,parallel_ms,speedup,equivalent\n";
    output << std::fixed << std::setprecision(6);

    for (const BenchmarkResult &result : results)
    {
        output << filterName(result.filter) << ','
               << result.numThreads << ','
               << result.sequentialMilliseconds << ','
               << result.parallelMilliseconds << ','
               << result.speedup << ','
               << (result.outputsEquivalent ? "true" : "false") << '\n';
    }
}
