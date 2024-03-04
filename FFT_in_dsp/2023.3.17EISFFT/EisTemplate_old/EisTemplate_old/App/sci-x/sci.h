/*
 * sci.h
 *
 *  Created on: 2023年3月5日
 *      Author: gxy
 */

#ifndef APP_SCI_X_SCI_H_
#define APP_SCI_X_SCI_H_

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"


#define SciA SciaRegs
#define SciB ScibRegs
#define SciC ScicRegs

// scib的最大容量
#define SCIB_RX_DATA_MAX 50
// scib_rx 的fifo容量
#define SCIB_RX_FIFO_LEN 8



extern unsigned int sci_b_buf[SCIB_RX_DATA_MAX];
extern int sci_rx_data_len;

void sci_b_init(Uint32 baud_rate);
void sci_send_byte(volatile struct SCI_REGS *sci_x, int a);
void sci_send_msg(volatile struct SCI_REGS *sci_x, char *msg);
void uart_printf(char * fmt, ...); //自定义变参函数

#endif /* APP_SCI_X_SCI_H_ */
