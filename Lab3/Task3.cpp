#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <numeric>

void printAvgPixelBGR(int event, int x, int y, int, void *param){
    if(event == cv::EVENT_LBUTTONDOWN){
        cv::Mat* img = static_cast<cv::Mat*>(param);

        std::vector<int> BVals;
        std::vector<int> GVals;
        std::vector<int> RVals;
        for(int i = -4; i <= 4; i++){
            for(int j = -4; j <= 4; j++){
                if(x + i >= 0 && x + i < img->cols && y + j >= 0 && y + j < img->rows){
                    cv::Vec3b pixel = img->at<cv::Vec3b >(j+y, i+x);
                    BVals.push_back((int)pixel[0]);
                    GVals.push_back((int)pixel[1]);
                    RVals.push_back((int)pixel[2]);
                }
            }
        }
        int len = BVals.size();

        std::cout << "BGR: (" 
                  << std::reduce(BVals.begin(), BVals.end())/len << ", " 
                  << std::reduce(GVals.begin(), GVals.end())/len << ", " 
                  << std::reduce(RVals.begin(), RVals.end())/len 
                  << ")\n"; 
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
    cv::setMouseCallback(argv[1], printAvgPixelBGR, &img);
    cv::waitKey(0);



    return 0;
}