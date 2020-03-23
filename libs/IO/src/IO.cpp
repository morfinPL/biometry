#include <IO.h>

#include <iostream>
#include <filesystem>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace
{
	bool hasEnding(const std::string& fullString, const std::string& ending)
	{
		if (fullString.size() < ending.length())
		{
			return false;
		}
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
}

std::vector<std::string> IO::findImagesWithExtensionsInDirectory(const std::string& directory, const std::string& extension)
{
	std::vector<std::string> pngs;
	for (const auto& p : std::filesystem::directory_iterator(directory))
	{
		if (p.is_regular_file() && hasEnding(p.path().filename().string(), extension))
		{
			pngs.emplace_back(p.path().string());
		}
	}
	return pngs;
}

std::vector<cv::Mat> IO::readGrayImages(const std::vector<std::string>& paths)
{
	std::vector<cv::Mat> images;
	for (const auto& path : paths)
	{
		cv::Mat image = cv::imread(path);
		if (image.channels() == 3)
		{
			cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
		}
		images.emplace_back(image);
	}
	return images;
}

std::map<int, std::vector<cv::Mat>> IO::readGrayscaleDataset(const std::string& datasetPath, const std::string& extension)
{
	std::map<int, std::vector<cv::Mat>> dataset;
	for (const auto& p : std::filesystem::directory_iterator(datasetPath))
	{
		if (p.is_directory())
		{
			const auto label = std::stoi(p.path().filename());
			const auto images = readGrayImages(findImagesWithExtensionsInDirectory(p.path().string(), extension));
			for (const auto& image : images)
			{
				dataset.emplace(label, images);
			}
		}
	}
	return dataset;
}


void IO::displayImage(const std::string& title, const cv::Mat& image)
{
	cv::imshow(title, image);
	cv::waitKey();
}