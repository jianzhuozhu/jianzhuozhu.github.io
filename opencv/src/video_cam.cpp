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
        imshow("video",frame);
        waitKey(30);
    }
    return 0;
}
