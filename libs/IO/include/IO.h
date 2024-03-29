#pragma once

#include <map>
#include <opencv2/core/types.hpp>
#include <string>
#include <utility>
#include <vector>

template <class T>
class AudioFile;

namespace IO
{
    IO_DLL_API void saveWavToTxt(const std::string& outputPath, const AudioFile<double>& sound);
    IO_DLL_API std::map<int, std::vector<cv::Mat>> readGrayscaleDataset(const std::string& datasetPath, const std::string& extension);
    IO_DLL_API void displayImage(const std::string& title, const cv::Mat& image);
    IO_DLL_API std::vector<AudioFile<double>> readWavDataset(const std::string& datasetPath);
}
