#include <FeatureExtraction.h>

#include <opencv2/core/core.hpp>

namespace {
	inline int localBinaryPattern(const int row, const int col, const cv::Mat& image)
	{
		int powerOfTwo = 1;
		int result = 0;
		const int center = image.at<char>(row, col);
		for (const int& i : { -1, 0, 1 })
		{
			for (const int& j : { -1, 0, 1 })
			{
				if (i != 0 || j != 0)
				{
					if (image.at<char>(row, col) < image.at<char>(row + i, col + j))
					{
						result += powerOfTwo;
					}
					powerOfTwo <<= 1;
				}
			}
		}
		return result;
	}
}

std::vector<int> FeatureExtraction::localBinaryPatternsHistogram(const cv::Mat& image)
{
	std::vector<int> histogram(256);
	for (int row = 1; row < image.rows - 1; row++)
	{
		for (int col = 1; col < image.cols - 1; col++)
		{
			histogram[localBinaryPattern(row, col, image)]++;
		}
	}
	return histogram;
}