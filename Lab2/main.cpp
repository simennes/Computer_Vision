#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Imagefilters.h"




int main(int argc, char** argv){

    std::cout << "argc: " << argc << std::endl;

    if(argc >= 2){

        // Task 1
        std::string filename = argv[1];
        cv::Mat img = cv::imread(filename);
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

        int ind = filename.find(".");
        std::string format = filename.substr(ind, filename.size());
        std::string filename_gray = filename.substr(0, ind) + "_grayscale" + format;

        cv::imwrite(filename_gray, img);

        // Task 2-4

        if(filename.substr(7, 9) == "Astronaut"){
            cv::Mat astronaut = cv::imread("Images/Astronaut_original_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat astronautMax = cv::imread("Images/Astronaut_salt_pepper_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat astronautMin = cv::imread("Images/Astronaut_salt_pepper_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat astronautMed = cv::imread("Images/Astronaut_salt_pepper_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat astronautGau = cv::imread("Images/Astronaut_salt_pepper_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat astronautSP = cv::imread("Images/Astronaut_salt_pepper_grayscale.png", cv::IMREAD_GRAYSCALE);
            maxFilter(astronautMax, 3);
            minFilter(astronautMin, 3);
            medianFilter(astronautMed, 5);
            gaussianSmoothing(astronautGau, 5);

            cv::imwrite("Images/Astronaut_max.png", astronautMax);
            cv::imwrite("Images/Astronaut_min.png", astronautMin);
            cv::imwrite("Images/Astronaut_med.png", astronautMed);
            cv::imwrite("Images/Astronaut_gau.png", astronautGau);

            cv::namedWindow("Astronaut");
            cv::imshow("Astronaut", astronaut);

            cv::namedWindow("Astronaut salt pepper");
            cv::imshow("Astronaut salt pepper", astronautSP);

            cv::namedWindow("Astronaut max");
            cv::imshow("Astronaut max", astronautMax);

            cv::namedWindow("Astronaut min");
            cv::imshow("Astronaut min", astronautMin);

            cv::namedWindow("Astronaut median");
            cv::imshow("Astronaut median", astronautMed);

            cv::namedWindow("Astronaut gaussian");
            cv::imshow("Astronaut gaussian", astronautGau);

            cv::waitKey(0);
        }

        else if(filename.substr(7, 4) == "Lena"){
            cv::Mat lena = cv::imread("Images/Lena_original_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat lenaMax = cv::imread("Images/Lena_corrupted_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat lenaMin = cv::imread("Images/Lena_corrupted_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat lenaMed = cv::imread("Images/Lena_corrupted_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat lenaCorr = cv::imread("Images/Lena_corrupted_grayscale.png", cv::IMREAD_GRAYSCALE);
            cv::Mat lenaGau = cv::imread("Images/Lena_corrupted_grayscale.png", cv::IMREAD_GRAYSCALE);
            maxFilter(lenaMax, 3);
            minFilter(lenaMin, 3);
            medianFilter(lenaMed, 5);
            gaussianSmoothing(lenaGau, 7);

            cv::imwrite("Images/Lena_max.png", lenaMax);
            cv::imwrite("Images/Lena_min.png", lenaMin);
            cv::imwrite("Images/Lena_med.png", lenaMed);
            cv::imwrite("Images/Lena_gau.png", lenaGau);

            cv::namedWindow("Lena");
            cv::imshow("Lena", lena);

            cv::namedWindow("Lena salt pepper");
            cv::imshow("Lena salt pepper", lenaCorr);

            cv::namedWindow("Lena max");
            cv::imshow("Lena max", lenaMax);

            cv::namedWindow("Lena min");
            cv::imshow("Lena min", lenaMin);

            cv::namedWindow("Lena median");
            cv::imshow("Lena median", lenaMed);

            cv::namedWindow("Lena gaussian");
            cv::imshow("Lena gaussian", lenaGau);

            cv::waitKey(0);
        }
        else if(filename.substr(7, 6) == "Garden"){
            cv::Mat garden = cv::imread("Images/Garden_grayscale.jpg", cv::IMREAD_GRAYSCALE);
            cv::Mat gardenMax = cv::imread("Images/Garden_grayscale.jpg", cv::IMREAD_GRAYSCALE);
            cv::Mat gardenMed = cv::imread("Images/Garden_grayscale.jpg", cv::IMREAD_GRAYSCALE);
            cv::Mat gardenGau = cv::imread("Images/Garden_grayscale.jpg", cv::IMREAD_GRAYSCALE);
        
            maxFilter(gardenMax, 5);
            medianFilter(gardenMed, 5);
            gaussianSmoothing(gardenGau, 3);

            cv::imwrite("Images/Garden_max.png", gardenMax);
            cv::imwrite("Images/Garden_med.png", gardenMed);
            cv::imwrite("Images/Garden_gau.png", gardenGau);

            cv::namedWindow("Garden");
            cv::imshow("Garden", garden);

            cv::namedWindow("Garden max");
            cv::imshow("Garden max", gardenMax);

            cv::namedWindow("Garden median");
            cv::imshow("Garden median", gardenMed);

            cv::namedWindow("Garden gaussian");
            cv::imshow("Garden gaussian", gardenGau);

            cv::waitKey(0);
        }
    }


    else{

        // Task 5-6

        cv::Mat garden = cv::imread("Images/Garden_grayscale.jpg");

        cv::Mat histOut;
        float range[] {0, 256};
        const float* histRange = {range};
        // Change number of bins below
        const int histSize = 64;
        const int channelNum = 1;
        double maxVal = 0;

        cv::calcHist(&garden, 1, &channelNum, cv::Mat(), histOut, 1, &histSize, &histRange);
        cv::minMaxLoc(histOut, 0, &maxVal);

        // x to y scale (e.g. 3 gives a 3:1 image)
        int scale = 1;
        // width = widthScale * histSize (e.g. 2 * 256)
        int widthScale = 8;
        int maxHeight = widthScale*histSize/scale;

        cv::Mat histImg(histSize*scale*widthScale, histSize*widthScale, CV_8UC1, cv::Scalar(200)); 
        for(int i = 0; i < histSize; i++){
            int height = (int)histOut.at<float>(0,i);
            cv::rectangle(histImg,
                        cv::Point(i*scale*widthScale, maxHeight - height*maxHeight/maxVal),
                        cv::Point((i+1)*scale*widthScale, maxHeight),
                        0,-1);
        }
        cv::namedWindow("Histogram");
        cv::imshow("Histogram", histImg);

        cv::namedWindow("Garden");
        cv::imshow("Garden", garden);

        cv::Mat gardenEq = cv::imread("Images/Garden_grayscale.jpg", cv::IMREAD_GRAYSCALE);
        cv::equalizeHist(gardenEq, gardenEq);
        cv::namedWindow("Garden equalized");
        cv::imshow("Garden equalized", gardenEq);
        cv::waitKey(0);

    }

    return 0;
}