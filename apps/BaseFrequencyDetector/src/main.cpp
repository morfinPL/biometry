#include <iostream>

#include <Configuration.h>

#include <AudioFile.h>

int main(const int argc, const char **argv)
{
    const auto params(Configuration::parseParameters(argc, argv));
    if (params.empty() || params.count("--help") != 0 || params.count("-h"))
    {
        std::string help = "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<        HELP        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
        help += "--input - path of input image, required\n";
        std::cout << help;
        return 0;
    }
    const auto inputPath(params.at("--input"));

    AudioFile<double> input;
    input.load(inputPath);
    input.printSummary();
	std::cout << "Number of samples: " << input.samples.front().size() << "\n";
}