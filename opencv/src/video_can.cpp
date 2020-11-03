
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
        Mat edge,grayImage;
        cvtColor(frame,grayImage,CV_BGR2GRAY);
        blur(grayImage,edge,Size(3,3));
        Canny(edge,edge,10,20,3);
        imshow("edge",edge);
        waitKey(30);
    }
    return 0;
}