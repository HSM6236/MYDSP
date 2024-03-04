#include <stdio.h>
#include "math.h"
#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "DSP2833x_GlobalPrototypes.h"

#include "beep.h"
#include "sci.h"
#include "timer.h"
#include "ad7606.h"
#include "da5620.h"
//#include "fft.h"
#include "sogi.h"

volatile unsigned char timer0_flag = 0;
int text = 0;
Uint16 adc_cnt=0;

float testSample7[N_SIGN_SOGI];  //通道1的时域采集点
float testSample6[N_SIGN_SOGI];  //通道2的时域采集点

float output1[N_SIGN_SOGI];  //角度
float output2[N_SIGN_SOGI];  //角度

//float testSample7[N_SIGN];  //通道1的时域采集点
//float testSample6[N_SIGN];  //通道2的时域采集点
//EIS output1[N_SIGN];
//EIS output2[N_SIGN];

//int N1=1000; //一个正弦波的采样个数
double PI=3.1415927;
float my_ang=0.0;
float avr_ang=0.0;


//double angle_per1=(2*3.1415927)/(1000); //每个采样点占用的角度，单位是弧度
//double sin_arr1[500]={0};

//void sindata_init(void)
//{
//	int n=0;
//	double x=0;
//   for(n=0;n<(N1/2);n++){
//       x=angle_per1*(N1/2-n);
//       sin_arr1[n]=(0.9875*x-0.1551*pow(x,3)+0.0056*pow(x,5))*(256/2.2);//这里算出来的值直接写入ADC，就是输出-1V到1V，换成负载电流是正负5A
//   }
//}

void main(void)
{
	/*设置CPU主频,30M晶振倍频，再分频给cpu；
	  给高、低速外设时钟预定标寄存器赋值以获得想要的高、低速外设时钟频率;
	 可在其中关闭不用的外设时钟*/
	InitSysCtrl();

	/*************  常规外设初始化  *************/
	beep_init();	//蜂鸣器初始化

	InitXintf16Gpio();
	InitXintf();	//扩展ADC存储初始化
	ad7606_init();	//adc初始化
//	dac5620_init(); //dac初始化

	/*************  包含中断外设初始化  *************/

	//失能 CPU 级中断，并初始化 PIE 控制器寄存器和 PIE 中断向量表
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

	DINT;
//	sindata_init();
	sci_b_init(9600); // 串口初始化
	timer0_init(150, 1000000/2560);  //定时器初始化，150M主频，1000/128ms中断一次
	//1000--1ms--1000hz
	//5000--5ms--200hz
	//1000000/128--1000/128 ms--128hz
	//1000000/640--1000/640 ms--640hz
	//1000000/1280--1000/1280ms--1280hz
	//1000000/2560--1000/2560ms--2560hz
	//可以测到的范围 128/2=64hz

	//float f=2560.0;
	//修改采样频率时，时钟也需修改
	EINT;
	ERTM; //使能总中断
//	int duty_cnt=0;
//	int n=0;
	while(1){

//	    for(n=0;n<(N1/2);n++){
////		   WriteDAC(0,0,sin_arr1[n] + 0.4*256/2.2);
//		   WriteDAC(0,0,128);
//		   DELAY_US(1000);
//	   }
//	    for(int cnt=0;cnt<1000;cnt++){
//	        DELAY_US(1);
//
//	    if(cnt==1000){
//	        WriteDAC(0,0,sin_arr1[n] + 0.4*256/2.2);
////	        WriteDAC(0,0,128);
//	        if(duty_cnt == 500) duty_cnt=0;
//
//	    }
//	    }





//		if(timer0_flag){
//			uart_printf("d1: %f\r\n",(20.0 / 65536.0) * adc_value[6]);
//			//sci_send_msg(&SciB, send_buf);
//			timer0_flag = 0;
//		}
	    if(timer0_flag==1){
//	             for(i=0;i<8;i++)
//	                AdcSample[i] = (20.0/65536.0)* adc_value[i];//数字量转换为模拟量,输入范围是正负10V,精度为16位
//	                                               //相当于将20V分成了65536份,公式为A=(20.0/65536.0)*D;A为模拟量值，D为数字量值;
//	                                               //如果输入范围是正负5V则公式为A=(10.0/65536.0)*D

//	             testSample1[adc_cnt] = AdcSample[0];  //存放每个通道的历史数值
//	             testSample2[adc_cnt] = AdcSample[1];
//	             testSample3[adc_cnt] = AdcSample[2];
//	             testSample4[adc_cnt] = AdcSample[3];
//	             testSample5[adc_cnt] = AdcSample[4];
//	             testSample6[adc_cnt] = AdcSample[5];
	             testSample6[adc_cnt] = adc_value[6] * (20.0/65536.0);
//	             uart_printf("d: %f\n", adc_value[7] * (20.0/65536.0));
                 testSample7[adc_cnt] = adc_value[7] * (20.0/65536.0);
                 adc_cnt++;


	             if(adc_cnt > N_SIGN_SOGI - 1){

	                 //fft时改为N_SIGN

	                 adc_cnt = 0;
//	                 run_fft(testSample6, f, output1);  //f是采样频率
//	                 run_fft(testSample7, f, output2);  //f是采样频率
//	                 uart_printf("d: %f\n", output1[60].ang-output2[60].ang);//这里返回的是60HZ的角度差（弧度值）
//	                 uart_printf("d: %f\n", (output1[60].ang-output2[60].ang)/PI*180.0);//这里返回的是60HZ的角度差（弧度值）
	                 run_sogi(testSample6, output1, N_SIGN_SOGI);
	                 run_sogi(testSample7, output2, N_SIGN_SOGI);
	                 for (int i = 0; i < N_SIGN_SOGI; i++){
	                     if(i>=2000){
	                         //取2560的后560个角度值
	                         my_ang = my_ang + output2[i]-output1[i];
	                     }
	                 }
	                 avr_ang=my_ang/560;
	                 //对这些角度取平均

	             }

	             timer0_flag = 0;       //定时器复位改到前面
	          }

	}

}


