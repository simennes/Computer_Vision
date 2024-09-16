#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
    // Load the image
    if(argc != 3){
        std::cout << "Two image filenames must be provided" << std::endl;
        return 0;
    }
    cv::Mat image1 = cv::imread(argv[1]);
    cv::Mat image2 = cv::imread(argv[2]);

    if(image1.data == NULL){
        std::cout << "Unable to load '" << argv[1] << "'" << std::endl;
        return 0;
    }

    if(image2.data == NULL){
        std::cout << "Unable to load '" << argv[2] << "'" << std::endl;
        return 0;
    }

    // Convert the images to grayscale
    cv::Mat grayImage1, grayImage2;
    cv::cvtColor(image1, grayImage1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(image2, grayImage2, cv::COLOR_BGR2GRAY);

    // Initiate SIFT detector
    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();

    // Find the keypoints and descriptors with SIFT
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    sift->detectAndCompute(grayImage1, cv::noArray(), keypoints1, descriptors1);
    sift->detectAndCompute(grayImage2, cv::noArray(), keypoints2, descriptors2);

    // Use FLANN based matcher
    cv::FlannBasedMatcher matcher;
    std::vector<std::vector<cv::DMatch>> knnMatches;
    matcher.knnMatch(descriptors1, descriptors2, knnMatches, 2);

    // Apply ratio test
    std::vector<cv::DMatch> goodMatches;
    for (size_t i = 0; i < knnMatches.size(); i++) {
        if (knnMatches[i][0].distance < 0.75 * knnMatches[i][1].distance) {
            goodMatches.push_back(knnMatches[i][0]);
        }
    }

    // Draw the matches
    cv::Mat imgMatches;
    cv::drawMatches(image1, keypoints1, image2, keypoints2, goodMatches, imgMatches, cv::Scalar::all(-1),
                    cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    // Display the matches
    cv::imshow("Matches", imgMatches);
    cv::waitKey(0);

    // Calculate the percentage of matched keypoints
    double similarity = static_cast<double>(goodMatches.size()) / std::min(keypoints1.size(), keypoints2.size()) * 100.0;

    std::cout << "Similarity: " << similarity << "%" << std::endl;

    // Set the threshold for similarity classification
    double similarityThreshold = 50.0; // Adjusted threshold based on new calculation

    // Compare the similarity with the threshold
    if (similarity >= similarityThreshold) {
        std::cout << "The two images have similar content" << std::endl;
    } else {
        std::cout << "The two images have different content" << std::endl;
    }

    return 0;
}
