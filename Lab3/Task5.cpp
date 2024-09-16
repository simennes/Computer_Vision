#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <numeric>

const int threshold = 6;

int getAvgPixelH(int x, int y, cv::Mat *img){
    std::vector<int> HVals;
    for(int i = -4; i <= 4; i++){
        for(int j = -4; j <= 4; j++){
            if(x + i >= 0 && x + i < img->cols && y + j >= 0 && y + j < img->rows){
                cv::Vec3b pixel = img->at<cv::Vec3b >(j+y, i+x);
                HVals.push_back((int)pixel[0]);
            }
        }
    }
    int len = HVals.size();

    return std::reduce(HVals.begin(), HVals.end())/len;

}

void createMask(int event, int x, int y, int, void *param){
    if(event == cv::EVENT_LBUTTONDOWN){
        cv::Mat* img = static_cast<cv::Mat*>(param);
        int avgVal = getAvgPixelH(x, y, img);

        cv::Mat mask = cv::Mat::zeros(cv::Size(img->cols,img->rows),CV_8UC1);
        for(int i = 0; i < img->cols; i++){
            for(int j = 0; j < img->rows; j++){
                cv::Vec3b pixel = img->at<cv::Vec3b>(j, i);
                if(abs(pixel[0] - avgVal) <= threshold){
                    mask.at<uchar>(j, i) = 255;
                }
            }
        }
        cv::namedWindow("Mask");
        cv::imshow("Mask", mask);
    }
}



int main(int argc, char** argv){

    if(argc < 2){
        std::cout << "Image filename must be provided" << std::endl;
        return 0;
    }
    cv::Mat img = cv::imread(argv[1]);

    if(img.data == NULL){
        std::cout << "File '" << argv[1] << "' not found" << std::endl;
        return 0;
    }
    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    cv::namedWindow(argv[1]);
    cv::imshow(argv[1], img);
    cv::setMouseCallback(argv[1], createMask, &imgHSV);
    cv::waitKey(0);

    return 0;
}