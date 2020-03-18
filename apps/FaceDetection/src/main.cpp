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
	const auto datasetPath = "..\\..\\..\\testData\\GeorgiaTechDatabaseCropped";
	const auto extension = ".jpg";
	const auto dataset = IO::readGrayscaleDataset(datasetPath, extension);
	std::cout << dataset.size() << std::endl;
	cv::imshow("Face", dataset.begin()->second.front());
	cv::waitKey();
	return 0;
}
