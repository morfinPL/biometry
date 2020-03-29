#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <opencv2/core/types.hpp>

namespace IO
{
	std::vector<std::string> findImagesWithExtensionsInDirectory(const std::string& directory, const std::string& extension);
	std::vector<cv::Mat> readGrayImages(const std::vector<std::string>& paths);
	std::map<int, std::vector<cv::Mat>> readGrayscaleDataset(const std::string& datasetPath, const std::string& extension);
};