#include <iostream>
#include <random>

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
    const auto N = std::stoi(params.at("--window"));
    std::string dataset1Path = "C:\\Projects\\biometry\\testData\\sound\\abrakadabra\\01";
    std::string dataset2Path = "C:\\Projects\\biometry\\testData\\sound\\czarymary\\01";
    auto data1 = IO::readWavDataset(dataset1Path);
    auto data2 = IO::readWavDataset(dataset2Path);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist1(0, static_cast<int>(data1.size())-1);
    std::uniform_int_distribution<int> dist2(0, static_cast<int>(data2.size())-1);
    int queryId1 = dist1(mt);
    int queryId2 = dist2(mt);
    const auto query1 = data1[queryId1];
    const auto query2 = data2[queryId2];
    data1.erase(data1.begin() + queryId1);
    data2.erase(data2.begin() + queryId2);
    std::map<int, std::vector<AudioFile<double>>> data;
    data.emplace(0, data1);
    data.emplace(1, data2);
    const double frequencyResolution = 1.0 * query1.getSampleRate() / N;
    std::cout << "query1 - abrakadabra" << std::endl;
    for (const auto label : data)
    {
        for (const auto audio : label.second)
        {
            std::cout << ((label.first == 0) ? "abrakadabra" : "czarymary" ) << " distance: " << SoundProcessing::compareSingnalsMFCC(query1.samples.front(), audio.samples.front(), 7, 100, 2, 15, N, frequencyResolution) << std::endl;
        }
    }
    std::cout << "query2 - czarymary" << std::endl;
    for (const auto label : data)
    {
        for (const auto audio : label.second)
        {
            std::cout << ((label.first == 0) ? "abrakadabra" : "czarymary") << " distance: " << SoundProcessing::compareSingnalsMFCC(query2.samples.front(), audio.samples.front(), 7, 100, 2, 15, N, frequencyResolution) << std::endl;
        }
    }
}
