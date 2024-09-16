#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>



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
    cv::waitKey(0);


    return 0;
}