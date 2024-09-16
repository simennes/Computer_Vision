
#include <opencv2/opencv.hpp>
#include <iostream>

/*int openImg(int argc, char** argv)
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
    std::cout << nChannels << " channels of the image opened" << std::endl;

    cv::namedWindow("Example 1");
    cv::imshow("Example 1", img);
    char wait = cv::waitKey(0);

    std::cout << wait << std::endl;

    return 0;
}*/

int main(){
    std::cout << "Yoo" << std::endl;
    /*char* argv[2] = {"C:/Users/Simen/OneDrive - NTNU/FYSMAT/INDMAT/24V/Computer Vision/Lab1",
                     "Lena_grayscale.jpg"};
    const int argc = sizeof(argv)/sizeof(char*);
    openImg(argc, argv);*/
    return 0;
}