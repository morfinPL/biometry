#pragma once
#include <map>
#include <string>
#include <vector>

namespace Configuration
{
	CONFIGURATION_DLL_API std::map<std::string, std::string> parseParameters(const int& argc, const char** argv);
}