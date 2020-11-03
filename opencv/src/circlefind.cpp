
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//图像文件名strImgName, canny的低阈值dThreshold1, canny的高阈值dThreshold2, canny的核大小iSize, 筛选圆的条件：圆的最小面积iMinArea, 圆的最小面积int iMaxArea
void findCircles(double dThreshold1, double dThreshold2, int iSize, int iMinArea, int iMaxArea)    
{    
Mat q_MatImage;  
Mat q_MatImageGray;  
Mat q_MatImageShow;  
q_MatImage=imread("/media/jianzhuozhu/7F1F-0B38/coin.bmp");//读入一张图片  
imshow("q_MatImage",q_MatImage);
q_MatImage.copyTo(q_MatImageShow);  
cvtColor(q_MatImage,q_MatImageGray,CV_RGB2GRAY);  
//Canny找边界，也可以阈值二值化
Mat cannyEdge;
Canny(q_MatImageGray, cannyEdge, dThreshold1, dThreshold2, iSize);
imshow("cannyEdge",cannyEdge);
//找轮廓
vector<vector<Point>> q_vPointContours;  
findContours(cannyEdge, q_vPointContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE,Point(0,0));  
//drawContours(q_MatImageShow, q_vPointContours, -1, Scalar(0,255,0));//显示轮廓
//筛选目标轮廓点
vector<Point> vfindContours;
size_t q_iAmountContours = q_vPointContours.size();  
size_t iIndex = 0;
for ( iIndex = 0; iIndex < q_iAmountContours; iIndex++)  
{ 
//根据圆的面积判断是否为目标圆
double ddarea = contourArea(q_vPointContours[iIndex]);
if((iMinArea < ddarea) && (iMaxArea > ddarea)) 
{
break;
}
}
//存储目标圆的轮廓点
size_t findCount = q_vPointContours[iIndex].size();
for(int i=0; i<findCount; i++)
vfindContours.push_back(q_vPointContours[iIndex][i]);
 
//采用椭圆拟合来得到圆
RotatedRect rectElli = fitEllipse(vfindContours);
float fR = MIN(rectElli.size.width , rectElli.size.height);// 是否为圆，可以比较这两个值，若十分接近或相等，就是一个正圆
cout << "fitEllipse 中心: " <<  rectElli.center.x << ", " <<rectElli.center.y << "  半径:"<<fR/2<< endl;  
circle(q_MatImageShow, Point(rectElli.center), fR/2, Scalar(0,0,255), 2);//圆周
circle(q_MatImageShow, Point(rectElli.center), 5, Scalar(0,0,255), 3);//圆心
 
namedWindow("Test");        //创建一个名为Test窗口  
imshow("Test",q_MatImageShow);//窗口中显示图像  
waitKey();        
}  


int main(int argc, char** argv)
{
    const char* strImgName = "/media/jianzhuozhu/7F1F-0B38/coin.bmp";
    findCircles(1, 100, 3, 1000, 1000000);
    

    return 0;
    }