#include <complex>
#include <fstream>
#include <iomanip>
#include <limits>
#include <unordered_map>
#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

#include <SoundProcessing.h>

#include <AudioFile.h>

namespace
{

	class DTWWrapper
	{
    private:
        const int size;
        const int size2;
        const double a;
        const int window;
        std::vector<std::unordered_map<int, double>> matrix;
	public:
		DTWWrapper(const int size1, const int size2, const int window) : size(size1), size2(size2), a(static_cast<double>(size2) / static_cast<double>(size1)), window(window), matrix(size1) {}
		double at(const int i, const int j) const
		{
			if (i == 0 || j == 0)
			{
				return 0.0;
			}
			if (i >= 0 && i < size && std::max(1, static_cast<int>(a * i - window)) < j && j < std::min(size2, static_cast<int>(a * i + window)))
			{
				return matrix[i].at(j);
			}
			return std::numeric_limits<double>::max();
		}

		void set(const int i, const int j, const double &value)
		{
			if (std::max(1, static_cast<int>(a * i - window)) < j && j < std::min(size2, static_cast<int>(a * i + window)))
			{
				matrix[i].emplace(j, value);
			}
		}
	};

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

    inline double hz2mel(double f) {
        return 2595 * std::log10(1 + f / 700);
    }

    inline double mel2hz(double m) {
        return 700 * (std::pow(10, m / 2595) - 1);
    }

    std::vector<double> initHammington(const std::vector<double>& doubles)
    {
        std::vector<double> result;
        for (int i = 0; i < static_cast<int>(doubles.size()); ++i)
        {
            result.emplace_back((0.54 - 0.46 * cos(i * 2 * M_PI / (doubles.size() - 1))) * doubles[i]);
        }
        return result;
    }

    std::vector<double> initFilter(const int k, const int N, const double& d, const double& frequencyResolution)
    {
        double c = mel2hz(k * d);
        double l = mel2hz((k - 1) * d);
        double r = mel2hz((k + 1) * d);
        std::vector<double> filter;
        for (int i = 0; i < N; ++i)
        {
            auto f = i * frequencyResolution;
            double temp = 0;
            if (f >= l && f <= c)
            {
                temp = (f - l) / (c - l);
            }
            else if (f >= c && f <= r)
            {
                temp = (r - f) / (r - c);
            }
            filter.emplace_back(temp);
        }
        return filter;
    }

    double computeS(const std::vector<double>& signal, const std::vector<double>& filter)
    {
        double result = 0.0;
        for (int i = 0; i < static_cast<int>(signal.size()) / 2; ++i)
        {
            result += signal[i] * filter[i];
        }
        return result;
    }

    double computeMel(const int n, const int K, const std::vector<double>& sValues)
    {
        double result = 0.0;
        for (int k = 0; k < K; ++k)
        {
            result += sValues[k] * cos(2 * M_PI * ((2 * k + 1) * n) / (4 * K));
        }
        return result;
    }
}

std::pair<std::vector<double>, AudioFile<double>> SoundProcessing::autoCorelation(const AudioFile<double> &input, const int N)
{
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
        for (int j = 1; j < static_cast<int>(resultAbs.size()) / 2; j++)
        {
            if (resultAbs[j - 1LL] < resultAbs[j] && resultAbs[j] > resultAbs[j + 1LL])
            {
                maxs.push_back(static_cast<int>(j));
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

double distance(const std::vector<double> &vec1, const std::vector<double> &vec2)
{
	double result = 0.0;
	for (int i = 0; i < static_cast<int>(vec1.size()); ++i)
	{
		result += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i]);
	}
	return sqrt(result);
}

double SoundProcessing::DTW(const std::vector<std::vector<double>>& input1, const std::vector<std::vector<double>>& input2)
{
    const int window = 11;
	const double a = static_cast<double>(input2.size()) / static_cast<double>(input1.size());
    DTWWrapper matrix(static_cast<int>(input1.size()), static_cast<int>(input2.size()), window);
	for (int i = 1; i < static_cast<int>(input1.size()); ++i)
    {
        for (int j = std::max(1, static_cast<int>(a * i - window)); j < std::min(static_cast<int>(input2.size()), static_cast<int>(a * i + window)); ++j)
        {
            const auto cost = distance(input1[i - 1], input2[j - 1]);
            const auto lastMin = std::min({ matrix.at(i - 1, j), matrix.at(i, j - 1), matrix.at(i - 1, j - 1) });
            matrix.set(i, j, cost + lastMin);
        }
    }
    std::pair<int, int> currentCell (static_cast<int>(input1.size()) -1, static_cast<int>(input2.size()) -1);
    std::pair<int, int> end(0,0);
	double result = 0.0;
    while (currentCell != end)
    {
        std::pair<int, int> neigborA(currentCell.first - 1, currentCell.second);
        std::pair<int, int> neigborB(currentCell.first, currentCell.second - 1);
        std::pair<int, int> neigborC(currentCell.first - 1, currentCell.second - 1);
        double tempValue = matrix.at(neigborA.first, neigborA.second);
        currentCell = neigborA;
        if (matrix.at(neigborB.first, neigborB.second) < tempValue)
        {
            tempValue = matrix.at(neigborB.first, neigborB.second);
            currentCell = neigborB;
        }
        if (matrix.at(neigborC.first,neigborC.second) < tempValue)
        {
            currentCell = neigborC;
            tempValue = matrix.at(neigborC.first, neigborC.second);
        }
        result += tempValue;
    }
    return result;
}

std::vector<std::vector<double>> SoundProcessing::mfcc(const std::vector<double>& input, const int K, const double &d, const double &gamma, const int F, const int N, const double &frequencyResolution)
{
    auto i = 0;
    std::vector<std::vector<double>> filterBank;
    std::vector<std::vector<double>> results;
    for (int j = 1; j <= K; ++j)
    {
        filterBank.emplace_back(initFilter(j, N, d, frequencyResolution));
    }
    for (; i + N < static_cast<int>(input.size()); i += N)
    {
        std::vector<double> samples(input.begin() + i, input.begin() + i + N);
        samples = initHammington(samples);
        const auto complexSamples(convertToComplex(samples));
        const auto resultAbs = computeAbs(dfft(complexSamples, false), 0.0);        
        std::vector<double> sValues;
        for(int j = 0; j < K; ++j)
        {
            sValues.emplace_back(std::pow(std::log(computeS(resultAbs, filterBank[j])), gamma));
        }
        results.push_back({});
        for(int j = 1; j < 1 + F; ++j)
        {
            results.back().emplace_back(computeMel(j, K, sValues));
        }
    }
    return results;
}


double SoundProcessing::compareSingnalsMFCC(const std::vector<double>& sound1, const std::vector<double>& sound2, const int K, const double& d, const double& gamma, const int F, const int N, const double& frequencyResolution)
{
    const auto mel1 = SoundProcessing::mfcc(sound1, K, d, gamma, F, N, frequencyResolution);
    const auto mel2 = SoundProcessing::mfcc(sound2, K, d, gamma, F, N, frequencyResolution);
    return DTW(mel1, mel2);
}
