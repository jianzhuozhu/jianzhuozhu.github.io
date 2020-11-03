#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
int main()
{

    Mat srcImage = imread("../2.jpg");
    imshow("raw",srcImage);
    Mat channels[3];
    Mat imageBlueChannel;
    Mat imageGreenChannel;
    Mat imageRedChannel;
    split(srcImage,channels);
    imageBlueChannel = channels[0];
    imageGreenChannel = channels[1];
    imageRedChannel = channels[2];
    imshow("blue",imageBlueChannel);
    imshow("green",imageGreenChannel);
    imshow("red",imageRedChannel);
    waitKey(0); 
    return 0;
}