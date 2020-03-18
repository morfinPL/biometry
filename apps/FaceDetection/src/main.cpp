#include <map>
#include <string>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Configuration.h>
#include <IO.h>



int main(const int argc, const char **argv)
{
	const auto params = Configuration::parseParameters(argc, argv);
	const auto sampleFacesDir = "..\\..\\..\\testData\\sampleFaces";
	const auto extension = ".png";
	const auto imagePaths = IO::findImagesWithExtensionsInDirectory(sampleFacesDir, extension);
	std::cout << "The following images are founded:" << std::endl;
	for (const auto &path : imagePaths)
	{
		std::cout << path << std::endl;
	}
	const auto images = IO::readGrayImages(imagePaths);
	for (const auto &image : images)
	{
		cv::imshow("Image", image);
		cv::waitKey();
	}
	return 0;
}
