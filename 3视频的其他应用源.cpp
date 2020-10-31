#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

int OtsuAlgThreshold(double Histogram[256], double totalNum)
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
	//Mat back;
	//back = imread("r.jpg");
	cout << "begin";
	VideoCapture cap("mt//pika.mp4");//素材视频 
	int T = 195;
	VideoCapture cap_back(0);
	cap.set(CAP_PROP_FRAME_WIDTH, 320);//宽度 
	cap.set(CAP_PROP_FRAME_HEIGHT, 240);//高度
	//cout<<cap.get(CAP_PROP_FRAME_WIDTH)<<"---";//宽度 
	//cout<<cap.get(CAP_PROP_FRAME_HEIGHT);//高度

	Mat frame;
	Mat back;
	cap >> frame;
	//cout << frame.channels(); =3
	Mat frame_2(frame.rows, frame.cols, CV_8UC1);
	Mat frame_3(frame.rows, frame.cols, CV_8UC1);
	//Mat frame_3(frame.rows, frame.cols, CV_8UC1);
	Mat back_m;
	int totalFrames = cap.get(CAP_PROP_FRAME_COUNT);
	int currentFrames = 0;
	cout << endl<< "************************************" << endl;
	while (true) {
		cap >> frame;
		cap_back >> back;
		back_m = back(Rect(160, 230, 240, 240));
		vector<Mat > splited(3);
		split(frame, splited);
		frame_2 = splited[1](Rect(0, 0, 480,480));
		threshold(frame_2, frame_3, T, 1, THRESH_BINARY);
		resize(frame_3,frame_3, Size(240, 240));
		resize(frame(Rect(0, 0, 480, 480)), frame_2, Size(240, 240));

		Vec3b* back_p;
		Vec3b* frame_p;
		uchar* frame_3_p;
		for (int i = 0; i < 240; ++i) {
			for (int j = 0; j < 240; ++j) {
				back_p = back_m.ptr<Vec3b>(i, j);
				frame_p = frame_2.ptr<Vec3b>(i, j);
				frame_3_p = frame_3.ptr<uchar>(i, j);
				back_p->val[0] = back_p->val[0] * (*frame_3_p)+ frame_p->val[0]*(1-*frame_3_p);
				back_p->val[1] = back_p->val[1] * (*frame_3_p) + frame_p->val[1] * (1 - *frame_3_p);
				back_p->val[2] = back_p->val[2] * (*frame_3_p) + frame_p->val[2] * (1 - *frame_3_p);
			}
		}
		if (currentFrames == totalFrames - 3)
		{
			currentFrames = 0;
			cap.set(CAP_PROP_POS_FRAMES, 1);
		}
		currentFrames++;
		//cout << "restart" << endl;
		//imshow("frame", frame);
		//imshow("back_m", back_m);
		//imshow("frame_3", back_m);
		imshow("mix", back);
		waitKey(40);
	}
	return 0;
}
