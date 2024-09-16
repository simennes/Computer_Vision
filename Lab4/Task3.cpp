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

    // Detect edges using Canny edge detection
    cv::Mat edges;
    cv::Canny(grayImage, edges, 180, 490);

    // Perform Hough transform to detect lines
    std::vector<cv::Vec3f> lines1;
    std::vector<cv::Vec3f> lines2;
    std::vector<cv::Vec3f> lines;

    //Detect lines using Hough transform, use max theta and min theta to detect vertical lines
    cv::HoughLines(edges, lines1, 8, CV_PI / 15, 210, 0, 0, CV_PI/1.5, CV_PI);
    cv::HoughLines(edges, lines2, 8, CV_PI / 15, 210, 0, 0, 0, CV_PI/3);

    // Combine the two sets of lines
    lines.insert(lines.end(), lines1.begin(), lines1.end());
    lines.insert(lines.end(), lines2.begin(), lines2.end());

    // Select the two strongest lines detected using the accumulator count
    int maxCount1 = 0, maxCount2 = 0;
    float maxRho1 = 0, maxRho2 = 0;
    cv::Vec3f strongestLine1, strongestLine2;
    for (size_t i = 0; i < lines.size(); ++i) {
        float rho = lines[i][0], theta = lines[i][1];
        int count = lines[i][2];
        if (count > maxCount1) {
            maxCount2 = maxCount1;
            maxRho2 = maxRho1;
            strongestLine2 = strongestLine1;
            maxCount1 = count;
            maxRho1 = rho;
            strongestLine1 = lines[i];
        }
        else if (count > maxCount2) {
            maxCount2 = count;
            maxRho2 = rho;
            strongestLine2 = lines[i];
        }
    }

    // Find the intersection of the two strongest lines
    float theta1 = strongestLine1[1], theta2 = strongestLine2[1];
    float rho1 = strongestLine1[0], rho2 = strongestLine2[0];
    float x = (rho2 - sin(theta2) * rho1 / sin(theta1)) / (cos(theta2) - sin(theta2) * cos(theta1) / sin(theta1));
    float y = (rho1 - cos(theta1) * x) / sin(theta1);
    cv::Point intersection(x, y);

    // Color the area between the two strongest lines red using fillpoly
    // Make sure the top corners of the polygon are where the strongest lines cross the top of the image
    // and the bottom corners are where the strongest lines cross the bottom of the image
    cv::Mat resultImage = srcImage.clone();
    cv::Point bottom1, bottom2;
    bottom1.x = (strongestLine1[0] / cos(strongestLine1[1])) - (resultImage.rows * tan(strongestLine1[1]));
    bottom1.y = resultImage.rows;
    bottom2.x = (strongestLine2[0] / cos(strongestLine2[1])) - (resultImage.rows * tan(strongestLine2[1]));
    bottom2.y = resultImage.rows;
    cv::Point pts[3]{
        intersection,
        bottom1,
        bottom2
    };
    cv::fillConvexPoly(resultImage, pts, 3, cv::Scalar(0, 0, 255));

    // Display the result
    cv::namedWindow("Result");
    cv::imshow("Result", resultImage);
    cv::waitKey(0);



    return 0;
}