#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;

int OtsuAlgThreshold(double Histogram[256], double totalNum)
{

	int T = 0; //Otsu�㷨��ֵ
	double varValue = 0; //��䷽���м�ֵ����
	double w0 = 0; //ǰ�����ص�����ռ����
	double w1 = 0; //�������ص�����ռ����
	double u0 = 0; //ǰ��ƽ���Ҷ�
	double u1 = 0; //����ƽ���Ҷ�
	for (int i = 0; i < 255; i++)
	{
		//ÿ�α���֮ǰ��ʼ��������
		w1 = 0;		u1 = 0;		w0 = 0;		u0 = 0;
		//***********����������ֵ����**************************
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
		//***********����������ֵ����**************************

		//***********ǰ��������ֵ����**************************
		for (int k = i + 1; k < 255; k++)
		{
			w0 += Histogram[k];  //ǰ���������ص�����
			u0 += k * Histogram[k]; //ǰ�����������ܻҶȺ�
		}
		if (w0 == 0) //ǰ���������ص���Ϊ0ʱ�˳�
		{
			break;
		}
		u0 = u0 / w0; //ǰ������ƽ���Ҷ�
		w0 = w0 / totalNum; // ǰ���������ص�����ռ����
		//***********ǰ��������ֵ����**************************

		//***********��䷽�����******************************
		double varValueI = w0 * w1*(u1 - u0)*(u1 - u0); //��ǰ��䷽�����
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
	VideoCapture cap("mt//pika.mp4");//�ز���Ƶ 
	int T = 195;
	VideoCapture cap_back(0);
	cap.set(CAP_PROP_FRAME_WIDTH, 320);//��� 
	cap.set(CAP_PROP_FRAME_HEIGHT, 240);//�߶�
	//cout<<cap.get(CAP_PROP_FRAME_WIDTH)<<"---";//��� 
	//cout<<cap.get(CAP_PROP_FRAME_HEIGHT);//�߶�

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
