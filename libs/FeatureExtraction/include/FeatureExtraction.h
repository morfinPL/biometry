#pragma once

#include <map>
#include <opencv2/core/types.hpp>
#include <string>
#include <utility>
#include <vector>

namespace FeatureExtraction
{
    FEATUREEXTRACTION_DLL_API std::pair<std::vector<int>, cv::Mat> localBinaryPatternsHistogram(const cv::Mat& image);
}
