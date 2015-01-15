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

#include "definitions.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpiops.h"
#include "xstatus.h"


/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
//#define Device_Id_Var		XPAR_XSPIPS_1_DEVICE_ID
#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID

#define SPI1_GPIO_BANK		XGPIOPS_BANK3

#define SPI0_GPIO_BANK		XGPIOPS_BANK3

#define SPI_EMIO_BANK		XGPIOPS_BANK2

#define SPI0_SS0_PIN		93 //TRSS
#define SPI0_SS1_PIN		92 //AFSE
#define SPI0_MOSI_PIN		90
#define SPI0_MISO_PIN		89
#define SPI0_CLK_PIN		91
#define SPI0_PORT_MASK		0xF0
//#define SPI0_PORT_MASK		0x3F0000  //loopback mask
#define SPI0_MISO_MASK		0x8
#define SPI0_MISO_SHIFT		3

#define SPI1_SS0_PIN		97 //ADI1
#define SPI1_SS1_PIN		98 //ADI2
#define SPI1_MOSI_PIN		95 //11 swapped
#define SPI1_MISO_PIN		94 //10 swapped
#define SPI1_CLK_PIN		96
#define SPI1_PORT_MASK		0x1E00
#define SPI1_MISO_MASK		0x100
#define SPI1_MISO_SHIFT		8

#define EMIO_SS0_PIN		54
#define EMIO_SS1_PIN		54
#define EMIO_MOSI_PIN		55
#define EMIO_MISO_PIN		56
#define EMIO_CLK_PIN		57
#define EMIO_PORT_MASK		0xB
#define EMIO_MISO_MASK		0x4


/*
 * The following constant defines the slave select signal that is used to
 * to select the flash device on the SPI bus, this signal is typically
 * connected to the chip select of the device
 */
//#define Slave_Select_Var	0x00
#define SS_DELAY		1000000

#define LED_DELAY		100000

#define PRODUCT_ID                      0x037

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

int SpiPs_write_sensor(u16 SpiDeviceId);
int SpiPs_read_sensor(u16 SpiDeviceId);

void soft_spi_transfer(XGpioPs *InstancePtr, char *SendBufPtr,char *RecvBufPtr, unsigned ByteCount);
void soft_spi_transfer_invert(XGpioPs *InstancePtr, char *SendBufPtr,char *RecvBufPtr, unsigned ByteCount);
