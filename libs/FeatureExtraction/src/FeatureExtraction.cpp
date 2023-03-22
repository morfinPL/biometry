#include <FeatureExtraction.h>

#include <opencv2/core/core.hpp>

namespace
{
    inline int localBinaryPattern(const int row, const int col, const cv::Mat& image)
    {
        int powerOfTwo = 1;
        int result = 0;
        for (const int& i : {-1, 0, 1})
        {
            for (const int& j : {-1, 0, 1})
            {
                if (i != 0 || j != 0)
                {
                    if (image.at<uchar>(row, col) < image.at<uchar>(row + i, col + j))
                    {
                        result += powerOfTwo;
                    }
                    powerOfTwo <<= 1;
                }
            }
        }
        return result;
    }
}

std::pair<std::vector<int>, cv::Mat> FeatureExtraction::localBinaryPatternsHistogram(const cv::Mat& image)
{
    cv::Mat result = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    std::vector<int> histogram(256, 0);
    for (int row = 1; row < image.rows - 1; row++)
    {
        for (int col = 1; col < image.cols - 1; col++)
        {
            const auto lbp = localBinaryPattern(row, col, image);
            histogram[lbp]++;
            result.at<unsigned char>(row, col) = static_cast<unsigned char>(lbp);
        }
    }
    return {histogram, result};
}
