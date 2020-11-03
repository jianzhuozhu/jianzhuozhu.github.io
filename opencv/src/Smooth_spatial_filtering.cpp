#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
int main()
{
    Mat srcImage = imread("../1.jpg");
    Mat dstImage;
    imshow("raw",srcImage);

    blur(srcImage,dstImage,Size(7,7));

    imshow("dstImage",dstImage);
    waitKey(0);
    return 0;
}