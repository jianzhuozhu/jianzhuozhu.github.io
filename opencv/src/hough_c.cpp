#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main() {
    while(1)
    {
    Mat src;
    // VideoCapture capture;
    // capture.open(0);//打开zed相机
    // capture.read(src);
    src = imread("/home/jianzhuozhu/catkin_ws_tools/src/img/houghcircle_1.png");
    namedWindow("origin");
    imshow("origin", src);
    //转出灰度图，并进行平滑,因为要进行边缘检测，最好进行平滑
    Mat gray;
    cvtColor(src, gray, CV_BGR2GRAY);
    imshow("gray", gray);;
    Mat dst;
    medianBlur(gray, dst, 5);;
    imshow("median filter", dst);
    vector<Vec3f> circles;
    HoughCircles(dst, circles, HOUGH_GRADIENT,1,120,100,30,0,0);
    for (size_t i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));  
        int radius = cvRound(circles[i][2]);
        //绘制圆心
        circle(src, center, 2, Scalar(0, 255, 0), 2);
        //绘制轮廓
        circle(src, center, radius, Scalar(0, 255, 0), 2);
    }
    imshow("result", src);
    waitKey(100);
    }
}
