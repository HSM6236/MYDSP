/*
 * timer.c
 *
 *  Created on: 2023年3月5日
 *      Author: gxy
 */


#include "timer.h"
#include "ad7606.h"


extern volatile unsigned char timer0_flag;

interrupt void TIM0_IRQ_Handle(void)
{
	EALLOW;
	ad7606_sample();


	timer0_flag = 1;
	PieCtrlRegs.PIEACK.bit.ACK1=1;
	EDIS;
}



//周期单位为us
void timer0_init(Uint32 frq, Uint32 period)
{
	// 使能时钟
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer 0
	PieVectTable.TINT0 = &TIM0_IRQ_Handle; // 注册中断服务函数
	EDIS;

	//指向定时器 0 的寄存器地址
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	//设置定时器 0 的周期寄存器值
	CpuTimer0Regs.PRD.all	= 0xFFFFFFFF;
	//设置定时器预定标计数器值为 0
	CpuTimer0Regs.TPR.all	= 0;
	CpuTimer0Regs.TPRH.all = 0;
	//确保定时器 0 为停止状态
	CpuTimer0Regs.TCR.bit.TSS = 1;
	//重载使能
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;

	ConfigCpuTimer(&CpuTimer0, frq, period); //参数2为系统主频(MHZ)；参数3单位为us，此处1ms产生以此中断

	//开始定时器功能
	CpuTimer0Regs.TCR.bit.TSS=0;
	//开启 CPU 第一组中断并使能第一组中断的第 7 个小中断，即定时器 0
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;


}

