#pragma once
#include <map>
#include <string>
#include <vector>

namespace Configuration
{
	std::map<std::string, std::string> parseParameters(const int& argc, const char** argv);
};