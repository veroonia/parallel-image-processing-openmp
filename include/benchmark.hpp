#pragma once

#include "processor.hpp"

#include <opencv2/core.hpp>

#include <string>
#include <vector>

struct BenchmarkResult
{
    Filter filter;
    int numThreads;
    double sequentialMilliseconds;
    double parallelMilliseconds;
    double speedup;
    bool outputsEquivalent;
};

std::vector<int> defaultThreadCounts();

BenchmarkResult benchmarkFilter(
    const cv::Mat &input,
    Filter filter,
    int numThreads);

std::vector<BenchmarkResult> benchmarkThreadCounts(
    const cv::Mat &input,
    Filter filter,
    const std::vector<int> &threadCounts);

void writeBenchmarkResults(
    const std::string &filePath,
    const std::vector<BenchmarkResult> &results);
