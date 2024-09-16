// Main author: Jan Kristian Alstergren
#include "BallTracker.hpp"

std::vector<cv::Rect> BallTracker::expandBoundingBoxes(std::vector<BoundingBox> inputBoxes) {
    std::vector<cv::Rect> expandedBoundingBoxes;
    for(const auto& box : inputBoxes) {
        cv::Rect expandedBox = cv::Rect(box.x - 12, box.y - 12, box.width + 24, box.height + 24);
        expandedBoundingBoxes.push_back(expandedBox);
    }
    return expandedBoundingBoxes;
}

void BallTracker::trackBalls(cv::VideoCapture video, cv::Mat backgroundImage) {

    cv::Mat frame;
    int frameWidth = video.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = video.get(cv::CAP_PROP_FRAME_HEIGHT);

    //cv::VideoWriter videoWriter("../output/videos/game4_clip2_out.avi", cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(frameWidth, frameHeight));
    
    video.read(frame);
    // Detect the pool table
    cv::Mat tableFrame;
    TableDetector tableDetector;
    tableFrame = tableDetector.detectTable(frame);
    std::cout << "Table corners" << tableDetector.tableCorners << std::endl;

    // Detect the balls and make bounding boxes
    BallDetector ballDetector;
    ballDetector.segmentBalls(tableFrame);

    TopViewVisual topView = TopViewVisual(tableDetector.tableCorners,
                                          backgroundImage, ballDetector.segmentedBalls.size());
    
    cv::Mat gaussianFrame;
    cv::GaussianBlur(tableFrame, gaussianFrame, cv::Size(11, 11), 2, 2);

    std::vector<cv::Rect> expandedBoundingBoxes = this->expandBoundingBoxes(ballDetector.segmentedBalls);
    cv::Ptr<cv::legacy::MultiTracker> multiTracker = cv::legacy::MultiTracker::create();


    if(expandedBoundingBoxes.size() < 1) {
        std::cout << "There are no bounding boxes" << std::endl;
        return;
    }

    for(const auto& box : expandedBoundingBoxes) {
        multiTracker->add(cv::legacy::TrackerBoosting::create(), gaussianFrame, box);
    }
    int i = 0;
    while(video.read(frame)) {
        topView.clearTopView();
        tableFrame = tableDetector.detectTable(frame);
        cv::GaussianBlur(tableFrame, gaussianFrame, cv::Size(11, 11), 2, 2);

        multiTracker->update(gaussianFrame);
        std::cout << "Updating frame: " << i << std::endl;
        i++;
        int j = 0;
        for(const auto& box : multiTracker->getObjects()) {
            if(ballDetector.segmentedBalls[j].ballType == 1) {
            cv::rectangle(gaussianFrame, box, cv::Scalar(255, 255, 255), 2, 1);
            topView.updateTopView(box, 1, frame);
            }
            else if(ballDetector.segmentedBalls[j].ballType == 2) {
            cv::rectangle(gaussianFrame, box, cv::Scalar(0, 0, 0), 2, 1);
            topView.updateTopView(box, 2, frame);
            }
            else if(ballDetector.segmentedBalls[j].ballType == 3) {
            cv::rectangle(gaussianFrame, box, cv::Scalar(255, 0, 0), 2, 1);
            topView.updateTopView(box, 3, frame);
            }
            else if(ballDetector.segmentedBalls[j].ballType == 4) {
            cv::rectangle(gaussianFrame, box, cv::Scalar(0, 255, 0), 2, 1);
            topView.updateTopView(box, 4, frame);
            }
            topView.updateBallTrajectories(box, j);
            j++;
        }
        topView.drawIntegratedTopView(frame);
        //cv::imshow("MultiTracker", gaussianFrame);
        //videoWriter.write(topView.getIntegratedTopView());
        
        if (cv::waitKey(75) == 'q') {
            break;
        }
    }
}