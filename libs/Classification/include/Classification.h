#include <vector>

namespace Classification
{
	class NNClassifier
	{
	private:
		std::vector<int> trainY;
		std::vector<std::vector<int>> trainFeatures;

	public:
		NNClassifier(const std::vector<int>& trainY, const std::vector<std::vector<int>>& trainFeatures);
		std::vector<int> classify(const std::vector<std::vector<int>>& testFeatures) const;
		int classifyOne(const std::vector<int>& features) const;
	};
}