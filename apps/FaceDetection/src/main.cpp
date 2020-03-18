#include <iostream>
#include <map>
#include <functional>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Configuration.h>
#include <IO.h>
#include <FeatureExtraction.h>

std::pair<std::map<int, std::vector<cv::Mat>>, std::map<int, std::vector<cv::Mat>>> dataPartition(const int numberOfImagesInTestData, const std::map<int, std::vector<cv::Mat>>& dataset)
{
	std::map<int, std::vector<cv::Mat>> train(dataset.cbegin(), dataset.cend());
	std::map<int, std::vector<cv::Mat>> test;
	for (auto& object : train)
	{
		const auto images = object.second;
		const auto label = object.first;
		object.second = std::vector<cv::Mat>(images.begin(), images.begin() + (static_cast<int>(images.size()) - numberOfImagesInTestData));
		test.emplace(label, std::vector<cv::Mat>(images.begin() + (static_cast<int>(images.size()) - numberOfImagesInTestData), images.end()));
	}
	return { train, test };
}

std::pair<std::vector<int>, std::vector<std::vector<int>>> extractFeatures(const std::map<int, std::vector<cv::Mat>>& dataset, std::function<std::vector<int>(const cv::Mat&)> extractor)
{
	std::vector<int> labels;
	std::vector<std::vector<int>> features;
	for (const auto& object : dataset)
	{
		const auto label = object.first;
		for (const auto& image : object.second)
		{
			labels.emplace_back(label);
			features.emplace_back(extractor(image));
		}
	}
	return { labels, features };
}



int main(const int argc, const char **argv)
{
	const auto params = Configuration::parseParameters(argc, argv);
	const auto datasetPath = "..\\..\\..\\testData\\GeorgiaTechDatabaseCropped";
	const auto extension = ".jpg";
	const auto dataset = IO::readGrayscaleDataset(datasetPath, extension);
	const auto [trainDataset, testDataset] = dataPartition(2, dataset);
	const auto [trainY, trainFeatures] = extractFeatures(trainDataset, FeatureExtraction::localBinaryPatternsHistogram);
	const auto [testY, testFeatures] = extractFeatures(trainDataset, FeatureExtraction::localBinaryPatternsHistogram);
	return 0;
}
