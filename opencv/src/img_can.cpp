#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
int main()
{
    Mat srcImage = imread("/home/jianzhuozhu/catkin_ws_tools/src/img/canny_1.png");
    imshow("raw",srcImage);
    Mat edge,grayImage;
    cvtColor(srcImage,grayImage,CV_BGR2GRAY);
    blur(grayImage,edge,Size(3,3));
    Canny(edge,edge,3,9,3);
    imshow("edge",edge);
    waitKey(0);
    return 0;
}