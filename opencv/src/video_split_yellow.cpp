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
        Mat frIn = frame(cv::Rect(0, 0, frame.cols / 2, frame.rows));//截取左目图片
        if(frame.empty())
        {
            printf("摄像头没有正常打开，重新插拔工控机上当摄像头\n");
            break;
        }
        //高斯滤波
        GaussianBlur(frIn,frIn,Size(3,3),0,0);
        Mat imageYellowChannel;
        //通道分离
        inRange(frIn,Scalar(26,43,46),Scalar(34,255,255),imageYellowChannel);
        //腐蚀
        //Mat element = getStructuringElement(MORPH_RECT,Size(15,15));
        //erode(imageBlueChannel,imageBlueChannel,element);

        //均值滤波
        //blur(imageBlueChannel,imageBlueChannel,Size(3,3));
        
        //边沿检测
        //Canny(imageBlueChannel,imageBlueChannel,3,9,3);
        
        imshow("src",frIn);
        imshow("yellow",imageYellowChannel);
        waitKey(30);


        
    }
    return 0;
}
