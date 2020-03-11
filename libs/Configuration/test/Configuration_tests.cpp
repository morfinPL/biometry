#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <Configuration.h>

TEST_CASE("AConfigurationParseParametersCorrectly")
{
	constexpr auto argc = 7;
	const char *argv[] = {"name", "--first", "first", "--second", "second", "--third", "third"};
	auto parameters = Configuration::parseParameters(argc, argv);
	REQUIRE(parameters.at("--first") == "first");
	REQUIRE(parameters.at("--second") == "second");
	REQUIRE(parameters.at("--third") == "third");
}