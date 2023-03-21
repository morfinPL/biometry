#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <SoundProcessing.h>

TEST_CASE("ASoundProcessingDTW")
{
	SoundProcessing::mfcc({}, 0, 0, 0, 0, 0, 0);
}