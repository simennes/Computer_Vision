#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

void printPixelBGR(int event, int x, int y, int, void *param){
    if(event == cv::EVENT_LBUTTONDOWN){
        cv::Mat* image = static_cast<cv::Mat*>(param);
        cv::Vec3b pixel = image->at<cv::Vec3b>(y, x);
        std::cout << "BGR: (" << (int)pixel[0] << ", " << (int)pixel[1] << ", " << (int)pixel[2] << ")\n"; 
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
    cv::namedWindow(argv[1]);
    cv::imshow(argv[1], img);
    cv::setMouseCallback(argv[1], printPixelBGR, &img);
    cv::waitKey(0);



    return 0;
}