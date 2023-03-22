#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Classification.h>
#include <Configuration.h>
#include <FeatureExtraction.h>
#include <IO.h>

namespace
{
    inline std::pair<std::map<int, std::vector<cv::Mat>>, std::map<int, std::vector<cv::Mat>>> dataPartition(
        const int numberOfImagesInTestData, const std::map<int, std::vector<cv::Mat>>& dataset)
    {
        std::map<int, std::vector<cv::Mat>> train(dataset.cbegin(), dataset.cend());
        std::map<int, std::vector<cv::Mat>> test;
        for (auto& object : train)
        {
            const auto images = object.second;
            const auto label = object.first;
            object.second =
                std::vector<cv::Mat>(images.begin(), images.begin() + (static_cast<int>(images.size()) - numberOfImagesInTestData));
            test.emplace(label,
                         std::vector<cv::Mat>(images.begin() + (static_cast<int>(images.size()) - numberOfImagesInTestData), images.end()));
        }
        return {train, test};
    }

    inline std::tuple<std::vector<int>, std::vector<std::vector<int>>, std::vector<cv::Mat>> extractFeatures(
        const std::map<int, std::vector<cv::Mat>>& dataset, std::function<std::pair<std::vector<int>, cv::Mat>(const cv::Mat&)> extractor)
    {
        std::vector<int> labels;
        std::vector<std::vector<int>> features;
        std::vector<cv::Mat> debug;
        for (const auto& object : dataset)
        {
            const auto label = object.first;
            for (const auto& image : object.second)
            {
                const auto extracted = extractor(image);
                labels.emplace_back(label);
                features.emplace_back(extracted.first);
                debug.emplace_back(extracted.second);
            }
        }
        return {labels, features, debug};
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

// #lizard forgives the complexity
int main(const int argc, const char** argv)
{
    if (argc != 9)
    {
        std::cout << "You provide wrong number of parameters!" << std::endl;
        std::cout << "You should provide 3 flags and after each flag value!" << std::endl;
        std::cout << "Example: --dataset ..\\..\\..\\testData\\GeorgiaTechDatabaseCropped --extension .jpg --testNumber 2 --debug 1"
                  << std::endl;
        std::cout << "--dataset - path to your dataset," << std::endl;
        std::cout << "--extension - extension of images to read," << std::endl;
        std::cout << "--testNumber - number of image per class in test set," << std::endl;
        std::cout << "--debug - [0 | 1] debug output switch, 0 - disable." << std::endl;
        return 1;
    }
    const auto params = Configuration::parseParameters(argc, argv);
    const auto datasetPath = params.at("--dataset");
    const auto extension = params.at("--extension");
    const auto testNumber = std::stoi(params.at("--testNumber"));
    const auto debug = std::stoi(params.at("--debug")) != 0;
    const auto dataset = IO::readGrayscaleDataset(datasetPath, extension);
    const auto [trainDataset, testDataset] = dataPartition(testNumber, dataset);
    const auto [trainY, trainFeatures, trainDebug] = extractFeatures(trainDataset, FeatureExtraction::localBinaryPatternsHistogram);
    const auto [testY, testFeatures, testDebug] = extractFeatures(testDataset, FeatureExtraction::localBinaryPatternsHistogram);
    const auto classifier = Classification::NNClassifier(trainY, trainFeatures);
    const auto results = classifier.classify(testFeatures);
    if (debug)
    {
        for (int i = 0; i < static_cast<int>(testY.size()); i++)
        {
            std::cout << "Ground truth: " << testY[i] << " Result: " << results[i] << std::endl;
            IO::displayImage("Test extracted image", trainDebug[i]);
        }
    }
    const auto correct = correctResults(testY, results);
    std::cout << "Raport: " << correct << " / " << static_cast<int>(testY.size()) << " correct, percentage: " << std::setprecision(2)
              << correct / static_cast<double>(testY.size()) * 100 << " %!" << std::endl;
    return 0;
}
