#include "Imagefilters.h"
#include <iostream>
#include <vector>

void maxFilter(cv::Mat img, int kernelSize){
    if(kernelSize % 2 == 0){
        std::cout << "Kernel size must be an odd number" << std::endl;
    }

    cv::Mat temp = cv::Mat::zeros(cv::Size(img.cols,img.rows), CV_8UC1);
    int kernelSizeHalf = kernelSize / 2;

    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            uchar maxValue = 0;
            for(int k = -kernelSizeHalf; k <= kernelSizeHalf; k++){
                for(int l = -kernelSizeHalf; l <= kernelSizeHalf; l++){
                    if(i + k >= 0 && i + k < img.rows && j + l >= 0 && j + l < img.cols){
                        uchar currValue = img.at<uchar>(i+k, j+l);
                        if(currValue > maxValue){
                            maxValue = currValue;
                        }
                    }
                }
            }
        temp.at<uchar>(i,j) = maxValue;
        }
    }
    temp.copyTo(img);
}

void minFilter(cv::Mat img, int kernelSize){
    if(kernelSize % 2 == 0){
        std::cout << "Kernel size must be an odd number" << std::endl;
    }

    cv::Mat temp = cv::Mat::zeros(cv::Size(img.cols,img.rows), CV_8UC1);
    int kernelSizeHalf = kernelSize / 2;
    
    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            uchar minValue = 255;
            for(int k = -kernelSizeHalf; k <= kernelSizeHalf; k++){
                for(int l = -kernelSizeHalf; l <= kernelSizeHalf; l++){
                    if(i + k >= 0 && i + k < img.rows && j + l >= 0 && j + l < img.cols){
                        uchar currValue = img.at<uchar>(i+k, j+l);
                        if(currValue < minValue){
                            minValue = currValue;
                        }
                    }
                }
            }
        temp.at<uchar>(i,j) = minValue;
        }
    }
    temp.copyTo(img);
}

void medianFilter(cv::Mat img, int kernelSize){
    if(kernelSize % 2 == 0){
        std::cout << "Kernel size must be an odd number" << std::endl;
    }
    cv::Mat temp = cv::Mat::zeros(cv::Size(img.cols,img.rows), CV_8UC1);
    int kernelSizeHalf = kernelSize / 2;
    
    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            std::vector<uchar> values;
            for(int k = -kernelSizeHalf; k <= kernelSizeHalf; k++){
                for(int l = -kernelSizeHalf; l <= kernelSizeHalf; l++){
                    if(i + k >= 0 && i + k < img.rows && j + l >= 0 && j + l < img.cols){
                        values.push_back(img.at<uchar>(i+k, j+l));
                    }
                }
            }

        int len = values.size();
        uchar median;
        std::sort(values.begin(), values.end());

        if(len % 2 == 0){
            median = values[len/2];
        }
        else{
            median = (values[len/2] + values[(len/2) - 1])/2;
        }
        temp.at<uchar>(i,j) = median;
        }
    }
    temp.copyTo(img);
    
}

void gaussianSmoothing(cv::Mat img, int kernelSize){
    GaussianBlur(img, img, cv::Size(kernelSize, kernelSize), 0);
}