#include <opencv.hpp>
#include <highgui.hpp>
#include<iostream>
#include <imgproc.hpp>
#include <core.hpp>
#include<string>
#include<vector>
using namespace std;
using namespace cv;
void drawHist(vector<int> nums, string s)
{
	Mat hist = Mat::zeros(600, 800, CV_8UC1);
	auto Max = max_element(nums.begin(), nums.end());//max迭代器类型,最大数目
	putText(hist, s, Point(150, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
	//*********绘制坐标系************//
	Point o = Point(100, 550);
	Point x = Point(700, 550);
	Point y = Point(100, 150);
	//x轴
	line(hist, o, x, Scalar(255), 2, 8, 0);
	//y轴
	line(hist, o, y, Scalar(255), 2, 8, 0);

	//********绘制灰度曲线***********//
	Point pts[256];
	//生成坐标点
	for (int i = 0; i < 256; i++)
	{
		pts[i].x = i * 2 + 100;//隔一个点显式横坐标
		pts[i].y = 550 - int(nums[i] * (300.0 / (*Max)));//归一化到[0, 300]
		//显示横坐标
		if ((i + 1) % 16 == 0)
		{
			string num = format("%d", i + 1);
			putText(hist, num, Point(pts[i].x, 570), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255));
		}
	}
	//绘制线
	for (int i = 0; i < 256; i++)
	{
		line(hist, pts[i], Point(pts[i].x, 550), Scalar(255), 1);
	}
	//显示图像
	imshow(s, 255 - hist);
}

//计算直方图，统计各灰度级像素个数
vector<int> calHist(Mat img, string s)
{
	Mat grey;
	grey = img;
	//imshow("灰度图", grey);
	//计算各灰度级像素个数
	int height = img.rows;
	int width = img.cols;
	int mn = height * width;
	vector<int> nums(256);
	for (int i = 0; i < grey.rows; i++)
	{
		uchar* p = grey.ptr<uchar>(i);
		for (int j = 0; j < grey.cols; j++)
		{
			nums[p[j]] = nums[p[j]] + 1;//统计各灰度像素个数
		}
	}
	drawHist(nums, s);//画图
	return nums;
}

int OtsuAlgThreshold(const Mat image);

int main(int argc, char *argv[])
{
	cout << "请将图片拖动到窗口,并按下回车键." << endl;
	string imageName;
	for (int i = 0; cin >> imageName; i++)
	{
		Mat image;
		image = imread(imageName, IMREAD_COLOR);
		if (!image.data)
		{
			printf(" No image data \n ");
			return -1;
		}
		cvtColor(image, image, COLOR_RGB2GRAY);
		Mat imageOutput;
		Mat imageOtsu;
		int thresholdValue = OtsuAlgThreshold(image);
		cout << "计算得到阈值为： " << thresholdValue << endl;
		threshold(image, imageOutput, thresholdValue, 255, THRESH_BINARY);
		//threshold(image, imageOtsu, 0, 255, THRESH_OTSU); //Opencv Otsu算法
		//imshow("SoureImage",image);
		imshow("Image", image);
		imshow("Otsu", imageOutput);
		waitKey();
		cout << "该图像以处理完毕,请将下一幅图片拖动到窗口,并按下回车键." << endl;
	}
}
int OtsuAlgThreshold(const Mat image)
{
	if (image.channels() != 1)
	{
		cout << "Please input Gray-image!" << endl;
		return 0;
	}
	int T = 0; //Otsu算法阈值
	double varValue = 0; //类间方差中间值保存
	double w0 = 0; //前景像素点数所占比例
	double w1 = 0; //背景像素点数所占比例
	double u0 = 0; //前景平均灰度
	double u1 = 0; //背景平均灰度
	double Histogram[256] = { 0 }; //灰度直方图，下标是灰度值，保存内容是灰度值对应的像素点总数
	uchar *data = image.data;
	double totalNum = image.rows*image.cols; //像素总数
	for (int i = 0; i < image.rows; i++)   //为表述清晰，并没有把rows和cols单独提出来
	{
		for (int j = 0; j < image.cols; j++)
		{
			Histogram[data[i*image.step + j]]++;
		}
	}
	for (int i = 0; i < 255; i++)
	{
		//每次遍历之前初始化各变量
		w1 = 0;		u1 = 0;		w0 = 0;		u0 = 0;
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

		for (int k = i + 1; k < 255; k++)
		{
			w0 += Histogram[k];  //前景部分像素点总数
			u0 += k * Histogram[k]; //前景部分像素总灰度和
		}
		if (w0 == 0) //前景部分像素点数为0时退出
		{
			break;
		}
		u0 = u0 / w0;
		w0 = w0 / totalNum; 
		double varValueI = w0 * w1*(u1 - u0)*(u1 - u0); 
		if (varValue < varValueI)
		{
			varValue = varValueI;
			T = i;
		}
	}
	return T;
}