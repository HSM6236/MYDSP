#ifndef __BEEP__
#define __BEEP__

#include "DSP2833x_Device.h"
// DSP2833x 头文件
#include "DSP2833x_Examples.h"
// DSP2833x 例子相关头文件

#define BEEP_OFF (GpioDataRegs.GPBSET.bit.GPIO53=1)
#define BEEP_ON (GpioDataRegs.GPBCLEAR.bit.GPIO53=1)
#define BEEP_TOGGLE (GpioDataRegs.GPBTOGGLE.bit.GPIO53=1)





void beep_init(void);


#endif
