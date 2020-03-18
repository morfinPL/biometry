#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <opencv2/core/types.hpp>

namespace FeatureExtraction
{
	std::vector<int> localBinaryPatternsHistogram(const cv::Mat &image);
};