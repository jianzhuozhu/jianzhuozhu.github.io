#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#define PI 3.14159

using namespace cv;
using namespace std;

Mat dstImage;

/*
由于二维傅里叶变换后得到的矩阵元素数值很大，并且包含实数和虚数。为便于观察分析，需要将变换后的结果进行求模，然后归一化到[0,255]，以便保存为频谱图。
归一化后，大部分像素灰度较低，在频谱图上接近黑色，肉眼不容易察觉，因此还需要使用log函数对低灰度区域进行增强。
另外，由于傅里叶变换本身具有对称性，最终生成的频谱图的四个角也具有对称性，
因此，一般在完成归一化后，还有一个把频谱原点平移到频谱图中心的操作。
对应的求模、归一化和中心平移函数分别为generate_spectrum()、normalize_spectrum()和shift_spectrum_to_center()。
*/

struct CComplex {
	CComplex(float r, float ir):real(r),irreal(ir){}
	CComplex():real(0),irreal(0){}
	float real;
	float irreal;
};

CComplex eMult(const CComplex& a, const CComplex& b) {
	return CComplex(a.real*b.real - a.irreal*b.irreal, a.real*b.irreal + a.irreal*b.real);
}

CComplex eAdd(const CComplex& a, const CComplex& b) {
	return CComplex(a.real + b.real, a.irreal + b.irreal);
}

//求二进制逆序数,得到傅里叶变换蝶形运算需要的原离散序列的顺序
int reverse_bit(int num, int len) 
{
	int i, bit;
	unsigned new_num = 0;
	for (i = 0; i < len; i++)
	{
		bit = num & 1;
		new_num <<= 1;
		new_num = new_num | bit;
		num >>= 1;
	}
	return new_num;
}

//判断是否是2的整数次方
int if_binaryNum(int length) {
	int num = 0;
	while (length != 1) {
		if (length % 2 == 0) {
			length = length / 2;
			num++;
		}
		else {
			return -1;
		}
	}
	return num;
}

//将非2的整数次方边长的图片缩放为2的整数次方
Mat binarylizeImage(Mat image) { 
	float c = image.cols, r = image.rows;
	int cn = 0, rn = 0, cnew = 2, rnew = 2;
	while (c / 2 > 1) { c = c / 2; cn++;}
	while (r / 2 > 1) { r = r / 2; rn++;}
	while (cn > 0) { cnew = cnew * 2; cn--;}
	while (rn > 0) { rnew = rnew * 2; rn--;}
	resize(image, image, Size(cnew, rnew));
	return image;
}

//FFT
void fastFuriorTransform(Mat image) {
	int lengthC = image.cols;
	int lengthR = image.rows;
	int numC, numR;
	vector<CComplex> resultE;
	Mat furiorResultF = Mat(image.cols, image.rows, CV_32FC1);
	//映射表
	vector <int> mappingC;
	vector <int> mappingR;
	//W值表
	vector <CComplex> mappingWC;
	vector <CComplex> mappingWR;

	//判断输入图片边长是否是2的n次方，如果不符合，调整image大小
	numC = if_binaryNum(lengthC);
	numR = if_binaryNum(lengthR);
	if (numC == -1 || numR == -1) {
		fastFuriorTransform(binarylizeImage(image));
		return;
	}

	//构造映射表
	for (int c = 0; c < image.cols; c++) {
		mappingC.push_back(0);
	}
	for (int r = 0; r < image.rows; r++) {
		mappingR.push_back(0);
	}
	for (int c = 0; c < image.cols; c++) {
		mappingC.at(reverse_bit(c, numC)) = c;  //reverse_bit(c, numC)给出原序列元素应该放置在蝶形运算中的位置
	}
	for (int r = 0; r < image.rows; r++) {
		mappingR.at(reverse_bit(r, numR)) = r;
	}

	//构造W表,相当于旋转因子的x次方
	for (int i = 0; i < lengthC / 2; i++) {
		CComplex w(cosf(2 * PI / lengthC * i), -1 * sinf(2 * PI / lengthC * i));
		mappingWC.push_back(w);
	}
	for (int i = 0; i < lengthR / 2; i++) {
		CComplex w(cosf(2 * PI / lengthR * i), -1 * sinf(2 * PI / lengthR * i));
		mappingWR.push_back(w);
	}

	//初始化
	for (int r = 0; r < lengthR; r++) {
		for (int c = 0; c < lengthC; c++) {
			//利用映射表，并且以0到1区间的32位浮点类型存储灰度值
			//实部为灰度值，虚部为0
			CComplex w((float)image.at<uchar>(mappingR.at(r), mappingC.at(c)) / 255, 0);  //使用mappingR或mappingC得到需要的蝶形坐标
			resultE.push_back(w);  //resultE中存放的其实是蝶形运算需要的重排后的图像数据，但是将图像数据组成了复数，实部为原图像数据，虚部为0
		}
	}

	//循环计算每行
	for (int r = 0; r < lengthR; r++) {
		//循环更新resultE中当前行的数值，即按照蝶形向前层层推进
		for (int i = 0; i < numC; i++) {   //总共有numC级蝶形结构，numC=log2N
			int combineSize = 2 << i;   //代表第m级蝶形运算的个数为2^m
			vector<CComplex> newRow;
			//按照2,4,8,16...为单位进行合并，并更新节点的值
			for (int j = 0; j < lengthC; j = j + combineSize) {  // j是当前的蝶形单元，j + combineSize代表跳到下一个蝶形运算
				int n;
				for (int k = 0; k < combineSize; k++) {  //在一个蝶形中运算
					if (k < (combineSize / 2)) {  //在前N/2， X1()+WX2()
						int w = k * lengthC / combineSize;
						n = k + j + r*lengthC;   // k + j是选出第r行(r*lengthC)的第j个蝶形运算单元
						newRow.push_back(eAdd(resultE.at(n), eMult(resultE.at(n + (combineSize >> 1)), mappingWC.at(w))));
					}
					else {   //在后N/2   X1()-WX2()
						int w = (k - (combineSize >> 1)) * lengthC / combineSize;
						n = k + j - (combineSize >> 1) + r*lengthC;
						newRow.push_back(eAdd(resultE.at(n), eMult(CComplex(-1, 0), eMult(resultE.at(n + (combineSize >> 1)), mappingWC.at(w)))));
					}

				}
			}
			//用newRow来更新resultE中的值
			for (int j = 0; j < lengthC; j++) {
				int n = j + r*lengthC;
				resultE.at(n) = newRow.at(j);
			}
			newRow.clear();
		}
	}

	//循环计算每列
	for (int c = 0; c < lengthC; c++) {
		for (int i = 0; i < numR; i++) {
			int combineSize = 2 << i;
			vector <CComplex> newColum;
			for (int j = 0; j < lengthR; j = j + combineSize) {
				int n;
				for (int k = 0; k < combineSize; k++) {
					if (k < (combineSize >> 1)) {
						int w = k * lengthR / combineSize;
						n = (j + k) * lengthC + c;
						newColum.push_back(eAdd(resultE.at(n), eMult(resultE.at(n + (combineSize >> 1)*lengthC), mappingWR.at(w))));
					}
					else {
						int w = (k - (combineSize >> 1)) * lengthR / combineSize;
						n = (j + k - (combineSize >> 1)) * lengthC + c;
						newColum.push_back(eAdd(resultE.at(n), eMult(CComplex(-1, 0), eMult(resultE.at(n + (combineSize >> 1)*lengthC), mappingWR.at(w)))));
					}
				}
			}
			//用newColum来更新resultE中的值
			for (int j = 0; j < lengthR; j++) {
				int n = j*lengthC + c;
				resultE.at(n) = newColum.at(j);
			}
			newColum.clear();
		}
	}

	//结果存入一个vector<float>中
	float val_max, val_min;
	vector <float> amplitude;
	for (int r = 0; r < lengthR; r++) {
		for (int c = 0; c < lengthC; c++) {
			CComplex e = resultE.at(r*lengthC + c);
			float val = sqrt(e.real*e.real + e.irreal*e.irreal) + 1;
			//对数尺度缩放
			val = log(val);
			amplitude.push_back(val);
			if (c == 0 && r == 0) {
				val_max = val;
				val_min = val;
			}
			else {
				if (val_max < val) val_max = val;
				if (val_min > val) val_min = val;
			}
		}
	}

	//将vector中的数据转存到Mat中，并归一化到0到255区间
	Mat fftResult = Mat(lengthC, lengthR, CV_8UC1);
	for (int i = 0; i < lengthR; i++) {
		for (int j = 0; j < lengthC; j++) {
			int val = (int)((amplitude.at(i*lengthC + j) - val_min) * 255 / (val_max - val_min));
			fftResult.at<uchar>(i, j) = val;
		}
	}

	//调整象限
	int cx = fftResult.cols / 2;
	int cy = fftResult.rows / 2;
	Mat q0(fftResult, Rect(0, 0, cx, cy));
	Mat q1(fftResult, Rect(cx, 0, cx, cy));
	Mat q2(fftResult, Rect(0, cy, cx, cy));
	Mat q3(fftResult, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	
    dstImage = fftResult.clone();
	imwrite("fft.jpg", fftResult);
	imshow("fft", fftResult);
}


int main(void) {
	time_t start, end;
    Mat srcImage = imread("../2.jpg");
    Mat grayImage;
    imshow("raw",srcImage);
    cvtColor(srcImage,grayImage,COLOR_RGB2GRAY);
	Mat inputImage = grayImage.clone();

	time(&start);
	fastFuriorTransform(inputImage);
    Mat out;
	time(&end);
	printf("Total Cost: %fs\n", difftime(end, start));
    imshow("dstImage", dstImage);
	waitKey();
	return 0;
}

