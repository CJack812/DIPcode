#include <opencv.hpp>
#include <highgui.hpp>
#include<iostream>
#include <imgproc.hpp>
#include <core.hpp>
#include<string>
#include<vector>
#include<math.h>
using namespace std;
using namespace cv;

void Transfer(Mat grey) {
	int rows = grey.rows;
	int cols = grey.cols;
	double gama = 1;
	cout <<  "使用伽马变换:请输入gama值" << endl<<" gama = " ;
	cin >> gama;
	Mat Transfered(rows, cols, CV_8UC1,Scalar(0));
	uchar * data1 = grey.data;
	uchar * data2 = Transfered.data;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			Transfered.ptr<uchar>(i, j)[0] =
			uchar(powl(grey.ptr<uchar>(i, j)[0],gama)/powl(255, gama) * 255);
		}
	}
	imshow("gama transfer", Transfered);
}


int main()
{
	string imageName;
	cout<< "请将图片拖入窗口并按回车" << endl;
	while (cin >> imageName)
	{
		//**********读入图像**************//
		Mat image;
		image = imread(imageName, IMREAD_COLOR);
		if (!image.data)
		{
			printf(" No image data \n ");
			return -1;
		}
		int height = image.rows;
		int width = image.cols;
		Mat grey_image(height, width, CV_8UC1);
		cvtColor(image, grey_image,COLOR_BGR2GRAY);
		Transfer(grey_image);//图像增强并显示
		imshow("grey", grey_image);
		waitKey(0);
		cout << "please input next image" << endl;
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