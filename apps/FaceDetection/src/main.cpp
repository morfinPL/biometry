#include <map>
#include <string>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Configuration.h>
#include <IO.h>
#include <FeatureExtraction.h>

int main(const int argc, const char **argv)
{
	const auto params = Configuration::parseParameters(argc, argv);
	const auto datasetPath = "..\\..\\..\\testData\\GeorgiaTechDatabaseCropped";
	const auto extension = ".jpg";
	const auto dataset = IO::readGrayscaleDataset(datasetPath, extension);
	std::cout << dataset.size() << std::endl;
	const auto histogram = FeatureExtraction::localBinaryPatternsHistogram(dataset.begin()->second.front());
	for (int i = 0; i < static_cast<int>(histogram.size()); i++)
	{
		std::cout << i << " " << histogram[i] << std::endl;
	}
	return 0;
}
