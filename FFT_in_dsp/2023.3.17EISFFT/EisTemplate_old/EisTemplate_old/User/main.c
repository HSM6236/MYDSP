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
#include "fft.h"

volatile unsigned char timer0_flag = 0;
int text = 0;
Uint16 adc_cnt=0;

float testSample7[N_SIGN];

EIS output[N_SIGN];

double PI=3.1415927;
//double angle_per1=(2*3.1415927)/(1000); //姣忎釜閲囨牱鐐瑰崰鐢ㄧ殑瑙掑害锛屽崟浣嶆槸寮у害
//double sin_arr1[500]={0};

//void sindata_init(void)
//{
//	int n=0;
//	double x=0;
//   for(n=0;n<(N1/2);n++){
//       x=angle_per1*(N1/2-n);
//       sin_arr1[n]=(0.9875*x-0.1551*pow(x,3)+0.0056*pow(x,5))*(256/2.2);//杩欓噷绠楀嚭鏉ョ殑鍊肩洿鎺ュ啓鍏DC锛屽氨鏄緭鍑�-1V鍒�1V锛屾崲鎴愯礋杞界數娴佹槸姝ｈ礋5A
//   }
//}

void main(void)
{
	/*璁剧疆CPU涓婚,30M鏅舵尟鍊嶉锛屽啀鍒嗛缁檆pu锛�
	  缁欓珮銆佷綆閫熷璁炬椂閽熼瀹氭爣瀵勫瓨鍣ㄨ祴鍊间互鑾峰緱鎯宠鐨勯珮銆佷綆閫熷璁炬椂閽熼鐜�;
	 鍙湪鍏朵腑鍏抽棴涓嶇敤鐨勫璁炬椂閽�*/
	InitSysCtrl();

	/*************  甯歌澶栬鍒濆鍖�  *************/
	beep_init();	//铚傞福鍣ㄥ垵濮嬪寲

	InitXintf16Gpio();
	InitXintf();	//鎵╁睍ADC瀛樺偍鍒濆鍖�
	ad7606_init();	//adc鍒濆鍖�
//	dac5620_init(); //dac鍒濆鍖�

	/*************  鍖呭惈涓柇澶栬鍒濆鍖�  *************/

	//澶辫兘 CPU 绾т腑鏂紝骞跺垵濮嬪寲 PIE 鎺у埗鍣ㄥ瘎瀛樺櫒鍜� PIE 涓柇鍚戦噺琛�
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

	DINT;

	sci_b_init(9600); // 涓插彛鍒濆鍖�
	timer0_init(150, 1000000.0/640);  //瀹氭椂鍣ㄥ垵濮嬪寲锛�150M涓婚锛�1ms涓柇涓�娆�
	//1000--1ms--1000hz
	//5000--5ms--200hz
	//1000000/128--1000/128 ms--128hz
	//1000000/640--1000/640 ms--640hz
	//鍙互娴嬪埌鐨勮寖鍥� 128/2=64hz

	float f=640.0;

	EINT;
	ERTM; //浣胯兘鎬讳腑鏂�

	//int n=0;
	while(1){


//	    for(n=0;n<(N1/2);n++){
////		   WriteDAC(0,0,sin_arr1[n] + 0.4*256/2.2);
//		   WriteDAC(0,0,128);
//		   DELAY_US(1000);
//	   }

//		if(timer0_flag){
//			uart_printf("d1: %f\r\n",(20.0 / 65536.0) * adc_value[6]);
//			//sci_send_msg(&SciB, send_buf);
//			timer0_flag = 0;
//		}
	    if(timer0_flag==1){
//	             for(i=0;i<8;i++)
//	                AdcSample[i] = (20.0/65536.0)* adc_value[i];//鏁板瓧閲忚浆鎹负妯℃嫙閲�,杈撳叆鑼冨洿鏄璐�10V,绮惧害涓�16浣�
//	                                               //鐩稿綋浜庡皢20V鍒嗘垚浜�65536浠�,鍏紡涓篈=(20.0/65536.0)*D;A涓烘ā鎷熼噺鍊硷紝D涓烘暟瀛楅噺鍊�;
//	                                               //濡傛灉杈撳叆鑼冨洿鏄璐�5V鍒欏叕寮忎负A=(10.0/65536.0)*D
                 timer0_flag = 0;       //瀹氭椂鍣ㄥ浣嶆敼鍒板墠闈�
//	             testSample1[adc_cnt] = AdcSample[0];  //瀛樻斁姣忎釜閫氶亾鐨勫巻鍙叉暟鍊�
//	             testSample2[adc_cnt] = AdcSample[1];
//	             testSample3[adc_cnt] = AdcSample[2];
//	             testSample4[adc_cnt] = AdcSample[3];
//	             testSample5[adc_cnt] = AdcSample[4];
//	             testSample6[adc_cnt] = AdcSample[5];
//	             testSample7[adc_cnt] = AdcSample[6];
//	             uart_printf("d: %f\n", adc_value[7] * (20.0/65536.0));
                 testSample7[adc_cnt] = adc_value[7]* (20.0/65536.0);
                 adc_cnt++;


	             if(adc_cnt > N_SIGN - 1){
	                 adc_cnt = 0;

	                 run_fft(testSample7, f, output);  //f鏄噰鏍烽鐜�

	             }



	          }



	}

}


