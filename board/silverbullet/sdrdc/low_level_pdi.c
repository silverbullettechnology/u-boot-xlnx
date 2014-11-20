/**
 * \file
 *
 * \brief Low level PDI driver
 *
 * Copyright (C) 2009 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
#include <low_level_pdi.h>

#include "xparameters.h"
#include "xgpiops.h"
#include "xstatus.h"
#include "xuartps_hw.h"

/************************** Constant Definitions ****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID

/*
 * The following constant is used to wait after an LED is turned on to make
 * sure that it is visible to the human eye.  This constant might need to be
 * tuned for faster or slower processor speeds.
 */
#define LED_DELAY		10000000
#define PDI_DELAY		10000000

/*
 * Following constant is used to determine which Bank of the GPIO is
 * used for the Input and Output pins.
 */
#define INPUT_BANK		XGPIOPS_BANK0	/* Bank to be used for input */
#define OUTPUT_BANK		XGPIOPS_BANK0	/* Bank to be used for LEDS */
#define EMIO_BANK		XGPIOPS_BANK2	/* Bank to be used for emio */

//#define LED0			10	/* GPIO pin number of first LED */
#define OUTPUT_BANK_WIDTH	1	/* 32 pins are connected */

#define LED_MAX_BLINK		0x06	/* Number of times the LED Blinks */

#define LED0_pin       54
#define LED1_pin       55
#define LED2_pin       56
#define LED3_pin       57

#define PDI_DATA_pin   54
#define PDI_CLOCK_pin    55

#define AD1_reset_pin  58
#define AD2_reset_pin  59


/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions *******************/

/************************** Function Prototypes ****************************/

static int GpioOutputExample(void);
static int GpioInputExample(u32 *DataRead);
int GpioPolledInputExample(u16 DeviceId, u32 *DataRead);
int GpioPolledOutputExample(u16 DeviceId, u32 *DataRead);

/************************** Variable Definitions **************************/

/*
 * The following are declared globally so they are zeroed and can be
 * easily accessible from a debugger.
 */
XGpioPs Gpio;	/* The driver instance for GPIO Device. */
/*****************************************************************************/



static int GpioOutputExample(void)
{
	volatile int Delay;
	u32 LedBit;
	u32 LedLoop;
	int i;

	/*
	 * Set the Mode as Normal Mode, set the direction for all signals to be
	 * outputs and Enable the Output enable for the LED Pins.
	 */
	XGpioPs_SetDirection(&Gpio, OUTPUT_BANK, 0x400);
	XGpioPs_SetOutputEnable(&Gpio, OUTPUT_BANK, 0x400);

	XGpioPs_SetDirection(&Gpio, EMIO_BANK, 0xFFFFFFFF);
	XGpioPs_SetOutputEnable(&Gpio, EMIO_BANK, 0xFFFFFFFF);


	for (LedBit = 10; LedBit < OUTPUT_BANK_WIDTH + 10; LedBit++) {

		for (LedLoop = 0; LedLoop < LED_MAX_BLINK; LedLoop++) {

			/*
			 * Clear the GPIO Output.
			 */
//			XGpioPs_WritePin(&Gpio, LedBit, 0x0);

			XGpioPs_WritePin(&Gpio, LED0_pin, 0x0);
			XGpioPs_WritePin(&Gpio, LED1_pin, 0x0);
			XGpioPs_WritePin(&Gpio, LED2_pin, 0x0);
			XGpioPs_WritePin(&Gpio, LED3_pin, 0x0);

			for(i=60;i<=70;i++){
				XGpioPs_WritePin(&Gpio, i, 0);
			}


			/*
			 * Wait a small amount of time so the LED is visible.
			 */
			for (Delay = 0; Delay < LED_DELAY; Delay++);

			/*
			 * Set the GPIO Output to High.
			 */
//			XGpioPs_WritePin(&Gpio, LedBit, 0x1);

			XGpioPs_WritePin(&Gpio, LED0_pin, 0x1);
			XGpioPs_WritePin(&Gpio, LED1_pin, 0x1);
			XGpioPs_WritePin(&Gpio, LED2_pin, 0x1);
			XGpioPs_WritePin(&Gpio, LED3_pin, 0x1);

			for(i=60;i<=70;i++){
				XGpioPs_WritePin(&Gpio, i, 1);
			}

			/*
			 * Wait a small amount of time so the LED is visible.
			 */
			for (Delay = 0; Delay < LED_DELAY; Delay++);
		}

	}


	XGpioPs_WritePin(&Gpio, AD1_reset_pin, 0x1);
	XGpioPs_WritePin(&Gpio, AD2_reset_pin, 0x1);
	return XST_SUCCESS;
}

int GpioPolledOutputExample(u16 DeviceId, u32 *DataRead)
{
	int Status;
	XGpioPs_Config *ConfigPtr;

	/*
	 * Initialize the GPIO driver.
	 */
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Run the Output Example.
	 */
	Status = GpioOutputExample();
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

static int GpioInputExample(u32 *DataRead)
{

	/*
	 * Set the Mode as Normal Mode, set the direction for all signals to be
	 * inputs in the specified BANK.
	 */
	XGpioPs_SetDirection(&Gpio, INPUT_BANK, 0x0);

	/*
	 * Read the state of the data so that it can be  verified.
	 */
	*DataRead = XGpioPs_Read(&Gpio, INPUT_BANK);

	return XST_SUCCESS;
}

int hello_led(void)
{
	int Status;
	u32 InputData;

	printf("\n\r********************************************************");
	printf("\n\r********************************************************");
	printf("\n\r**     SDRDC - GPIO LED Test                          **");
	printf("\n\r********************************************************");
	printf("\n\r********************************************************\r\n");
	printf("Test successful if D2303..D2306 are flashing.\r\n");

	//Xil_DCacheEnable();
	Status = GpioPolledOutputExample(GPIO_DEVICE_ID, &InputData);
	if (Status != XST_SUCCESS) {
		printf("GPIO Polled Mode Example Test Failed\r\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}



/**
 * \brief Initialize PDI.
 *
 * Initialize the peripeherals necessary to communicate with the ATxmega
 *  PDI.
 */
void pdi_init( void )
{
	int Status;
	XGpioPs_Config *ConfigPtr;
	int i, cycle;
	volatile int Delay;

	/*
	 * Initialize the GPIO driver.
	 */
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);

	/*
	 * Initialize the PDI pins in TX
	 */

	XGpioPs_SetDirectionPin(&Gpio, PDI_DATA_pin, 1);
	XGpioPs_SetDirectionPin(&Gpio, PDI_CLOCK_pin, 1);

	udelay(100);
	
	/*
	 * Set PDI_DATA High
	 */
	
	XGpioPs_WritePin(&Gpio, PDI_DATA_pin, 0x1);
	XGpioPs_WritePin(&Gpio, PDI_CLOCK_pin, 0x0);
	udelay(100);

	/*
	 * Cycle PDI clock 17 times
	 */

	for(cycle = 0; cycle <=17; cycle++){
		XGpioPs_WritePin(&Gpio, PDI_CLOCK_pin, 0x1);
		for (Delay = 0; Delay < PDI_DELAY; Delay++);
		XGpioPs_WritePin(&Gpio, PDI_CLOCK_pin, 0x0);
		for (Delay = 0; Delay < PDI_DELAY; Delay++);
	}
}

/**
 * \brief Write bulk bytes with PDI.
 *
 *  Send bytes over the PDI channel.
 *
 * \param data Pointer to memory where data to be sent is stored.
 * \param length Number of bytes to be sent.
 *
 * \retval STATUS_OK The transmission was successful.
 * \retval ERR_BAD_DATA One of the bytes sent was corrupted during transmission.
 */
enum status_code pdi_write( const uint8_t *data, uint16_t length )
{
	uint16_t i, j;
	uint8_t dummy_read = 0;
	int active_tx_bit;
	u8 current_byte;
	volatile int Delay;
	

	//enable TX mode
	XGpioPs_SetDirectionPin(&Gpio, PDI_DATA_pin, 1);

	for (i = 0; i < length; i++) {
		current_byte = data[i];
		for(j=7;j>=0;j--){
			active_tx_bit = current_byte>>j&0x1;
			XGpioPs_WritePin(&Gpio, PDI_DATA_pin, active_tx_bit);
			for (Delay = 0; Delay < PDI_DELAY; Delay++);
			XGpioPs_WritePin(&Gpio, PDI_CLOCK_pin, 0x1);
			for (Delay = 0; Delay < PDI_DELAY; Delay++);
			XGpioPs_WritePin(&Gpio, PDI_CLOCK_pin, 0x0);
		}		
	}

	return STATUS_OK;
}

/**
 * \brief Read bulk bytes from PDI.
 *
 * \param data Pointer to memory where data to be stored.
 * \param length Number of bytes to be read.
 * \param retries the retry count.
 *
 * \retval non-zero the length of data.
 * \retval zero read fail.
 */
uint16_t pdi_read( uint8_t *data, uint16_t length, uint32_t retries )
{
	uint32_t count;
	uint16_t bytes_read = 0;
	uint16_t i;

	/*for (i = 0; i < length; i++) {
		count = retries;
		while (count != 0) {
			if (usart_rx_is_complete(&USART)) {
				*(data + i) = usart_get(&USART);
				bytes_read++;
				break;
			}
			--count;
		}
		// Read fail error 
		if (count == 0) {
			return 0;
		}
	}*/

	return bytes_read;
}

/**
 * \brief Read a byte from PDI.
 *
 * \param ret Pointer to buffer memory where data to be stored.
 * \param retries the retry count.
 *
 * \retval STATUS_OK read successfully.
 * \retval ERR_TIMEOUT read fail.
 */
enum status_code pdi_get_byte( uint8_t *ret, uint32_t retries )
{
	while (retries != 0) {
		/*if (usart_rx_is_complete(&USART)) {
			*ret = usart_get(&USART);
			return STATUS_OK;
		}*/
		--retries;
	}

	return ERR_IO_ERROR;
}

/**
 * \brief This function disables the PDI port.
 */
void pdi_deinit( void )
{

}
