#pragma once

#include <string>
#include <vector>

template<class T>
class AudioFile;

namespace SoundProcessing
{
    std::pair<std::vector<double>, AudioFile<double>> autoCorelation(const AudioFile<double> &input, const int N);
    std::pair<std::vector<double>, AudioFile<double>> fourier(const AudioFile<double> &input, const int N, const double &threshold);
    std::vector<std::vector<double>> mfcc(const std::vector<double>& input, const int K, const double &d, const double &gamma, const int F, const int N, const double &frequencyResolution);
    double compareSingnalsMFCC(const std::vector<double>& sound1, const std::vector<double>& sound2, const int K, const double& d, const double& gamma, const int F, const int N, const double& frequencyResolution);
    std::vector<std::pair<int, int>> DTW(const std::vector<double> &input1, const std::vector<double> &input2);
}