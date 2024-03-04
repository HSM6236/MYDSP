/*
 * da5620.c
 *
 *  Created on: 2023年3月5日
 *      Author: GXY1
 */

#include "da5620.h"


void spi_init(void)
{



    SpiaRegs.SPICCR.all =0x0a;//进入初始状态，数据在上升沿输出，自测禁止，11位数据模式

    SpiaRegs.SPICTL.all =0x0006; //使能主机模式，正常相位，使能主机发送，禁止接收

    SpiaRegs.SPIBRR =0x0031;    //SPI波特率=37.5M/50=0.75MHZ
    SpiaRegs.SPICCR.all =0x8a; //退出初始状态
    SpiaRegs.SPIPRI.bit.FREE = 1;  // 自由运行
}


void dac5620_init(void)
{
    InitSpiaGpio();  //初始化spi引脚
    EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;        // 定义为GPIO口
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;         // 定义为输出引脚
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;         // 禁止上拉电阻

	SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;   // SPI-A
    EDIS;

    spi_init();
    SetLOAD;
}

/*
* add 是 4 个通道的地址（00，01，10，11）
* RNG 是输出范围的倍数，可以是 0(1倍) 或 1（2倍）
* VOL 是 0~256 数据
* REF* rng/256 ， REF=2.2V
*/
void WriteDAC(unsigned char add,unsigned char rng,unsigned char vol)
{
    Uint16 data=0;
    data = ((add<<14) | (rng<<13) | (vol<<5));
    while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG ==1);            //判断SPI的发送缓冲区是否是空的，等于0可写数据

    SpiaRegs.SPITXBUF = data;    //把要发送的数据写入SPI发送缓冲区

    while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG==1);        //当发送缓冲区出现满标志位时，开始锁存数据

    ClrLOAD;
    DELAY_US(2);

    SetLOAD;
    DELAY_US(10);
}
