#include <iostream>
#include <iomanip>
#include <map>
#include <functional>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Configuration.h>
#include <IO.h>
#include <FeatureExtraction.h>
#include <Classification.h>

namespace
{
	inline std::pair<std::map<int, std::vector<cv::Mat>>, std::map<int, std::vector<cv::Mat>>> dataPartition(const int numberOfImagesInTestData, const std::map<int, std::vector<cv::Mat>>& dataset)
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

	inline std::pair<std::vector<int>, std::vector<std::vector<int>>> extractFeatures(const std::map<int, std::vector<cv::Mat>>& dataset, std::function<std::vector<int>(const cv::Mat&)> extractor)
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

	inline int correctResults(const std::vector<int>& groundTruth, const std::vector<int>& results)
	{
		int correct = 0;
		for (int i = 0; i < static_cast<int>(groundTruth.size()); i++)
		{
			if (groundTruth[i] == results[i])
			{
				correct++;
			}
		}
		return correct;
	}
}

int main(const int argc, const char **argv)
{
	if (argc != 7)
	{
		std::cout << "You provide wrong number of parameters!" << std::endl;
		std::cout << "You should provide 3 flags and after each flag value!" << std::endl;
		std::cout << "Example: --dataset ..\\..\\..\\testData\\GeorgiaTechDatabaseCropped --extension .jpg --testNumber 2" << std::endl;
		std::cout << "--dataset - path to your dataset," << std::endl;
		std::cout << "--extension - extension of images to read," << std::endl;
		std::cout << "--testNumber - number of image per class in test set." << std::endl;
		return 1;
	}
	const auto params = Configuration::parseParameters(argc, argv);
	const auto datasetPath = params.at("--dataset");
	const auto extension = params.at("--extension");
	const auto testNumber = std::stoi(params.at("--testNumber"));
	const auto dataset = IO::readGrayscaleDataset(datasetPath, extension);
	const auto [trainDataset, testDataset] = dataPartition(testNumber, dataset);
	const auto [trainY, trainFeatures] = extractFeatures(trainDataset, FeatureExtraction::localBinaryPatternsHistogram);
	const auto [testY, testFeatures] = extractFeatures(testDataset, FeatureExtraction::localBinaryPatternsHistogram);
	const auto classifier = Classification::NNClassifier(trainY, trainFeatures);
	const auto results = classifier.classify(testFeatures);
	for (int i = 0; i < static_cast<int>(testY.size()); i++)
	{
		std::cout << "Ground truth: " << testY[i] << " Result: " << results[i] << std::endl;
	}
	const auto correct = correctResults(testY, results);
	std::cout << "Raport: " << correct << " / " << static_cast<int>(testY.size()) << " correct, percentage: " << std::setprecision(2) << correct / static_cast<double>(testY.size()) * 100 << " %!" << std::endl;
	return 0;
}
