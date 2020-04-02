#include <iostream>

#include <Configuration.h>
#include <IO.h>

#include <AudioFile.h>

int main(const int argc, const char **argv)
{
    const auto params(Configuration::parseParameters(argc, argv));
    if (params.empty() || params.count("--help") != 0 || params.count("-h"))
    {
        std::string help = "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<        HELP        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
        help += "--input - path of input wav file, required\n";
        help += "--output - path of output txt file, required\n";
        std::cout << help;
        return 0;
    }
    const auto inputPath(params.at("--input"));
    const auto outputPath(params.at("--output"));

    AudioFile<double> input;
    input.load(inputPath);
    input.printSummary();
	std::cout << "Number of samples: " << input.samples.front().size() << "\n";
	IO::saveWavToTxt(outputPath, input);
}