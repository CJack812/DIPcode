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
	auto Max = max_element(nums.begin(), nums.end());//max����������,�����Ŀ
	putText(hist, s, Point(150, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));
	//*********��������ϵ************//
	Point o = Point(100, 550);
	Point x = Point(700, 550);
	Point y = Point(100, 150);
	//x��
	line(hist, o, x, Scalar(255), 2, 8, 0);
	//y��
	line(hist, o, y, Scalar(255), 2, 8, 0);

	//********���ƻҶ�����***********//
	Point pts[256];
	//���������
	for (int i = 0; i < 256; i++)
	{
		pts[i].x = i * 2 + 100;//��һ������ʽ������
		pts[i].y = 550 - int(nums[i] * (300.0 / (*Max)));//��һ����[0, 300]
		//��ʾ������
		if ((i + 1) % 16 == 0)
		{
			string num = format("%d", i + 1);
			putText(hist, num, Point(pts[i].x, 570), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255));
		}
	}
	//������
	for (int i = 0; i < 256; i++)
	{
		line(hist, pts[i], Point(pts[i].x, 550), Scalar(255), 1);
	}
	//��ʾͼ��
	imshow(s, 255 - hist);
}

//����ֱ��ͼ��ͳ�Ƹ��Ҷȼ����ظ���
vector<int> calHist(Mat img, string s)
{
	Mat grey;
	grey = img;
	//imshow("�Ҷ�ͼ", grey);
	//������Ҷȼ����ظ���
	int height = img.rows;
	int width = img.cols;
	int mn = height * width;
	vector<int> nums(256);
	for (int i = 0; i < grey.rows; i++)
	{
		uchar* p = grey.ptr<uchar>(i);
		for (int j = 0; j < grey.cols; j++)
		{
			nums[p[j]] = nums[p[j]] + 1;//ͳ�Ƹ��Ҷ����ظ���
		}
	}
	drawHist(nums, s);//��ͼ
	return nums;
}

int OtsuAlgThreshold(const Mat image);

int main(int argc, char *argv[])
{
	cout << "�뽫ͼƬ�϶�������,�����»س���." << endl;
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
		cout << "����õ���ֵΪ�� " << thresholdValue << endl;
		threshold(image, imageOutput, thresholdValue, 255, THRESH_BINARY);
		//threshold(image, imageOtsu, 0, 255, THRESH_OTSU); //Opencv Otsu�㷨
		//imshow("SoureImage",image);
		imshow("Image", image);
		imshow("Otsu", imageOutput);
		waitKey();
		cout << "��ͼ���Դ������,�뽫��һ��ͼƬ�϶�������,�����»س���." << endl;
	}
}
int OtsuAlgThreshold(const Mat image)
{
	if (image.channels() != 1)
	{
		cout << "Please input Gray-image!" << endl;
		return 0;
	}
	int T = 0; //Otsu�㷨��ֵ
	double varValue = 0; //��䷽���м�ֵ����
	double w0 = 0; //ǰ�����ص�����ռ����
	double w1 = 0; //�������ص�����ռ����
	double u0 = 0; //ǰ��ƽ���Ҷ�
	double u1 = 0; //����ƽ���Ҷ�
	double Histogram[256] = { 0 }; //�Ҷ�ֱ��ͼ���±��ǻҶ�ֵ�����������ǻҶ�ֵ��Ӧ�����ص�����
	uchar *data = image.data;
	double totalNum = image.rows*image.cols; //��������
	for (int i = 0; i < image.rows; i++)   //Ϊ������������û�а�rows��cols���������
	{
		for (int j = 0; j < image.cols; j++)
		{
			Histogram[data[i*image.step + j]]++;
		}
	}
	for (int i = 0; i < 255; i++)
	{
		//ÿ�α���֮ǰ��ʼ��������
		w1 = 0;		u1 = 0;		w0 = 0;		u0 = 0;
		for (int j = 0; j <= i; j++) //�������ָ�ֵ����
		{
			w1 += Histogram[j];  //�����������ص�����
			u1 += j * Histogram[j]; //�������������ܻҶȺ�
		}
		if (w1 == 0) //�����������ص���Ϊ0ʱ�˳�
		{
			continue;
		}
		u1 = u1 / w1; //��������ƽ���Ҷ�
		w1 = w1 / totalNum; // �����������ص�����ռ����

		for (int k = i + 1; k < 255; k++)
		{
			w0 += Histogram[k];  //ǰ���������ص�����
			u0 += k * Histogram[k]; //ǰ�����������ܻҶȺ�
		}
		if (w0 == 0) //ǰ���������ص���Ϊ0ʱ�˳�
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