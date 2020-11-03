#include <opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<ctime>

using namespace cv;
using namespace std;
typedef unsigned char BYTE;
void TemplateMatch(Mat * pTo, Mat * pTemplate, Mat * src);
class Ve {
public:
    vector<Mat> ReadImage(cv::String pattern);
};

vector<Mat> Ve::ReadImage(cv::String pattern) 
{
    vector<cv::String> fn;
    glob(pattern, fn, false);
    vector<Mat> images;
    size_t count = fn.size(); //number of png files in images folder
    for (int i = 0; i < count; i++)
    {
        images.emplace_back(cv::imread(fn[i]));
        imshow("img", imread(fn[i]));

        Mat src = imread(fn[i]);
        Mat Template = imread("/home/jianzhuozhu/jianzhuozhu/opencv/pattern.bmp", 0);
        Mat pt;
        pt=src.clone();
        TemplateMatch(&pt, &Template, &src);
        imwrite("/home/jianzhuozhu/jianzhuozhu/opencv/img/P.jpg", pt);
        string old_str = "/home/jianzhuozhu/jianzhuozhu/opencv/img/P.jpg";
        string str1 = "/home/jianzhuozhu/jianzhuozhu/opencv/img/";
        char name[20];
        sprintf(name,"img%d",i);
  
        string str2 = name;
        string str = str1 + str2 + ".jpg";
        cout<<str<<endl;
        rename(old_str.c_str(), str.c_str());
        cout<<i<<endl;
    }
    return images;
}

void TemplateMatch(Mat * pTo, Mat * pTemplate, Mat * src)
{

    //循环变量
    int i, j, m, n;

    double dSumT; //模板元素的平方和
    double dSumS; //图像子区域元素的平方和
    double dSumST; //图像子区域和模板的点积    

                   //响应值
    double R;

    //记录当前的最大响应
    double MaxR;

    //最大响应出现位置
    int nMaxX;
    int nMaxY;

    int nHeight = src->rows;
    int nWidth = src->cols;
    //模板的高、宽
    int nTplHeight = pTemplate->rows;
    int nTplWidth = pTemplate->cols;
    //计算 dSumT
    dSumT = 0;
    for (m = 0; m < nTplHeight; m++)
    {
        for (n = 0; n < nTplWidth; n++)
        {
            // 模板图像第m行，第n个象素的灰度值
            int nGray =*pTemplate->ptr(m, n);

            dSumT += (double)nGray*nGray;
        }
    }

    //找到图像中最大响应的出现位置
    MaxR = 0;
    for (i = 0; i < nHeight - nTplHeight + 1; i++)
    {
        for (j = 0; j < nWidth - nTplWidth + 1; j++)
        {
            dSumST = 0;
            dSumS = 0;

            for (m = 0; m < nTplHeight; m++)
            {
                for (n = 0; n < nTplWidth; n++)
                {
                    // 原图像第i+m行，第j+n列象素的灰度值
                    int nGraySrc = *src->ptr(i + m, j + n);

                    // 模板图像第m行，第n个象素的灰度值
                    int nGrayTpl = *pTemplate->ptr(m, n);

                    dSumS += (double)nGraySrc*nGraySrc;
                    dSumST += (double)nGraySrc*nGrayTpl;
                }
            }

            R = dSumST / (sqrt(dSumS)*sqrt(dSumT));//计算相关响应

            //与最大相似性比较
            if (R > MaxR)
            {
                MaxR = R;
                nMaxX = j;
                nMaxY = i;
            }
        }
    }

    
    Point pt1;//点坐标1
    Point pt2;//点坐标2
    pt1.x = nMaxX;
    pt1.y = nMaxY;
    pt2.x = nMaxX+nTplWidth;
    pt2.y = nMaxY+nTplHeight;
    rectangle(*pTo,pt1,pt2,Scalar(255,255,255),3,8,0);//用点画矩形窗

}   


int main()
{

    clock_t start,end;
    start=clock();		//程序开始计时
    cv::String pattern="/home/jianzhuozhu/jianzhuozhu/opencv/NCC/*.bmp";
    Ve ve;
    vector<Mat> img=ve.ReadImage(pattern);

    end=clock();		//程序结束用时
	double endtime=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<"Total time:"<<endtime*1000<<"ms"<<endl;	//ms为单位
    waitKey(0);
    return 0;
}