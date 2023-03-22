#include <Configuration.h>

#include <iostream>
#include <utility>

std::map<std::string, std::string> Configuration::parseParameters(const int& argc, const char** argv)
{
    std::cout << "Configuration::parseParameters\n";
    std::map<std::string, std::string> parameters;
    for (auto i = 1; i < argc; i += 2)
    {
        std::cout << argv[i] << " = " << argv[i + 1] << '\n';
        parameters.insert(std::make_pair(argv[i], argv[i + 1]));
    }
    return parameters;
}
