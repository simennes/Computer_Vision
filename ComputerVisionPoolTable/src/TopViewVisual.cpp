// Main author: Simen Nesland
#include "../include/TopViewVisual.hpp"

// A map that given boundingbox type gives the corresponding color
std::map<int, cv::Scalar> colorMap = {
    {1, cv::Scalar(255, 255, 255)},
    {2, cv::Scalar(0, 0, 0)},
    {3, cv::Scalar(92, 184, 255)},
    {4, cv::Scalar(255, 167, 33)}
};


TopViewVisual::TopViewVisual(std::vector<cv::Point> _tableCorners, cv::Mat _backgroundImage, int numBalls) {
    tableCorners = _tableCorners;
    backgroundImage = _backgroundImage;
    topView = cv::Mat::zeros(backgroundImage.size(), CV_8UC3);

    setTransformMat();

    // binary image for each ball
    for(int i = 0; i < numBalls; i++) {
        ballTrajectories.push_back(cv::Mat::zeros(backgroundImage.size(), CV_8U));
    }
}

void TopViewVisual::setTransformMat() {
    cv::Point2f src[4];
    cv::Point2f dst[4];

    // see if x or y distance in src is bigger to determine which is the longest side
    // this is to make sure the perspective transform is correct
    float xDistTop = std::sqrt(std::pow(tableCorners[1].x - tableCorners[0].x, 2) + std::pow(tableCorners[1].y - tableCorners[0].y, 2));
    float yDistLeft = std::sqrt(std::pow(tableCorners[3].x - tableCorners[0].x, 2) + std::pow(tableCorners[3].y - tableCorners[0].y, 2));
    float xDistBottom = std::sqrt(std::pow(tableCorners[2].x - tableCorners[3].x, 2) + std::pow(tableCorners[2].y - tableCorners[3].y, 2));
    float yDistRight = std::sqrt(std::pow(tableCorners[2].x - tableCorners[1].x, 2) + std::pow(tableCorners[2].y - tableCorners[1].y, 2));
    for(int i = 0; i < 4; i++) {
        src[i] = tableCorners[i];
    }


    if(xDistTop > yDistLeft && xDistTop/xDistBottom > 0.9) {
        dst[3] = cv::Point2f(58, 435);
        dst[0] = cv::Point2f(58, 37);
        dst[1] = cv::Point2f(755, 46);
        dst[2] = cv::Point2f(755, 428);
    } else {
        dst[0] = cv::Point2f(58, 435);
        dst[1] = cv::Point2f(58, 37);
        dst[2] = cv::Point2f(755, 46);
        dst[3] = cv::Point2f(755, 428);
    }

    transformMat = cv::getPerspectiveTransform(src, dst);
}

void TopViewVisual::updateTopView(cv::Rect2d box, int ballType, cv::Mat frame) {

    cv::Point2f center(box.x + (int)box.width/2,
                        box.y + (int)box.height/2);
    std::vector<cv::Point2f> centerVec { center };
    cv::perspectiveTransform(centerVec, centerVec, transformMat);

    cv::circle(topView, centerVec[0], 15, colorMap[ballType], -1);
    cv::circle(topView, centerVec[0], 15, cv::Scalar(0, 0, 0), 2);
}

void TopViewVisual::drawTopView() {
    cv::imshow("Top view", topView);
}

void TopViewVisual::clearTopView() {
    topView = backgroundImage.clone();
}


void TopViewVisual::drawIntegratedTopView(cv::Mat frame) {
    cv::Mat integratedFrame = frame.clone();

    int height = integratedFrame.rows/3;
    int width = integratedFrame.cols * height / integratedFrame.rows;

    cv::Mat topViewResized;
    cv::resize(topView, topViewResized, cv::Size(width, height));

    // set bottom left corner of integratedFrame to topViewResized
    cv::Mat roi = integratedFrame(cv::Rect(0, integratedFrame.rows - height, width, height));
    topViewResized.copyTo(roi);
    this->integratedTopView = integratedFrame.clone();
    cv::imshow("Integrated top view", integratedFrame);
}

void TopViewVisual::updateBallTrajectories(cv::Rect2d box, int ballIndex) {
    cv::Point2f center(box.x + (int)box.width/2,
                        box.y + (int)box.height/2);
    std::vector<cv::Point2f> centerVec { center };
    cv::perspectiveTransform(centerVec, centerVec, transformMat);

    cv::circle(ballTrajectories[ballIndex], centerVec[0], 3, cv::Scalar(255), -1);


    if(cv::sum(ballTrajectories[ballIndex])[0] > 15) {
        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(ballTrajectories[ballIndex], lines, 1, CV_PI/180, 30, 10, 20);
        for( size_t i = 0; i < lines.size(); i++ ) {
            cv::Vec4i l = lines[i];
            cv::line(topView, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
        }
    }
}