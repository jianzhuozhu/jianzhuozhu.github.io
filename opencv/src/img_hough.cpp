#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{   
    

    int i;
    Mat frame;
    Mat canny;
    Mat dstframe;
    frame = imread("../2.jpg");
	imshow("video",frame);
	Canny(frame,canny,50,200,3);
        imshow("canny",canny);
	cvtColor(canny,dstframe,CV_GRAY2BGR);
        imshow("dstframe",dstframe);
        

	vector<Vec2f> lines;
	HoughLines(canny,lines,1,CV_PI/180,150,0,0);
        printf("%d\n",lines.size());
#if 1
	for(i = 0;i < lines.size();i++)
	{
	    float rho = lines[i][0],theta = lines[i][1];
	    Point pt1,pt2;
	    double a = cos(theta),b = sin(theta);
	    double x0 = rho*a,y0 = rho*b;
	    pt1.x = cvRound(x0 + 1000*(-b));
	    pt1.y = cvRound(y0 + 1000*(a));
	    pt2.x = cvRound(x0 - 1000*(-b));
	    pt2.y = cvRound(y0 - 1000*(a));
	    line(dstframe,pt1,pt2,Scalar(55,100,195),1,LINE_AA);
	}
	imshow("hough",dstframe);
#endif
    waitKey(0);
    return 0;
}
