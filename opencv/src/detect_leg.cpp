#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

CascadeClassifier cascade;
int dectedleg; //当前帧 检测人腿个数
void chatterCallbackCam(Mat& src);
int dectedImg(cv::Mat& dstImage,cv::CascadeClassifier cascade);

//使用分类器cascade对图片dstImage进行模板检测
int dectedImg(cv::Mat& dstImage,cv::CascadeClassifier cascade) {

	static bool lastDetect = false;//上一帧图片模板检测是否成功
	static Rect lastRect;//上一帧图片检测出来的矩形框

	dectedleg = 0;
	//检测  
	std::vector<cv::Rect> rect;//检测结果
	//使用分类器对cascade对图片dstImage进行检测，检测结果放在rect中，1.15检测过程中前后两次扫描窗口的比例系数，
	//检测目标的相邻的邻接矩形的个数
	cascade.detectMultiScale(dstImage, rect, 1.15, 3, 0);//dstImage待检测图片，rect 检测结果，3 相邻两个可能结果的最小距离（可以修改）
	int rectSizeMax = INT_MAX;//多个可能结果是人腿的最小距离
	int rectIdx = 0;//检测结果中最可能是人腿的id（只考虑一个人腿的情况）
	dectedleg = rect.size();//返回检测到的人腿个数
	if (rect.size() > 0)//当检测出可能是人腿的个数大于0
	{
		if (rect.size() > 1)//当检测出人腿的个数大于1个
		{
			//假设连续两帧之间最大的人腿位置变化不大，当当前帧检测出人腿结果可能有多个时，
			//以这些可能的结果与上一帧人腿的距离最近的位置为最可能是人腿的结果
			if (lastDetect)
			{
				for (int i = 0; i < rect.size(); i++)
				{
					int dist = (rect[i].x - lastRect.x) * (rect[i].x - lastRect.x)
						+ (rect[i].y - lastRect.y) * (rect[i].y - lastRect.y);
					if (rectSizeMax > dist)
					{
						rectSizeMax = dist;
						rectIdx = i;
					}
				}
			}
			else {//如果上一帧没有检测成功，则取第0个
				rectIdx = 0;
			}
		}
		lastRect = rect[rectIdx];
		lastDetect = true;
		//在图片dstImage中画出最有可能是人腿的矩形框
		rectangle(dstImage, Point(rect[rectIdx].x, rect[rectIdx].y),
			Point(rect[rectIdx].x + rect[rectIdx].width, rect[rectIdx].y + rect[rectIdx].height)
			, Scalar(255, 255, 255), 2, 8, 0);
		cout<<"rect[rectIdx].x       "<<rect[rectIdx].x<<"　　rect[rectIdx].y       "<<rect[rectIdx].y<<endl;
		cout<<"rect[rectIdx].width   "<<rect[rectIdx].width<<"　　rect[rectIdx].height  "<<rect[rectIdx].height<<endl;
		cout<<"dectedleg             "<<dectedleg<<endl;
		//截取出矩形框中图片准备进行进一步判断。
		Mat rectImage = dstImage.clone()(rect[rectIdx]);
		imshow("dstImage",dstImage);
	}
	return dectedleg;
}

//图片回调函数
void chatterCallbackCam(Mat& src)
{
    cv::Mat dst;//= src.clone();
    cv::cvtColor(src,dst,CV_BGR2RGB);//颜色空间转换
    cv::waitKey(10);
    dectedleg=dectedImg(dst, cascade);//检测人腿
}

int main(int argc, char **argv)
{
    //导入人腿分类器
    //cascade.xml需要与cascade放当实际路径一样
    bool loadxml=cascade.load("/home/jianzhuozhu/jianzhuozhu/opencv/data/cascade.xml");//
    if(!loadxml){
        cout<<"没有正常导入cascade.xml"<<endl;
        return 0;
    }
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
        chatterCallbackCam(frame);
        waitKey(30);
    }
    return 0;
}