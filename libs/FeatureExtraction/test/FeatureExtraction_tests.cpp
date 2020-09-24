#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <FeatureExtraction.h>
#include <opencv2/core/core.hpp>

TEST_CASE("AFeatureExtractionlocalBinaryPatternsHistogram")
{
	auto parameters = FeatureExtraction::localBinaryPatternsHistogram(cv::Mat());
}
