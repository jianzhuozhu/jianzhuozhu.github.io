#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
int main()
{
    Mat srcImage = imread("../1.jpg");
    imshow("raw",srcImage);
    Mat dstImage;
    Mat element = getStructuringElement(MORPH_RECT,Size(15,15));
    erode(srcImage,dstImage,element);
    imshow("cor",dstImage);
    waitKey(0); 
    return 0;
}
