#include <iostream>
#include <random>

#include <AudioFile.h>

#include <Configuration.h>
#include <IO.h>
#include <SoundProcessing.h>

std::vector<int> classify(const int N, const double &frequencyResolution, const std::vector<std::vector<double>> &queries, const std::map<int, std::vector<std::vector<double>>> &train)
{
    std::vector<int> result;
    double distance = std::numeric_limits<double>::max();
    for (const auto& query : queries)
    {
        int tempResult;
        for (const auto& t : train)
        {
			std::cout << std::endl;
            for (const auto& train : t.second) {
                const double temp = SoundProcessing::compareSingnalsMFCC(query, train, 30, 100, 2, 50, N, frequencyResolution);
                std::cout << temp << std::endl;
                if (temp < distance)
                {
                    distance = temp;
                    tempResult = t.first;
                }
            }
        }
        result.emplace_back(tempResult);
    }
    return result;
}


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
    std::string dataset1Path = "D:\\my\\biometry\\testData\\sound\\abrakadabra\\01";
    std::string dataset2Path = "D:\\my\\biometry\\testData\\sound\\czarymary\\01";
    auto data1 = IO::readWavDataset(dataset1Path);
    auto data2 = IO::readWavDataset(dataset2Path);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist1(0, static_cast<int>(data1.size())-1);
    std::uniform_int_distribution<int> dist2(0, static_cast<int>(data2.size())-1);
    int queryId1 = dist1(mt);
    int queryId2 = dist2(mt);
    const auto query1 = data1[queryId1].samples.front();
    const auto query2 = data2[queryId2].samples.front();
    data1.erase(data1.begin() + queryId1);
    data2.erase(data2.begin() + queryId2);
    std::map<int, std::vector<std::vector<double>>> data;
    std::vector<std::vector<double>> data1Samples;
    for (int i = 0; i < static_cast<int>(data1.size()); ++i)
    {
        data1Samples.emplace_back(data1[i].samples.front());
    }
    std::vector<std::vector<double>> data2Samples;
    for (int i = 0; i < static_cast<int>(data1.size()); ++i)
    {
        data2Samples.emplace_back(data2[i].samples.front());
    }
    data.emplace(0, data1Samples);
    data.emplace(1, data2Samples);
    const double frequencyResolution = 1.0 * data1.front().getSampleRate() / N;
    const auto results = classify(N, frequencyResolution, { query1, query2 }, data);
    for (const auto& result : results)
    {
        std::cout << result << std::endl;
    }
}
