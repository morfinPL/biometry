#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <opencv2/core/types.hpp>

namespace FeatureExtraction
{
	FEATUREEXTRACTION_DLL_API std::pair<std::vector<int>, cv::Mat> localBinaryPatternsHistogram(const cv::Mat &image);
}