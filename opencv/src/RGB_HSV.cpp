#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

// 全局变量定义及赋值
int threshold_type = 3;
int const max_type = 4;     //阈值化类型


int threshold_H_min = 0;   //H通道阈值设定
int threshold_S_min = 0;   //S通道阈值设定  
int threshold_V_min = 0;   //V通道阈值设定
int threshold_H_max = 255;   //H通道阈值设定
int threshold_S_max = 255;   //S通道阈值设定  
int threshold_V_max = 255;   //V通道阈值设定


int const max_value = 255;  
int const max_BINARY_value = 255;

Mat src, dst;
 Mat src_hsv;
//创建一个图像向量
vector<Mat> planes; 

string window_name = "Threshold Func";
string trackbar_type = "TrackbarType";  //0: Binary 1: Binary Inverted 2: Truncate 
                                   //3: To Zero  4: To Zero Inverted
string trackbar_H_min = "H_Value_min";
string trackbar_S_min = "S_Value_min";
string trackbar_V_min = "V_Value_min";
string trackbar_H_max = "H_Value_max";
string trackbar_S_max = "S_Value_max";
string trackbar_V_max = "V_Value_max";



/// 自定义函数声明
void Threshold_Func( int, void* );

int main()
{
    VideoCapture capture;
    capture.open(0);
    while(1)
    {
        
        Mat img_HSV;
       
        capture>>src;
        if(src.empty())
        {
            break;
        }
        GaussianBlur(src,src,Size(3,3),0,0);
        //将多通道图像分割为若干单通道图像
        cvtColor( src, src_hsv, CV_RGB2HSV ); 
        split(src_hsv, planes);

        // 创建一个窗口显示图片
        namedWindow( window_name, CV_WINDOW_AUTOSIZE );   

        // 创建滑动条来控制阈值
        

        createTrackbar( trackbar_H_min,
                        window_name, &threshold_H_min,
                        max_value, Threshold_Func );

        createTrackbar( trackbar_S_min,
                        window_name, &threshold_S_min,
                        max_value, Threshold_Func );

        createTrackbar( trackbar_V_min,
                        window_name, &threshold_V_min,
                        max_value, Threshold_Func );

        createTrackbar( trackbar_H_max,
                        window_name, &threshold_H_max,
                        max_value, Threshold_Func );

        createTrackbar( trackbar_S_max,
                        window_name, &threshold_S_max,
                        max_value, Threshold_Func );

        createTrackbar( trackbar_V_max,
                        window_name, &threshold_V_max,
                        max_value, Threshold_Func );



        // // 初始化自定义的阈值函数
        Threshold_Func( 0, 0 );


        imshow("video",src);
        
        waitKey(30);
    }
    return 0;
}

//自定义的阈值函数
void Threshold_Func( int, void* )
{
  /* 0: 二进制阈值
     1: 反二进制阈值
     2: 截断阈值
     3: 0阈值
     4: 反0阈值
   */
  dst.create(src.size(),src.type());

  
  // threshold( planes[0], thredplanes[0], threshold_H_min, threshold_H_max,1 );
  // threshold( planes[1], thredplanes[1], threshold_S_min, threshold_S_max,1 );
  // threshold( planes[2], thredplanes[2], threshold_V_min, threshold_V_max,1 );


  vector<Mat> thredplanes;
  // inRange(src_hsv,Scalar(threshold_H_min,threshold_S_min,threshold_V_min),
  //           Scalar(threshold_H_max,threshold_S_max,threshold_V_max),dst);
  split(dst, thredplanes);
  inRange(planes[0],Scalar(threshold_H_min,0.0,0,0),Scalar(threshold_H_max,0.0,0,0),thredplanes[0]);
	inRange(planes[1],Scalar(threshold_S_min,0.0,0,0),Scalar(threshold_S_max,0.0,0,0),thredplanes[1]);
	inRange(planes[2],Scalar(threshold_V_min,0.0,0,0),Scalar(threshold_V_max,0.0,0,0),thredplanes[2]);

  bitwise_and(thredplanes[1],thredplanes[2],thredplanes[1]);
  bitwise_and(thredplanes[0],thredplanes[1],thredplanes[1]);

  GaussianBlur(thredplanes[1],thredplanes[1],Size(3,3),0,0);

  imshow( window_name, thredplanes[1] );
}
