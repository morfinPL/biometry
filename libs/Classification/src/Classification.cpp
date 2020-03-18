#include <Classification.h>

#include <algorithm>
#include <cmath>

Classification::NNClassifier::NNClassifier(const std::vector<int>& trainY, const std::vector<std::vector<int>>& trainFeatures) : trainY(trainY), trainFeatures(trainFeatures) {}

int Classification::NNClassifier::classifyOne(const std::vector<int>& features) const
{
	std::vector<double> floatingFeatures;
	for (const auto& f : features)
	{
		floatingFeatures.emplace_back(static_cast<double>(f));
	}
	std::vector<double> distances;
	for (const auto& featuresTrain : trainFeatures)
	{
		double result = 0.0;
		for (int i = 0; i < static_cast<int>(floatingFeatures.size()); i++)
		{
			result += std::pow(std::abs(floatingFeatures[i] - featuresTrain[i]), 2);
		}
		distances.emplace_back(result);
	}
	const auto index = std::min_element(distances.begin(), distances.end()) - distances.begin();
	return trainY[index];
}

std::vector<int> Classification::NNClassifier::classify(const std::vector<std::vector<int>> &testFeatures) const
{
	std::vector<int> results;
	for (const auto& features : testFeatures)
	{
		results.emplace_back(classifyOne(features));
	}
	return results;
}