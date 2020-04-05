#include <iostream>

#include <AudioFile.h>

#include <Configuration.h>
#include <IO.h>
#include <SoundProcessing.h>

int main(const int argc, const char **argv)
{
    const auto params(Configuration::parseParameters(argc, argv));
    if (params.empty() || params.count("--help") != 0 || params.count("-h"))
    {
        std::string help = "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<        HELP        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
        help += "--input - path of input, required\n";
        help += "--output - path of output, optional\n";
        help += "--window - size of window, required\n";
        help += "--treshold - threshold for fourier analysis, required\n";
        std::cout << help;
        return 0;
    }
    const auto inputPath(params.at("--input"));
    const auto outputPath(params.at("--output"));
    const auto N(std::stoi(params.at("--window")));
    const auto treshold(std::stod(params.at("--treshold")));

    AudioFile<double> input;
    input.load(inputPath);
    input.printSummary();
    IO::saveWavToTxt(outputPath + "Orginal.txt", input);
    std::cout << "Autocorelation" << std::endl;
    auto result = SoundProcessing::autoCorelation(input, N);
    for (auto i = 0; i < static_cast<int>(result.first.size()); i++)
    {
        std::cout << result.first[i] << "[Hz] " << std::endl;
    }
    IO::saveWavToTxt(outputPath + ".txt", result.second);
    result.second.save(outputPath + ".wav");
    std::cout << "Fourier" << std::endl;
    auto fourierResult = SoundProcessing::fourier(input, N, treshold);
    IO::saveWavToTxt(outputPath + "fourier.txt", fourierResult.second);
    fourierResult.second.save(outputPath + "fourier.wav");
    for (auto i = 0; i < static_cast<int>(fourierResult.first.size()); i++)
    {
        std::cout << fourierResult.first[i] << "[Hz] " << std::endl;
    }
}
