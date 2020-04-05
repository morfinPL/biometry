#pragma once

#include <string>
#include <vector>

template<class T>
class AudioFile;

namespace SoundProcessing
{
    std::pair<std::vector<double>, AudioFile<double>> autoCorelation(const AudioFile<double> &input, const int N);
    std::pair<std::vector<double>, AudioFile<double>> fourier(const AudioFile<double> &input, const int N, const double &threshold);
}