#include <complex>
#include <fstream>
#include <iomanip>
#include <limits>
#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

#include <SoundProcessing.h>

#include <AudioFile.h>

namespace
{
    std::vector<std::complex<double>> convertToComplex(const std::vector<double>& doubles)
    {
        std::vector<std::complex<double>> result;
        result.reserve(doubles.size());
        for (auto sample = 0; sample < static_cast<int>(doubles.size()); sample++)
        {
            result.emplace_back(doubles[sample]);
        }
        return result;
    }

    std::vector<double> computeAbs(const std::vector<std::complex<double>>& dft, const double& threshold)
    {
        std::vector<double> result;
        result.reserve(dft.size());
        for (auto sample = 0; sample < static_cast<int>(dft.size()); sample++)
        {
            const auto abs = sqrt(dft[sample].real() * dft[sample].real() + dft[sample].imag() * dft[sample].imag());
            result.emplace_back((abs > threshold) ? abs : 0.0);
        }
        return result;
    }
    struct SamplesWrapper
    {
        const std::vector<double> samples;
        const int begin;
        const int end;

        SamplesWrapper(const std::vector<double>& samples) : samples(samples), begin(0), end(static_cast<int>(samples.size()))
        {
        }

        SamplesWrapper(const std::vector<double>& samples, const int begin, const int end) : samples(samples), begin(begin), end(end)
        {
        }

        double operator[](int sample) const
        {
            if (sample < begin || sample >= end)
            {
                return 0.0;
            }
            else
            {
                return samples[sample];
            }
        }
    };

    int reverseBits(int x, const int n)
    {
        auto result = 0;
        for (auto i = 0; i < n; i++, x >>= 1)
        {
            result = (result << 1) | (x & 1U);
        }
        return result;
    }

    std::vector<std::complex<double>> dfft(const std::vector<std::complex<double>>& image, const bool inverse)
    {
        auto dft(image);
        const auto n = static_cast<int>(dft.size());
        const auto levels = static_cast<int>(floor(log2(n)));
        std::vector<std::complex<double>> expLUT(n / 2);
        const auto inv = (inverse) ? -M_PI : M_PI;
        for (auto i = 0; i < n / 2; i++)
        {
            expLUT[i] = std::exp(std::complex<double>(0, -2 * inv * i / n));
        }
        for (auto i = 0; i < n; i++)
        {
            const auto j = reverseBits(i, levels);
            if (j > i)
            {
                std::swap(dft[i], dft[j]);
            }
        }
        for (auto size = 2; size <= n; size *= 2)
        {
            const auto halfsize = size / 2;
            const auto tablestep = n / size;
            for (auto i = 0; i < n; i += size)
            {
                for (auto j = i, k = 0; j < i + halfsize; j++, k += tablestep)
                {
                    const auto temp = dft[j + halfsize] * expLUT[k];
                    dft[j + halfsize] = dft[j] - temp;
                    dft[j] += temp;
                }
            }
        }
        for (auto& a : dft)
        {
            a /= sqrt(n);
        }
        return dft;
    }
}

std::pair<std::vector<double>, AudioFile<double>> SoundProcessing::autoCorelation(const AudioFile<double> &input, const int N)
{
    const double frequencyResolution = 1.0 * input.getSampleRate() / N;
    AudioFile<double> result = input;
    std::vector<double> frequences;
    auto i = 0;
    for (; i + N < input.getNumSamplesPerChannel(); i += N)
    {
        const SamplesWrapper samples(input.samples.front(), i, i + N);
        std::vector<double> corelation(N);
        for (auto j = 1; j < N; j++)
        {
            for (auto n = 0; n < N; n++)
            {
                corelation[j] += samples[i + n] * samples[i + j + n];
            }
            if (j >= 3)
            {
                if (corelation[j - 2] < corelation[j - 1] && corelation[j - 1] > corelation[j])
                {
                    frequences.push_back(1.0 / (static_cast<double>(j - 1) / input.getSampleRate()));
                    break;
                }
            }
        }
        for (auto j = i; j < i + N; j++)
        {
            double arg = static_cast<double>(j) / result.getSampleRate() * 2 * M_PI * frequences.back();
            result.samples.front()[j] = sin(arg);
        }
    }
    for (; i < input.getNumSamplesPerChannel(); i++)
    {
        result.samples.front()[i] = 0;
    }
    return {std::move(frequences), std::move(result)};
}

std::pair<std::vector<double>, AudioFile<double>> SoundProcessing::fourier(const AudioFile<double> &input, const int N, const double &threshold)
{
    const double frequencyResolution = 1.0 * input.getSampleRate() / N;
    AudioFile<double> result = input;
    AudioFile<double> resultTemp = input;
    std::vector<double> frequences;
    auto i = 0;
    for (; i + N < input.getNumSamplesPerChannel(); i += N)
    {
        std::vector<double> samples(input.samples.front().begin() + i, input.samples.front().begin() + i + N);
        const auto complexSamples(convertToComplex(samples));
        const auto resultAbs = computeAbs(dfft(complexSamples, false), threshold);
        std::copy(resultAbs.begin(), resultAbs.end(), resultTemp.samples.front().begin() + i);
        std::vector<int> maxs;
        for (auto j = 1; j < resultAbs.size() / 2; j++)
        {
            if (resultAbs[j - 1] < resultAbs[j] && resultAbs[j] > resultAbs[j + 1])
            {
                maxs.push_back(static_cast<double>(j));
            }
        }
        frequences.push_back(frequencyResolution * (maxs.back() - maxs.front()) / (maxs.size() - 1));
        for (auto j = i; j < i + N; j++)
        {
            double arg = static_cast<double>(j) / result.getSampleRate() * 2 * M_PI * frequences.back();
            result.samples.front()[j] = sin(arg);
        }
    }
    for (; i < input.getNumSamplesPerChannel(); i++)
    {
        result.samples.front()[i] = 0;
        resultTemp.samples.front()[i] = 0;
    }
    return {std::move(frequences), std::move(result)};
}
