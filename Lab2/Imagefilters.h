#include <opencv2/opencv.hpp>

// Applies a max filter to image
void maxFilter(cv::Mat img, int kernelSize);

// Applies a min filter to image
void minFilter(cv::Mat img, int kernelSize);

// Applies a median filter to image
void medianFilter(cv::Mat img, int kernelSize);

// Does a gaussian smoothing on image
void gaussianSmoothing(cv::Mat img, int kernelSize);