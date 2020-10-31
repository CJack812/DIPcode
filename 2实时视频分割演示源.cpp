#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

int OtsuAlgThreshold(double Histogram[256],double totalNum)
{
	
	int T = 0; //Otsu算法阈值
	double varValue = 0; //类间方差中间值保存
	double w0 = 0; //前景像素点数所占比例
	double w1 = 0; //背景像素点数所占比例
	double u0 = 0; //前景平均灰度
	double u1 = 0; //背景平均灰度
	for (int i = 0; i < 255; i++)
	{
		//每次遍历之前初始化各变量
		w1 = 0;		u1 = 0;		w0 = 0;		u0 = 0;
		//***********背景各分量值计算**************************
		for (int j = 0; j <= i; j++) //背景部分各值计算
		{
			w1 += Histogram[j];  //背景部分像素点总数
			u1 += j * Histogram[j]; //背景部分像素总灰度和
		}
		if (w1 == 0) //背景部分像素点数为0时退出
		{
			continue;
		}
		u1 = u1 / w1; //背景像素平均灰度
		w1 = w1 / totalNum; // 背景部分像素点数所占比例
		//***********背景各分量值计算**************************

		//***********前景各分量值计算**************************
		for (int k = i + 1; k < 255; k++)
		{
			w0 += Histogram[k];  //前景部分像素点总数
			u0 += k * Histogram[k]; //前景部分像素总灰度和
		}
		if (w0 == 0) //前景部分像素点数为0时退出
		{
			break;
		}
		u0 = u0 / w0; //前景像素平均灰度
		w0 = w0 / totalNum; // 前景部分像素点数所占比例
		//***********前景各分量值计算**************************

		//***********类间方差计算******************************
		double varValueI = w0 * w1*(u1 - u0)*(u1 - u0); //当前类间方差计算
		if (varValue < varValueI)
		{
			varValue = varValueI;
			T = i;
		}
	}
	return T;
}

int main()
{	
	cout << "begin";
	VideoCapture cap(0);
	Mat frame;
	cap >> frame;
	Mat frame_2(frame.rows, frame.cols,CV_8UC1);
	Mat frame_3(frame.rows, frame.cols, CV_8UC1);
	cvtColor(frame,frame_2,COLOR_BGR2GRAY);
	double Histogram[2][256] = { 0 }; //灰度直方图，下标是灰度值，保存内容是灰度值对应的像素点总数
	uchar *data = frame_2.data;
	double totalNum = frame_2.rows*frame_2.cols; //像素总数
	for (int i = 0; i < frame_2.rows; i++)   //为表述清晰，并没有把rows和cols单独提出来
	{
		for (int j = 0; j < frame_2.cols; j++)
		{
			Histogram[0][data[i*frame_2.step + j]]++;
		}
	}
	int times = 0;
	double similarity = 0;
	int k = 1;
	int T = 125;
	T = OtsuAlgThreshold(Histogram[0],totalNum);
	while (true) {
		k = k % 2;
		cap >> frame;//VideoCapture cap(0);摄像头
		cvtColor(frame, frame_2, COLOR_BGR2GRAY);
		data = frame_2.data;
		//二维数组Histogram[2][256]用来存储前后两帧的直方图数据
		for (int i = 0; i < frame_2.rows; i++) 
			for (int j = 0; j < frame_2.cols; j++) Histogram[k][data[i*frame_2.step + j]]++;
		similarity = 0;
		//计算前后两帧相似度
		for (int i = 0; i < 256; i++) {
			similarity = 
			similarity + sqrt(Histogram[k%2][i] * Histogram[(k+1)%2][i]) / totalNum;
			Histogram[(k + 1) % 2][i] = 0;
		}
		if (similarity<0.9) // 重新计算otus阈值
		{
			T = OtsuAlgThreshold(Histogram[k], totalNum);
			cout << similarity << endl;
			cout << "第"<<times++<<"次计算阈值,阈值为="<<T<<endl;
		}
		threshold(frame_2, frame_3, T, 255, THRESH_BINARY);
		imshow("frame_3", frame_3);
		k++;
		waitKey(500);
	}
	return 0;
}
