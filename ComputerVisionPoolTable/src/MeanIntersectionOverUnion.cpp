//Main author: Simen Nesland
#include "../include/MeanIntersectionOverUnion.hpp"

MeanIntersectionOverUnion::MeanIntersectionOverUnion(cv::Mat frame, std::string groundTruthPath) {
    groundTruth = cv::imread(groundTruthPath);
    if(groundTruth.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return;
    }

    TableDetector tableDetector;
    segmentedTable = tableDetector.detectTable(frame);

    BallDetector ballDetector;
    ballDetector.segmentBalls(segmentedTable);
    segmentedBalls = ballDetector.segmentedBalls;

    setBallScores();
    setTableAndBackgroundScore();

}

void MeanIntersectionOverUnion::setBallScores() {
    allBallMasks = cv::Mat::zeros(groundTruth.size(), CV_8U);

    for(int i = 0; i < segmentedBalls.size(); i++) {

        BoundingBox ball = segmentedBalls[i];
        cv::Mat ballMask = cv::Mat::zeros(groundTruth.size(), CV_8U);
        cv::Rect2d ballRect(ball.x, ball.y, ball.width, ball.height);
        cv::rectangle(ballMask, ballRect, cv::Scalar(255), -1);
        
        for(int i = 0; i < ballMask.rows; i++) {
            for(int j = 0; j < ballMask.cols; j++) {
                if(ballMask.at<uchar>(i, j) == 255) {
                    allBallMasks.at<uchar>(i, j) = 255;
                }
            }
        }

        cv::Mat ballGroundTruth = cv::Mat::zeros(groundTruth.size(), CV_8U);
        for(int i = 0; i < groundTruth.rows; i++) {
            for(int j = 0; j < groundTruth.cols; j++) {
                if(groundTruth.at<uchar>(i, j) == ball.ballType) {
                    ballGroundTruth.at<uchar>(i, j) = 255;
                }
            }
        }

        cv::Mat intersection;
        cv::bitwise_and(ballGroundTruth, ballMask, intersection);
        cv::Mat unionMat;
        cv::bitwise_or(ballGroundTruth, ballMask, unionMat);

        double intersectionArea = cv::countNonZero(intersection);
        double unionArea = cv::countNonZero(unionMat);
        double iou = intersectionArea/unionArea;

        if(ball.ballType == 1) {
            whiteBallScore += iou;
        } else if(ball.ballType == 2) {
            blackBallScore += iou;
        } else if(ball.ballType == 3) {
            stripedBallScore += iou;
        } else if(ball.ballType == 4) {
            solidBallScore += iou;
        }

    }


}

void MeanIntersectionOverUnion::setTableAndBackgroundScore() {
    cv::Mat tableMask = cv::Mat::zeros(groundTruth.size(), CV_8U);
    cv::Mat backgroundMask = cv::Mat::zeros(groundTruth.size(), CV_8U);

    for(int i = 0; i < segmentedTable.rows; i++) {
        for(int j = 0; j < segmentedTable.cols; j++) {
            if(segmentedTable.at<cv::Vec3b>(i, j) != cv::Vec3b(0, 0, 0)) {
                tableMask.at<uchar>(i, j) = 255;
            }
            else {
                backgroundMask.at<uchar>(i, j) = 255;
            }
        }
    }

    //do : tableMask -= allBallMasks;
    for(int i = 0; i < tableMask.rows; i++) {
        for(int j = 0; j < tableMask.cols; j++) {
            if(allBallMasks.at<uchar>(i, j) == 255) {
                tableMask.at<uchar>(i, j) = 0;
            }
        }
    }

    cv::Mat tableGroundTruth = cv::Mat::zeros(groundTruth.size(), CV_8U);
    for(int i = 0; i < groundTruth.rows; i++) {
        for(int j = 0; j < groundTruth.cols; j++) {
            if(groundTruth.at<uchar>(i, j) == 5) {
                tableGroundTruth.at<uchar>(i, j) = 255;
            }
        }
    }

    cv::Mat backgroundGroundTruth = cv::Mat::zeros(groundTruth.size(), CV_8U);
    for(int i = 0; i < groundTruth.rows; i++) {
        for(int j = 0; j < groundTruth.cols; j++) {
            if(groundTruth.at<uchar>(i, j) == 0) {
                backgroundGroundTruth.at<uchar>(i, j) = 255;
            }
        }
    }

    cv::Mat intersection;
    cv::bitwise_and(backgroundGroundTruth, backgroundMask, intersection);
    cv::Mat unionMat;
    cv::bitwise_or(backgroundGroundTruth, backgroundMask, unionMat);

    double intersectionArea = cv::countNonZero(intersection);
    double unionArea = cv::countNonZero(unionMat);
    double iou = intersectionArea/unionArea;
    backgroundScore = iou;

    cv::Mat tableIntersection;
    cv::bitwise_and(tableGroundTruth, tableMask, tableIntersection);
    cv::Mat tableUnionMat;
    cv::bitwise_or(tableGroundTruth, tableMask, tableUnionMat);

    double tableIntersectionArea = cv::countNonZero(tableIntersection);
    double tableUnionArea = cv::countNonZero(tableUnionMat);
    double tableIoU = tableIntersectionArea/tableUnionArea;
    tableScore = tableIoU;
}

double MeanIntersectionOverUnion::getAvgIoU() {
    double avgIoU = (backgroundScore + tableScore + whiteBallScore + blackBallScore + stripedBallScore + solidBallScore)/6;
    return avgIoU;
}