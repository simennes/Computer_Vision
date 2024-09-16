#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    // Load the image
    if (argc < 2) {
        std::cout << "Image filename must be provided" << std::endl;
        return 0;
    }
    cv::Mat srcImage = cv::imread(argv[1]);

    if (srcImage.data == NULL) {
        std::cout << "File '" << argv[1] << "' not found" << std::endl;
        return 0;
    }

    // Convert the image to grayscale
    cv::Mat grayImage;
    cv::cvtColor(srcImage, grayImage, cv::COLOR_BGR2GRAY);

    // Apply Gaussian blur to reduce noise
    cv::GaussianBlur(grayImage, grayImage, cv::Size(5, 5), 0);

    // Apply hough circles transform
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(grayImage, circles, cv::HOUGH_GRADIENT, 1, grayImage.rows / 8, 100, 30, 1, 30);

    // Draw the circles
    cv::Mat resultImage = srcImage.clone();
    for (size_t i = 0; i < circles.size(); ++i) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(resultImage, center, radius, cv::Scalar(0, 0, 255), -1);
    }


    // Display the result
    cv::namedWindow("Result");
    cv::imshow("Result", resultImage);
    cv::waitKey(0);

    return 0;
}