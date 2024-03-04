#include "beep.h"

void beep_init(void)
{
	EALLOW;

	GpioCtrlRegs.GPBMUX2.bit.GPIO53=0;  // 选择寄存器，是否复用
	GpioCtrlRegs.GPBDIR.bit.GPIO53=1;  // 方向寄存器，输出
	GpioCtrlRegs.GPBPUD.bit.GPIO53=0;  // 上拉禁止寄存器


	GpioDataRegs.GPBSET.bit.GPIO53=1;  //不响

	EDIS;
}
