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
        if(frame.empty())
        {
            break;
        }
        Mat rangeImage;
        inRange(frame,Scalar(0,0,0),Scalar(180,255,46),rangeImage);
        imshow("rangeImage",rangeImage);
        //Mat edge,grayImage;
        //cvtColor(frame,grayImage,CV_BGR2GRAY);

        waitKey(30);
    }
    return 0;
}