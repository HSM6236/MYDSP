/*
 * ad7606.c
 *
 *  Created on: 2023年3月5日
 *      Author: GXY1
 */
#include "ad7606.h"

void ad7606_init(void)
{

	EALLOW;
	GpioCtrlRegs.GPBMUX2.bit.GPIO48  = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO49  = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO62  = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO49   = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO48   = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO62   = 1;

	GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO0   = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO1   = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO2   = 1;
	EDIS;
	GpioDataRegs.GPADAT.bit.GPIO0 = 0;
	DELAY_US(1);
	GpioDataRegs.GPADAT.bit.GPIO1 = 0;
	DELAY_US(1);
	GpioDataRegs.GPADAT.bit.GPIO2 = 0;
	DELAY_US(1);
	CLEAR_ADRST;
	SET_ADCLK;

	DELAY_US(1);
	SET_ADRST;
	DELAY_US(100);
	CLEAR_ADRST;
	DELAY_US(100);
}


int adc_value[8] = {0};
// I[adc_cnt] = (20.0 / 65536.0) * DOUT[6];//DOUT[6];
void ad7606_sample(void)
{
	int channel;

    SET_ADCLK;
    DELAY_US(1);
    CLR_ADCLK;
    DELAY_US(1);
    while (AD_BUSY) {}

    for (channel = 0; channel < 8; channel++)
    {
    	adc_value[channel] = AD7656_BASIC ;//
    }

}



