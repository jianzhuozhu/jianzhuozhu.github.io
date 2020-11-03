#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int thre, minLineLength, maxLineGap;
Mat src, dst;
void on_HoughLines(int, void *);
int main() {
    Mat ori = imread("../2.jpg");
    namedWindow("dst");
    imshow("origin", ori);
    Canny(ori, src, 50, 200,3);
    cvtColor(src, dst, CV_GRAY2BGR);
    imshow("canny image", src);
    thre = 80;
    minLineLength = 50;
    maxLineGap=10;
    createTrackbar("threshold:", "dst", &thre, 300, on_HoughLines);
    createTrackbar("minLine  :", "dst", &minLineLength, 300, on_HoughLines);
    createTrackbar("maxGap   :", "dst", &maxLineGap, 20, on_HoughLines);

    on_HoughLines(thre, 0);
    on_HoughLines(minLineLength, 0);
    on_HoughLines(maxLineGap, 0);
    /*HoughLinesP(src, lines, 1, CV_PI / 180, 50, 60, 10);
    for (size_t i = 0; i < lines.size(); i++) {
        Vec4i l = lines[i];
        line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88, 255));
    }*/
    //imshow("dst image", dst);
    waitKey(0);
}
//
void on_HoughLines(int, void *) {
    vector<Vec4i> lines;
    Mat dstImage = dst.clone();
    Mat srcImage = src.clone();
    HoughLinesP(srcImage, lines, 1, CV_PI / 180, thre, minLineLength, maxLineGap);
    for (size_t i = 0; i < lines.size(); i++) {
        Vec4i l = lines[i];
        line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255),2);
    }
    imshow("dst", dstImage);
}
