#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <opencv2/core/core.hpp>

#include <IO.h>

TEST_CASE("AIOReadGrayImages")
{
	const auto images = IO::findFilesWithExtensionsInDirectory(".", ".jpg");
}