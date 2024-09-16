#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

// Global variables to store Canny parameters
int lowThreshold = 0;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char* window_name = "Edge Map";

// Function to apply Canny edge detection
void CannyThreshold(int, void*);

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

    // Create the mask (detectedEdges) with the same size and type as the source grayscale image
    detectedEdges.create(srcGray.size(), CV_8UC1); // CV_8UC1 for single-channel image

    // Create a window to display the result
    cv::namedWindow(window_name);

    // Create a trackbar to adjust the Canny threshold
    cv::createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

    // Initial call to apply Canny with default parameters
    CannyThreshold(0, 0);

    // Wait for a keystroke in the window
    cv::waitKey(0);
    return 0;
}

void CannyThreshold(int, void*) {
    // Apply Gaussian blur to reduce noise
    cv::blur(srcGray, detectedEdges, cv::Size(3, 3));

    // Apply Canny edge detection
    cv::Canny(detectedEdges, detectedEdges, lowThreshold, lowThreshold * ratio, kernel_size);

    // Create a mask with the detected edges
    dstImage.setTo(cv::Scalar(0, 0, 0)); // Clear the destination image
    srcImage.copyTo(dstImage, detectedEdges);

    // Show the result
    cv::imshow(window_name, dstImage);
}