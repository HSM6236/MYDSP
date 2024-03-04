/*
 * sci.c
 *
 *  Created on: 2023年3月5日
 *      Author: gxy
 */


#include "sci.h"
#include "stdio.h"


#define MAX_WRITE_LEN (528)
void uart_printf(char * fmt, ...) //自定义变参函数
{
    char * 	arg_ptr;
    unsigned int  i ,  len;
	static char	write_buf[MAX_WRITE_LEN];

    va_start(arg_ptr, fmt);
    len = vsprintf((char *)write_buf, fmt, arg_ptr);
    va_end(arg_ptr);

    for (i = 0; i < len; i++)
    {
        if ( '\0' != write_buf[i] )
		{
        	while (ScibRegs.SCIFFTX.bit.TXFFST != 0); //使用SCIB
        	ScibRegs.SCITXBUF=write_buf[i];
        }
    }

}





unsigned int sci_b_buf[SCIB_RX_DATA_MAX] = {0};
int sci_rx_data_len = 0;

interrupt void SCIB_IRQ_Handle(void)
{
    DINT;
    //上为中断处理相关
    int i;
    for(i = 0; i < SCIB_RX_FIFO_LEN; i++)
    {
    	sci_b_buf[sci_rx_data_len++]  = ScibRegs.SCIRXBUF.all;
    	if(sci_rx_data_len == SCIB_RX_DATA_MAX) sci_rx_data_len = 0;
    }

    // 下为中断处理相关
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    EINT;
}

void sci_b_init(Uint32 baud_rate)
{
    unsigned char scihbaud=0;
    unsigned char scilbaud=0;
    Uint16 sci_brr=0;

    // 开启外设时钟
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   // SCI-B
    PieVectTable.SCIRXINTB = &SCIB_IRQ_Handle;    /*注册中断服务函数*/
    EDIS;

    // 初始化GPIO
    InitScibGpio();

    //初始化SCI外设
    sci_brr=37500000/(8*baud_rate)-1;  // 37.5M在外设时钟初始化时决定，根据波特率计算brr填充值
    scihbaud=sci_brr>>8;
    scilbaud=sci_brr&0xff;

    //Initalize the SCI FIFO
    ScibRegs.SCIFFTX.all=0xE040; //	00000 0 1 0 00000   FIFO发送深度0，失能中断，清除中断标志
    ScibRegs.SCIFFRX.all=0x0028; //	00000 0 0 1 01000   FIFO接收深度为8字节，使能中断，收到6字节触发中断
    ScibRegs.SCIFFCT.all=0x00;

    // Note: Clocks were turned on to the SCIC peripheral
    // in the InitSysCtrl() function
    ScibRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback，No parity,8 char bits, async mode, idle-line protocol
    ScibRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICTL2.all =0x0003;
    ScibRegs.SCICTL2.bit.TXINTENA =1;
    ScibRegs.SCICTL2.bit.RXBKINTENA =1;
    ScibRegs.SCIHBAUD    =scihbaud;  // 9600 baud @LSPCLK = 37.5MHz.
    ScibRegs.SCILBAUD    =scilbaud;
    ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
    ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
    ScibRegs.SCIFFRX.bit.RXFIFORESET=1;


    // Enable interrupts required for this example

   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
   PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT3 intB_RX

   //   PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, int1 intA_RX
   //   PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2 intA_TX
//  PieCtrlRegs.PIEIER9.bit.INTx4=1;     // PIE Group 9, INT4 intB_TX
   IER |= 0x100;	// Enable CPU INT


}

// 使用方式，sci_send_byte(&SciA, 0)
void sci_send_byte(volatile struct SCI_REGS *sci_x, int a)
{
    while (sci_x->SCIFFTX.bit.TXFFST != 0);
    sci_x->SCITXBUF = a;
}

// 使用方式，sci_send_msg(&SciA, "test")
void sci_send_msg(volatile struct SCI_REGS *sci_x, char *msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
    	sci_send_byte(sci_x, msg[i]);
        i++;
    }
}


