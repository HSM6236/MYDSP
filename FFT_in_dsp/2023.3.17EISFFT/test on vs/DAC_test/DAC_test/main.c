#include<stdio.h>

int main(){
double PI = 3.1415927;

double angle_per1=(2*3.1415927)/(1000); //每个采样点占用的角度，单位是弧度
double sin_arr1[500]={0};
int N1 = 1000; //一个正弦波的采样个数
int n=0;
double x=0;
    for(n=0;n<(N1/2);n++){
        x=angle_per1*(N1/2-n);
        sin_arr1[n]=(0.9875*x-0.1551*pow(x,3)+0.0056*pow(x,5))*(256/2.2);//这里算出来的值直接写入ADC，就是输出-1V到1V，换成负载电流是正负5A
    }

    for (n = 0; n < (N1 / 2); n++) 
    {
        
        printf("%f\n", sin_arr1[n]);//这里算出来的值直接写入ADC，就是输出-1V到1V，换成负载电流是正负5A
    }



}
