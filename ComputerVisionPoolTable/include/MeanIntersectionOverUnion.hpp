// Main author: Simen Nesland
#ifndef MEANINTERSECTIONOVERUNION_HPP
#define MEANINTERSECTIONOVERUNION_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "BallDetector.hpp"
#include "TableDetector.hpp"
#include "BoundingBox.hpp"

class MeanIntersectionOverUnion {
    // // To calculate the mean average precision, run the following code in main.cpp:
    // // frame is the current frame from the video
    // // groundTruthPath is the path to the ground truth bounding boxes
    // // All the ball table and ball detection is done within the class
    // MeanAveragePrecision map;
    // double averagePrecision = map.averagePrecisionCalculation(frame, groundTruthPath);
    // std::cout << "Average Precision: " << averagePrecision << std::endl;
    public:
        MeanIntersectionOverUnion(cv::Mat frame, std::string groundTruthPath);
        double getAvgIoU();

    private:
        cv::Mat groundTruth;
        cv::Mat segmentedTable;
        std::vector<BoundingBox> segmentedBalls;
        cv::Mat allBallMasks;

        double backgroundScore = 0;
        double tableScore = 0;
        double whiteBallScore = 0;
        double blackBallScore = 0;
        double stripedBallScore = 0;
        double solidBallScore = 0;

        void setBallScores();
        void setTableAndBackgroundScore();

};

#endif