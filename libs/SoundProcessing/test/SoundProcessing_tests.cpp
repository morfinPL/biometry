#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <SoundProcessing.h>

TEST_CASE("ASoundProcessingDTW")
{
	SoundProcessing::mfcc({}, 0, 0, 0, 0, 0, 0);
}