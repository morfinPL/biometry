#include <vector>

namespace Classification
{
	class NNClassifier
	{
	private:
		std::vector<int> trainY;
		std::vector<std::vector<int>> trainFeatures;

	public:
		CLASSIFICATION_DLL_API NNClassifier(const std::vector<int> &trainY, const std::vector<std::vector<int>> &trainFeatures);
		CLASSIFICATION_DLL_API std::vector<int> classify(const std::vector<std::vector<int>> &testFeatures) const;
		CLASSIFICATION_DLL_API int classifyOne(const std::vector<int> &features) const;
	};
} // namespace Classification