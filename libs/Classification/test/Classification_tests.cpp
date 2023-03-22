#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <Classification.h>

TEST_CASE("ANNClassifierConstruction")
{
    Classification::NNClassifier classifier({}, {});
}
