#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <FeatureExtraction.h>
#include <opencv2/core/core.hpp>

TEST_CASE("ALocalBinaryPatternsHistogramReturnEmptyHistogramForEmptyImage")
{
	const auto [histogram, output] = FeatureExtraction::localBinaryPatternsHistogram(cv::Mat());
	for (const auto& h : histogram)
	{
		REQUIRE(h == 0);
	}
}

TEST_CASE("ALocalBinaryPatternsHistogramReturnCorrectValueFor3x3Image")
{
	cv::Mat image = cv::Mat::zeros(3, 3, CV_8UC1);
	image.at<uchar>(0, 0) = static_cast<uchar>(255);
	image.at<uchar>(0, 2) = static_cast<uchar>(255);
	image.at<uchar>(2, 0) = static_cast<uchar>(255);
	image.at<uchar>(2, 2) = static_cast<uchar>(255);
	const auto [histogram, output] = FeatureExtraction::localBinaryPatternsHistogram(image);
	for (int i = 0; i < static_cast<int>(histogram.size()); ++i)
	{
		if (i == 165)
		{
			REQUIRE(histogram[i] == 1);
		}
		else
		{
			REQUIRE(histogram[i] == 0);
		}
	}
}
