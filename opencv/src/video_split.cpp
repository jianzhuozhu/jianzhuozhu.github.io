#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
int main()
{
    VideoCapture capture;
    capture.open(0);
    while(1)
    {
        Mat frame;
        capture>>frame;
        Mat channels[3];
        Mat imageBlueChannel;
        Mat imageGreenChannel;
        Mat imageRedChannel;
        //通道分离
        split(frame,channels);
        imageBlueChannel = channels[0];
        imageGreenChannel = channels[1];
        imageRedChannel = channels[2];
        //腐蚀
        Mat element = getStructuringElement(MORPH_RECT,Size(15,15));
        erode(imageBlueChannel,imageBlueChannel,element);
        erode(imageGreenChannel,imageGreenChannel,element);
        erode(imageRedChannel,imageRedChannel,element);

        //均值滤波
        //blur(imageBlueChannel,imageBlueChannel,Size(3,3));
        //blur(imageGreenChannel,imageGreenChannel,Size(3,3));
        //blur(imageRedChannel,imageRedChannel,Size(3,3));
        
        //高斯滤波
        GaussianBlur(imageBlueChannel,imageBlueChannel,Size(9,9),0,0);
        GaussianBlur(imageGreenChannel,imageGreenChannel,Size(9,9),0,0);
        GaussianBlur(imageRedChannel,imageRedChannel,Size(9,9),0,0);


        //边沿检测
        //Canny(imageBlueChannel,imageBlueChannel,3,9,3);
        //Canny(imageGreenChannel,imageGreenChannel,3,9,3);
        //Canny(imageRedChannel,imageRedChannel,3,9,3);

        imshow("src",frame);
        imshow("blue",imageBlueChannel);
        imshow("green",imageGreenChannel);
        imshow("red",imageRedChannel);
        waitKey(30);


        
    }
    return 0;
}
