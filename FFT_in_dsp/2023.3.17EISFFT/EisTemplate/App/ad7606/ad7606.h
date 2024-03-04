/*
 * ad7606.h
 *
 *  Created on: 2023年3月5日
 *      Author: GXY1
 */

#ifndef APP_AD7606_AD7606_H_
#define APP_AD7606_AD7606_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"


#define	  AD7656_BASIC    (*((volatile  Uint16 *)0x4000))
#define   SET_ADRST       GpioDataRegs.GPBSET.bit.GPIO49=1      //连接AD7606复位
#define   CLEAR_ADRST     GpioDataRegs.GPBCLEAR.bit.GPIO49=1
#define   AD_BUSY         GpioDataRegs.GPBDAT.bit.GPIO48
#define   SET_ADCLK       GpioDataRegs.GPBSET.bit.GPIO62=1      //启动AD转换
#define   CLR_ADCLK       GpioDataRegs.GPBCLEAR.bit.GPIO62=1

extern int adc_value[8];

void ad7606_init(void);
void ad7606_sample(void);




#endif /* APP_AD7606_AD7606_H_ */
