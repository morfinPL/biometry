#include <iostream>
#include <random>

#include <AudioFile.h>

#include <Configuration.h>
#include <IO.h>
#include <SoundProcessing.h>

std::vector<int> classify(const int N, const double &frequencyResolution, const std::vector<std::vector<double>> &queries, const std::map<int, std::vector<std::vector<double>>> &train)
{
    std::vector<int> result;
	auto queryCounter = 0;
    for (const auto& query : queries)
    {
		double distance = std::numeric_limits<double>::max();
        int tempResult = -1;
        for (const auto& t : train)
        {
			auto counter = 0;
            for (const auto& tt : t.second) {
                const double temp = SoundProcessing::compareSingnalsMFCC(query, tt, 60, 50, 2, 25, N, frequencyResolution);
				std::cout << "dtwDistanceOfMFCC: " << temp << " class: " << t.first << " data: " << counter++ << std::endl;
                if (temp < distance)
                {
                    distance = temp;
                    tempResult = t.first;
                }
            }
        }
		std::cout << "query: " << queryCounter++ << " resultClass: " << tempResult << " resultDtwDistanceOfMFCC: " << distance << std::endl;
        result.emplace_back(tempResult);
    }
    return result;
}

std::pair<std::vector<std::vector<double>>, std::map<int, std::vector<std::vector<double>>>> dataPartition(const int id, const std::vector<AudioFile<double>> &data1, const std::vector<AudioFile<double>> &data2, const std::vector<AudioFile<double>> &data3)
{
	const auto query1 = data1[id].samples.front();
    const auto query2 = data2[id].samples.front();
    const auto query3 = data3[id].samples.front();
    std::vector<std::vector<double>> data1Samples;
    for (int i = 0; i < static_cast<int>(data1.size()); ++i)
    {
		if (i == id)
		{
			continue;
		}
        data1Samples.emplace_back(data1[i].samples.front());
    }
    std::vector<std::vector<double>> data2Samples;
    for (int i = 0; i < static_cast<int>(data2.size()); ++i)
    {
		if (i == id)
		{
			continue;
		}
        data2Samples.emplace_back(data2[i].samples.front());
    }
    std::vector<std::vector<double>> data3Samples;
    for (int i = 0; i < static_cast<int>(data3.size()); ++i)
    {
		if (i == id)
		{
			continue;
		}
        data3Samples.emplace_back(data3[i].samples.front());
    }
    std::map<int, std::vector<std::vector<double>>> data;
    data.emplace(0, data1Samples);
    data.emplace(1, data2Samples);
    data.emplace(2, data3Samples);
	return { {query1, query2, query3}, data };
}


int main(const int argc, const char **argv)
{
    const auto params(Configuration::parseParameters(argc, argv));
    if (params.empty() || params.count("--help") != 0 || params.count("-h"))
    {
        std::string help = "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<        HELP        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
        help += "--input - path of input, required\n";
        help += "--window - size of window, required\n";
        std::cout << help;
        return 0;
    }
    const auto N = std::stoi(params.at("--window"));
	const auto inputPath = params.at("--input");
    std::string dataset1Path = inputPath + "\\03";
    std::string dataset2Path = inputPath + "\\02";
    std::string dataset3Path = inputPath + "\\01";
    const auto data1 = IO::readWavDataset(dataset1Path);
    const auto data2 = IO::readWavDataset(dataset2Path);
    const auto data3 = IO::readWavDataset(dataset3Path);
    const double frequencyResolution = 1.0 * data1.front().getSampleRate() / N;
	std::vector<std::vector<int>> confusion(3);
	for (int i = 0; i < static_cast<int>(confusion.size()); ++i)
	{
		confusion[i] = std::vector<int>(3);
	}
	for (int i = 0; i <= 10; ++i)
	{
		const auto[queries, train] = dataPartition(i, data1, data2, data3);
		const auto results = classify(N, frequencyResolution, queries, train);
		for (int j = 0; j < 3; ++j)
		{
			confusion[j][results[j]]++;
		}
	}
	std::cout << "Confusion matrix:" << std::endl;
	for (int i = 0; i < static_cast<int>(confusion.size()); ++i)
	{
		for (int j = 0; j < static_cast<int>(confusion[i].size()); ++j)
		{
			std::cout << confusion[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
