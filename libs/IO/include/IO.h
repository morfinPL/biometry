#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <opencv2/core/types.hpp>

template<class T>
class AudioFile;

namespace IO
{
	IO_DLL_API void saveWavToTxt(const std::string &outputPath, const AudioFile<double> &sound);
	IO_DLL_API std::vector<std::string> findFilesWithExtensionsInDirectory(const std::string& directory, const std::string& extension);
	IO_DLL_API std::vector<cv::Mat> readGrayImages(const std::vector<std::string>& paths);
	IO_DLL_API std::map<int, std::vector<cv::Mat>> readGrayscaleDataset(const std::string& datasetPath, const std::string& extension);
	IO_DLL_API void displayImage(const std::string& title, const cv::Mat& image);
	IO_DLL_API std::vector<AudioFile<double>> readWavDataset(const std::string& datasetPath);
}