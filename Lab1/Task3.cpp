#include <opencv2/highgui.hpp>
#include <iostream>

void cancelChannel(cv::Mat pic, int channel){
    if(pic.channels() == 3){
        for(int i=0; i<mat.rows; i++){
            pic.row(i).reshape(1,mat.cols).col(channel - 1).setTo(Scalar(0));
        }
    }
}

int main(int argc, char** argv)
{
    if(argc < 2){
        std::cout << "Image filename must be provided" << std::endl;
        return 0;
    }
    cv::Mat img = cv::imread(argv[1]);

    if(img.data == NULL){
        std::cout << "File '" << argv[1] << "' not found" << std::endl;
        return 0;
    }

    int nChannels = img.channels();
    cancelChannel(img, 1)
    std::cout << nChannels << " channels of the image opened" << std::endl;

    cv::namedWindow("Example 1");
    cv::imshow("Example 1", img);
    char wait = cv::waitKey(0);

    std::cout << wait << std::endl;

    return 0;
}