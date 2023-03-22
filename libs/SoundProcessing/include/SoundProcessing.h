#pragma once

#include <string>
#include <utility>
#include <vector>

template <class T>
class AudioFile;

namespace SoundProcessing
{
    SOUNDPROCESSING_DLL_API std::pair<std::vector<double>, AudioFile<double>> autoCorelation(const AudioFile<double>& input, const int N);
    SOUNDPROCESSING_DLL_API std::pair<std::vector<double>, AudioFile<double>> fourier(const AudioFile<double>& input, const int N,
                                                                                      const double& threshold);
    SOUNDPROCESSING_DLL_API std::vector<std::vector<double>> mfcc(const std::vector<double>& input, const int K, const double& d,
                                                                  const double& gamma, const int F, const int N,
                                                                  const double& frequencyResolution);
    SOUNDPROCESSING_DLL_API double compareSingnalsMFCC(const std::vector<double>& sound1, const std::vector<double>& sound2, const int K,
                                                       const double& d, const double& gamma, const int F, const int N,
                                                       const double& frequencyResolution);
    SOUNDPROCESSING_DLL_API double DTW(const std::vector<std::vector<double>>& input1, const std::vector<std::vector<double>>& input2);
}
