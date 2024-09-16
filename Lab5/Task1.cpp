#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat image;
cv::Mat binary;
cv::Mat gray;
int lowThreshold = 50;
int highThreshold = 150;
int medianBlurKernelSize = 5;
int binaryThreshold = 128;

void updateBinaryImage(int, void*) {
    // Pre-processing
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, medianBlurKernelSize);
    
    // Convert to CV_ 32F, do transform to enhance contrast
    cv::Mat gray32F;
    gray.convertTo(gray32F, CV_32F);
    cv::Mat logTransformed = cv::Mat::zeros(gray32F.size(), CV_32F);
    cv::log(gray32F + 1, logTransformed);
    cv::normalize(logTransformed, logTransformed, 0, 255, cv::NORM_MINMAX);
    cv::convertScaleAbs(logTransformed, gray);

    // Thresholding
    cv::threshold(gray, binary, binaryThreshold, 255, cv::THRESH_BINARY_INV);

    // Edge detection using Canny
    cv::Mat edges;
    cv::Canny(gray, edges, lowThreshold, highThreshold);

    // Morphological operations to clean up the image
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(edges, edges, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(edges, edges, cv::MORPH_OPEN, kernel);


    // Find contours and draw them on the binary image
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::drawContours(binary, contours, -1, cv::Scalar(255), cv::FILLED);

    // Display images
    cv::imshow("Binary Image", binary);
    cv::imshow("Blurred and contrasted Image", gray);
}

void onMedianBlurKernelSizeChange(int, void*) {
    if (medianBlurKernelSize % 2 == 0) {
        medianBlurKernelSize++;
    }
    updateBinaryImage(0, 0);
}


int main(int argc, char** argv) {
    // Load the image
    if(argc < 2){
        std::cout << "Image filename must be provided" << std::endl;
        return 0;
    }
    image = cv::imread(argv[1]);

    if(image.data == NULL){
        std::cout << "Unable to load '" << argv[1] << "'" << std::endl;
        return 0;
    }

    // Get image number as 5th last character of filename
    int num = argv[1][strlen(argv[1]) - 5] - '0';

    // Select preset parameters based on image number
    switch (num)
    {
    case 1:
        lowThreshold = 59;
        highThreshold = 447;
        medianBlurKernelSize = 7;
        binaryThreshold = 175;
        break;
    case 2:
        lowThreshold = 38;
        highThreshold = 302;
        medianBlurKernelSize = 5;
        binaryThreshold = 137;
        break;
    case 3:
        lowThreshold = 49;
        highThreshold = 394;
        medianBlurKernelSize = 5;
        binaryThreshold = 157;
        break;
    }

    // Create a window to display the original image
    cv::namedWindow("Binary Image");

    // Create trackbars for Canny thresholds
    cv::createTrackbar("Low Threshold", "Binary Image", &lowThreshold, 700, updateBinaryImage);
    cv::createTrackbar("High Threshold", "Binary Image", &highThreshold, 2100, updateBinaryImage);
    cv::createTrackbar("Binary Threshold", "Binary Image", &binaryThreshold, 255, updateBinaryImage);
    
    // Create trackbars for median blur and dilation kernel sizes
    cv::createTrackbar("Median Blur Kernel Size", "Binary Image", &medianBlurKernelSize, 31, onMedianBlurKernelSizeChange);

    // Display the original image
    cv::imshow("Original Image", image);

    // Call updateBinaryImage initially to display the binary image with default thresholds
    updateBinaryImage(0, 0);

    // Save the binary and blurred images
    cv::imwrite("binary" + std::to_string(num) + ".png", binary);
    cv::imwrite("blurred" + std::to_string(num) + ".png", gray);

    cv::waitKey(0);

    return 0;
}
