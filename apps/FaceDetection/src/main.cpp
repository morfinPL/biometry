#include <map>
#include <string>
#include <iostream>
#include <filesystem>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Configuration.h>

namespace
{
bool hasEnding(const std::string &fullString, const std::string &ending)
{
	if (fullString.size() < ending.length())
	{
		return false;
	}
	return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
}

std::vector<std::string> findPNGFromDirectory(const std::string &directory)
{
	std::vector<std::string> pngs;
	for (const auto &p : std::filesystem::directory_iterator(directory))
	{
		if (p.is_regular_file() && hasEnding(p.path().filename().string(), ".png"))
		{
			pngs.emplace_back(p.path().string());
		}
	}
	return pngs;
}

std::vector<cv::Mat> readGrayImages(const std::vector<std::string> &paths)
{
	std::vector<cv::Mat> images;
	for (const auto &path : paths)
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
} // namespace

int main(const int argc, const char **argv)
{
	const auto params = Configuration::parseParameters(argc, argv);
	const auto sampleFacesDir = "..\\..\\..\\testData\\sampleFaces";
	const auto imagePaths = findPNGFromDirectory(sampleFacesDir);
	std::cout << "The following images are founded:" << std::endl;
	for (const auto &path : imagePaths)
	{
		std::cout << path << std::endl;
	}
	const auto images = readGrayImages(imagePaths);
	for (const auto &image : images)
	{
		cv::imshow("Image", image);
		cv::waitKey();
	}
	return 0;
}
