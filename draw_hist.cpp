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
		line(hist, pts[i], Point(pts[i].x,550), Scalar(255), 1);
	}
	//显示图像
	imshow(s, 255-hist);
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
			nums[p[j]]= nums[p[j]]+1;//统计各灰度像素个数
		}
	}
	drawHist(nums, s);//画图
	return nums;
}

////////////////////////////////////////////////
int main(int argc, char** argv)
{
	cout << "请将图片拖动到窗口,并按下回车键." << endl;
	string imageName;
	for (int i = 0; cin >> imageName; i++)
	{
		//**********读入图像**************//
		Mat image;
		image = imread(imageName, IMREAD_COLOR);
		if (!image.data)
		{
			printf(" No image data \n ");
			return -1;
		}
		cout << imageName << endl;
		int height = image.rows;
		int width = image.cols;
		Mat grey_image;
		cvtColor(image, grey_image, COLOR_BGR2GRAY);//
		imshow(imageName, grey_image);
		calHist(grey_image, "The hist");
		waitKey(0);
		cout << "该图像以处理完毕,请将下一幅图片拖动到窗口,并按下回车键." << endl;
	}
return 0;
}

vector<int> gethist(Mat grey) {
	vector<int> hist(256);
	int nRows = grey.rows;
	int nCols = grey.cols;

	if (grey.isContinuous())
	{
		nCols = nRows * nCols;
		nRows = 1;
	}

	for (int h = 0; h < nRows; ++h)
	{
		uchar *ptr = grey.ptr<uchar>(h);
		for (int w = 0; w < nCols; ++w)
			hist[(*ptr)]++;
	}
	return hist;

}