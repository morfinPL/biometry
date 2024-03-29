#include <IO.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <AudioFile.h>

namespace
{
    bool hasEnding(const std::string& fullString, const std::string& ending)
    {
        if (fullString.size() < ending.length())
        {
            return false;
        }
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }

    std::vector<cv::Mat> readGrayImages(const std::vector<std::string>& paths)
    {
        std::vector<cv::Mat> images;
        for (const auto& path : paths)
        {
            cv::Mat image = cv::imread(path);
            if (image.channels() == 3)
            {
                cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
            }
            images.emplace_back(image);
        }
        return images;
    }

    std::vector<std::string> findFilesWithExtensionsInDirectory(const std::string& directory, const std::string& extension)
    {
        std::vector<std::string> pngs;
        for (const auto& p : std::filesystem::directory_iterator(directory))
        {
            if (p.is_regular_file() && hasEnding(p.path().filename().string(), extension))
            {
                pngs.emplace_back(p.path().string());
            }
        }
        return pngs;
    }
}

std::map<int, std::vector<cv::Mat>> IO::readGrayscaleDataset(const std::string& datasetPath, const std::string& extension)
{
    std::map<int, std::vector<cv::Mat>> dataset;
    for (const auto& p : std::filesystem::directory_iterator(datasetPath))
    {
        if (p.is_directory())
        {
            const auto label = std::stoi(p.path().filename());
            const auto images = readGrayImages(findFilesWithExtensionsInDirectory(p.path().string(), extension));
            dataset.emplace(label, images);
        }
    }
    return dataset;
}

void IO::displayImage(const std::string& title, const cv::Mat& image)
{
    cv::imshow(title, image);
    cv::waitKey();
}

void IO::saveWavToTxt(const std::string& outputPath, const AudioFile<double>& sound)
{
    auto output = std::ofstream(outputPath);
    for (auto sample = 0; sample < sound.getNumSamplesPerChannel(); sample++)
    {
        output << std::fixed << std::setprecision(std::numeric_limits<double>::digits)
               << static_cast<double>(sample) / sound.getSampleRate() << "\t";
        for (auto channel = 0; channel < sound.getNumChannels(); channel++)
        {
            output << std::fixed << std::setprecision(std::numeric_limits<double>::digits) << sound.samples[channel][sample] << "\t";
        }
        output << std::endl;
    }
    output.flush();
    output.close();
}

std::vector<AudioFile<double>> IO::readWavDataset(const std::string& datasetPath)
{
    std::vector<AudioFile<double>> data;
    const auto wavs = findFilesWithExtensionsInDirectory(datasetPath, ".wav");
    for (const auto& wav : wavs)
    {
        AudioFile<double> input;
        input.load(wav);
        data.emplace_back(input);
    }
    return data;
}
