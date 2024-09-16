// Main author: Jan Kristian Alstergren
#ifndef BALLTRACKER_HPP
#define BALLTRACKER_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include "TableDetector.hpp"
#include "BallDetector.hpp"
#include "TopViewVisual.hpp"

class BallTracker {
    // // To track balls, run the following code in main.cpp:
    // // All the ball table and ball detection is done within the class
    // BallTracker ballTracker;
    // ballTracker.trackBalls(video);
    public:
    std::vector<cv::Rect> boundingBoxes;

    std::vector<cv::Rect> expandBoundingBoxes(std::vector<BoundingBox> inputBoxes);

    void trackBalls(cv::VideoCapture video, cv::Mat backgroundImage);
   
};



#endif