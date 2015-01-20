/******************************************************************************
*
* (c) Copyright 2010-2013 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xspips_polled_example.c
*
*
* This file contains a design example using the SPI driver (XSpiPs) in
* polled mode with a Serial Flash device. This examples performs
* transfers in polled mode.
* The hardware which this example runs on, must have a Serial Flash
* for it to run. This example has been tested with SST25W080.
*
* @note
*
* None.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who Date     Changes
* ----- --- -------- -----------------------------------------------
* 1.00  sg  1/30/13  First release
*
*</pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "soft_spi_routines.h"



/************************** Variable Definitions *****************************/

/*
 * The instances to support the device drivers are global such that they
 * are initialized to zero each time the program runs. They could be local
 * but should at least be static so they are zeroed.
 */
//static XSpiPs SpiInstance;
extern XGpioPs Gpio; //gpio instance from PDI
 char gpio_bank;
 char ss_pin;
 char mosi_pin;
 char miso_pin;
 char clk_pin;
 unsigned long port_mask;
 unsigned long miso_pin_mask;
 char miso_shift = 0;

/*****************************************************************************/
/**
*
Main function to do SPI transfers
*
******************************************************************************/


//sends MSB first
void soft_spi_transfer(XGpioPs *InstancePtr, char *SendBufPtr,char *RecvBufPtr, unsigned char ByteCount)
{
	volatile int Delay;
	int i;
	int j;

	char shift;

	char current_byte;
	unsigned long current_recv;

	int active_tx_bit;
	unsigned long active_rx_bit;



	XGpioPs_WritePin(&Gpio, mosi_pin, 1);
	/*
	xil_printf("MOSI: %d ",mosi_pin);
	xil_printf("MISO: %d ",miso_pin);
	xil_printf("CLK: %d ",clk_pin);
	*/
	j = 0;
	while(j<ByteCount){
		current_byte = *(SendBufPtr+j);
		//xil_printf("\r\nCurrent Byte: %x\r\n", current_byte);
		current_recv = 0;

		for(i=7;i>=0;i--){

			active_tx_bit = current_byte>>i&0x1;

			//xil_printf("set the bit(%x) on the MOSI line\r\n", active_tx_bit);
			XGpioPs_WritePin(&Gpio, mosi_pin, active_tx_bit);
			XGpioPs_WritePin(&Gpio, EMIO_MOSI_PIN, active_tx_bit);

			//xil_printf("wait clock period\r\n");
			for (Delay = 0; Delay < LED_DELAY; Delay++);

			//xil_printf("set the clock high\r\n");
			XGpioPs_WritePin(&Gpio, clk_pin, 0x1);
			XGpioPs_WritePin(&Gpio, EMIO_CLK_PIN, 0x1);

			//xil_printf("wait clock period\r\n");
			for (Delay = 0; Delay < LED_DELAY; Delay++);

			//xil_printf("read MISO line\r\n");
			//active_rx_bit = XGpioPs_Read(&Gpio, gpio_bank)&miso_pin_mask;
			active_rx_bit = XGpioPs_ReadPin(&Gpio, miso_pin);
			XGpioPs_WritePin(&Gpio, EMIO_MISO_PIN, active_rx_bit>>miso_shift);
			//xil_printf("\r\nrx_bit:%x\r\n", active_rx_bit);

			//then have to apply some sort of mask or shift on this

			current_recv = current_recv|active_rx_bit<<i;

			//xil_printf("\r\nrx_byte:%x\r\n", current_recv);

			//xil_printf("set clock low\r\n");
			XGpioPs_WritePin(&Gpio, clk_pin, 0x0);
			XGpioPs_WritePin(&Gpio, EMIO_CLK_PIN, 0x0);

			//xil_printf("wait clock period\r\n");
			for (Delay = 0; Delay < LED_DELAY; Delay++);
		}

		//current_recv = current_recv>>miso_shift;
		//xil_printf("\r\nrx_byte:%x\r\n", current_recv);
		*(RecvBufPtr+j) = current_recv;

		j++;
	}

}

//sends LSB first
void soft_spi_transfer_invert(XGpioPs *InstancePtr, char *SendBufPtr,char *RecvBufPtr, unsigned char ByteCount)
{
	volatile int Delay;
	int i;
	int j;

	char shift;

	char current_byte;
	unsigned long current_recv;

	int active_tx_bit;
	unsigned long active_rx_bit;



	XGpioPs_WritePin(&Gpio, mosi_pin, 1);


	j = 0;
	while(j<ByteCount){
		current_byte = *(SendBufPtr+j);
		//xil_printf("\r\nCurrent Byte: %x\r\n", current_byte);
		current_recv = 0;

		for(i=0;i<8;i++){

			active_tx_bit = current_byte>>i&0x1;

			//xil_printf("%x", active_tx_bit);
			XGpioPs_WritePin(&Gpio, mosi_pin, active_tx_bit);
			XGpioPs_WritePin(&Gpio, EMIO_MOSI_PIN, active_tx_bit);
			//set below for loopback along with different mask:
			//XGpioPs_WritePin(&Gpio, miso_pin, active_tx_bit);

			//xil_printf("set the clock high\r\n");
			XGpioPs_WritePin(&Gpio, clk_pin, 0x1);
			XGpioPs_WritePin(&Gpio, EMIO_CLK_PIN, 0x1);

			//xil_printf("wait clock period\r\n");
			for (Delay = 0; Delay < LED_DELAY; Delay++);

			//xil_printf("read MISO line\r\n");
			active_rx_bit = XGpioPs_ReadPin(&Gpio, miso_pin);
			//active_rx_bit = active_tx_bit;
			XGpioPs_WritePin(&Gpio, EMIO_MISO_PIN, active_rx_bit>>miso_shift);
			//xil_printf("\r\nrx_bit:%x\r\n", active_rx_bit);

				//then have to apply some sort of mask or shift on this

			current_recv = current_recv|active_rx_bit<<i;

			//xil_printf("\r\nrx_byte:%x\r\n", current_recv);

			//xil_printf("set clock low\r\n");
			XGpioPs_WritePin(&Gpio, clk_pin, 0x0);
			XGpioPs_WritePin(&Gpio, EMIO_CLK_PIN, 0x0);

			//xil_printf("wait clock period\r\n");
			for (Delay = 0; Delay < LED_DELAY; Delay++);
		}

		//xil_printf("\r\n");
		//current_recv = current_recv>>miso_shift;
		//xil_printf("\r\nrx_byte:%x\r\n", current_recv);
		*(RecvBufPtr+j) = current_recv;

		j++;
	}

}

