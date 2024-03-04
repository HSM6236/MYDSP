/*
 * da5620.h
 *
 *  Created on: 2023年3月5日
 *      Author: GXY1
 */

#ifndef APP_DA5620_DA5620_H_
#define APP_DA5620_DA5620_H_

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define SetLOAD GpioDataRegs.GPBDAT.bit.GPIO55=1;
#define ClrLOAD GpioDataRegs.GPBDAT.bit.GPIO55=0;

void dac5620_init(void);
void WriteDAC(unsigned char add,unsigned char rng,unsigned char vol);



#endif /* APP_DA5620_DA5620_H_ */
