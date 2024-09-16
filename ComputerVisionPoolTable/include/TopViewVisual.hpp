// Main author: Simen Nesland
#ifndef TOPVIEWVISUAL_HPP
#define TOPVIEWVISUAL_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include "BoundingBox.hpp"

class TopViewVisual {
    
    public:
        TopViewVisual(std::vector<cv::Point> tableCorners, cv::Mat backgroundImage, int numBalls);

        void updateTopView(cv::Rect2d box, int ballType, cv::Mat frame);
        void drawTopView();
        void clearTopView();
        void updateBallTrajectories(cv::Rect2d box, int ballIndex);

        void drawIntegratedTopView(cv::Mat frame);
        cv::Mat getIntegratedTopView() { return integratedTopView; }


    private:
        cv::Mat topView;
        cv::Mat transformMat;
        std::vector<cv::Point> tableCorners;
        cv::Mat backgroundImage;
        cv::Mat integratedTopView;

        std::vector<cv::Mat> ballTrajectories;

        void setTransformMat();
};


#endif