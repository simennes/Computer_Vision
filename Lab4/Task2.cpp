#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>

// Global variables to store Canny parameters
int lowThreshold = 80;
const int max_lowThreshold = 200;
const int ratio = 3;
const int kernel_size = 3;
const char* window_name = "Edge Map";

int H_min = 0;
int H_max = 255;
int S_min = 0;
int S_max = 255;
int V_min = 0;
int V_max = 255;
const int HSV_stop = 255;

// Function to apply Canny edge detection
void lineDetect(int, void*);

// Global variable for the source image
cv::Mat srcImage, srcGray, dstImage, detectedEdges;

int main(int argc, char** argv){
    // Load the image
    if(argc < 2){
        std::cout << "Image filename must be provided" << std::endl;
        return 0;
    }
    srcImage = cv::imread(argv[1]);

    if(srcImage.data == NULL){
        std::cout << "File '" << argv[1] << "' not found" << std::endl;
        return 0;
    }

    // Convert the source image to grayscale
    cv::cvtColor(srcImage, srcGray, cv::COLOR_BGR2GRAY);

    // Display original image
    cv::namedWindow("Original image");
    cv::imshow("Original image", srcImage);

    // Create the destination image with the same size and type as the source image
    dstImage.create(srcImage.size(), srcImage.type());

    // Create the masks with the same size and type as the source grayscale image
    detectedEdges.create(srcGray.size(), CV_8UC1);

    cv::namedWindow(window_name);

    // Create a trackbar to adjust the Canny threshold
    cv::createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, lineDetect);
    cv::createTrackbar("Min H:", window_name, &H_min, HSV_stop, lineDetect);
    cv::createTrackbar("Max H:", window_name, &H_max, HSV_stop, lineDetect);
    cv::createTrackbar("Min S:", window_name, &S_min, HSV_stop, lineDetect);
    cv::createTrackbar("Max S:", window_name, &S_max, HSV_stop, lineDetect);
    cv::createTrackbar("Min V:", window_name, &V_min, HSV_stop, lineDetect);
    cv::createTrackbar("Max V:", window_name, &V_max, HSV_stop, lineDetect);

    lineDetect(0, 0);
    cv::waitKey(0);

    return 0;
}

void lineDetect(int, void*){
    // Apply Canny edge detection
    cv::blur(srcGray, detectedEdges, cv::Size(3, 3));
    cv::Canny(detectedEdges, detectedEdges, lowThreshold, lowThreshold * ratio, kernel_size);

    // Compute gradient of the grayscale image
    cv::Mat grad_x, grad_y;
    cv::Sobel(srcGray, grad_x, CV_32F, 1, 0);
    cv::Sobel(srcGray, grad_y, CV_32F, 0, 1);

    // Compute edge orientation
    cv::Mat edge_magnitude, edge_orientation;
    cv::cartToPolar(grad_x, grad_y, edge_magnitude, edge_orientation);

    // Create a color-based mask
    cv::Mat hsvImage;
    cv::cvtColor(srcImage, hsvImage, cv::COLOR_BGR2HSV);
    cv::Mat whiteMask;
    cv::inRange(hsvImage, cv::Scalar(H_min, S_min, V_min), cv::Scalar(H_max, S_max, V_max), whiteMask);

    // Filter out edges based on orientation
    cv::Mat filteredEdges;
    detectedEdges.copyTo(filteredEdges);
    for (int i = 0; i < detectedEdges.rows; ++i) {
        for (int j = 0; j < detectedEdges.cols; ++j) {
            if (detectedEdges.at<uchar>(i, j) > 0) {
                float orientation = edge_orientation.at<float>(i, j);
                // Filter out edges not aligned with expected orientation (e.g., from bottom upward, slightly left to right)
                if (orientation < CV_PI / 4 || orientation > 3 * CV_PI / 4) { // Lines inclined between 30 to 60 degrees from horizontal
                    filteredEdges.at<uchar>(i, j) = 0;
                }
            }
        }
    }

    // Combine Canny edge mask and color-based mask
    cv::bitwise_and(filteredEdges, whiteMask, detectedEdges);

    // Show the result
    cv::imshow(window_name, detectedEdges);
}