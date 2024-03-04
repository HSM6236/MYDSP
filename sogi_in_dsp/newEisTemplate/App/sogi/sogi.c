#include  <stdio.h>
#include  <math.h>
#include "sogi.h"

void G1_filter(float* input, float* y, int data_len) {
    //正交滤波器参数
    //B是x的
    //A是y的
	float B[3] = { 0.0682,0.0000,-0.0682 };
	float A[3] = { 1.0000,-1.8434,0.8635 };
    //采样频率为2560
	float w_x[3] = { 0,0,0 };
	float w_y[3] = { 0,0,0 };

	for (int i = 0; i < data_len; i++)
	{
		w_x[0] = input[i];
		w_y[0] = B[0] * w_x[0] + B[1] * w_x[1] + B[2] * w_x[2] - w_y[1] * A[1] - w_y[2] * A[2];
		y[i] = w_y[0] / A[0];
		w_x[2] = w_x[1];
		w_x[1] = w_x[0];
		w_y[2] = w_y[1];
		w_y[1] = w_y[0];
	}
}

void G2_filter(float* input, float* y, int data_len) {
	//正交滤波器参数
	//B是x的
	//A是y的
	float B[3] = {0.8628,-1.0000,0};
	float A[3] = {1.0000,-0.8628,0};
	//采样频率为2560
	float w_x[3];
	float w_y[3];
	w_x[0] = w_x[1] = w_x[2] = 0;
	w_y[0] = w_y[1] = w_y[2] = 0;
	for (int i = 0; i < data_len; i++)
	{
		w_x[0] = input[i];
		w_y[0] = (B[0] * w_x[0] + B[1] * w_x[1] + B[2] * w_x[2]) \
			- w_y[1] * A[1] - w_y[2] * A[2];
		y[i] = w_y[0] / A[0];
		w_x[2] = w_x[1];
		w_x[1] = w_x[0];
		w_y[2] = w_y[1];
		w_y[1] = w_y[0];
	}
}


void run_sogi(float* input,float* output,int data_len) {
    double PI=3.1415926;
	float output_G1[data_len];
	float output_G2[data_len];
	G1_filter(input, output_G1, data_len);
	G2_filter(output_G1, output_G2, data_len);
	for (int i = 0; i < data_len; i++) {
		output[i] = -180*atan(output_G1[i] / output_G2[i])/ PI;
		//参考廖师姐*（-180）
		//返回的是输入信号的相位
	}
	
}
