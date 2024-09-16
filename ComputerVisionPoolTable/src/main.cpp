// Main author: Jan Kristian Alstergren
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

#include "../include/TableDetector.hpp"
#include "../include/BallDetector.hpp"
#include "../include/BallTracker.hpp"
#include "../include/MeanAveragePrecision.hpp"
#include "../include/MeanIntersectionOverUnion.hpp"

int main(int argc, char** argv) {

    if(argc != 3) {
        std::cout << "Usage: " << argv[0] << " <videoDirectory path> " << "<groundTruthBboxesPath>" << std::endl;
        return -1;
    }

    cv::Mat backGroundImage = cv::imread("../include/2D_visualization/table_background.png", cv::IMREAD_COLOR);

    std::string videoDirectory = argv[1];
    std::string groundTruthPath = argv[2];

    cv::VideoCapture video(videoDirectory);

    if(!video.isOpened()){
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    //##############################################

    std::cout << "Choose what you want to see: " << std::endl;
    std::cout << "1: Ball tracker" << std::endl;
    std::cout << "2: Mean Average Precision" << std::endl;
    std::cout << "3: Mean Intersection Over Union" << std::endl;
    std::cout << "4: Exit" << std::endl;

    int choice;
    std::cin >> choice;

    //##############################################

    switch(choice) {
        case 1: {

            BallTracker ballTracker;
            ballTracker.trackBalls(video, backGroundImage);
            break;
        }
        case 2: {
            cv::VideoCapture video(videoDirectory);

            if(!video.isOpened()){
                std::cout << "Error opening video stream or file" << std::endl;
                return -1;
            }

            std::string lastChars = groundTruthPath.substr(groundTruthPath.size() - 14);
            if(lastChars == "first_bbox.txt") {
                video.set(cv::CAP_PROP_POS_FRAMES, 0);
            } else {
                video.set(cv::CAP_PROP_POS_FRAMES, video.get(cv::CAP_PROP_FRAME_COUNT) - 1);
            }

            cv::Mat frame;
            video.read(frame);

            MeanAveragePrecision  map;
            double averagePrecision = map.averagePrecisionCalculation(frame, groundTruthPath);
            std::cout << "Average Precision: " << averagePrecision << std::endl;
            break;
        }
        case 3: {

            std::string lastChars = groundTruthPath.substr(groundTruthPath.size() - 9);
            std::cout << lastChars << std::endl;
            if(lastChars == "first.png") {
                video.set(cv::CAP_PROP_POS_FRAMES, 0);
            } else {
                video.set(cv::CAP_PROP_POS_FRAMES, video.get(cv::CAP_PROP_FRAME_COUNT) - 1);
            }

            cv::Mat frame;
            video.read(frame);
            cv::imshow("Frame", frame);
            cv::waitKey(0);

            MeanIntersectionOverUnion  miou(frame, groundTruthPath);
            double avgIoU = miou.getAvgIoU();
            std::cout << "Average IoU: " << avgIoU << std::endl;
            break;
        }
        case 4: {
            return 0;
        }
        default: {
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }


    return 0;
}