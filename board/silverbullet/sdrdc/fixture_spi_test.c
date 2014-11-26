


/***************************** Include Files *********************************/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "definitions.h"
#include "soft_spi_routines.h"
#include "routines.h"

#include "xparameters.h"
#include "xgpiops.h"
#include "xstatus.h"
#include "xuartps_hw.h"


//static XSpiPs SpiInstance;
extern XGpioPs Gpio; //gpio instance

//extern char Slave_Select_Var = 3;
//extern char Device_Id_Var = 2;

extern char gpio_bank;
extern char ss_pin;
extern char mosi_pin;
extern char miso_pin;
extern char clk_pin;
extern unsigned long port_mask;
extern unsigned long miso_pin_mask;
extern char miso_shift;

//static char Slave_Select_Var = 3;
//static char Device_Id_Var = 2;
/*****************************************************************************/
/**
*
* Main function to call the SPI Flash Polled Example.
*
* @param	None
*
* @return
*		- XST_SUCCESS if successful
*		- XST_FAILURE if not successful
*
* @note		None
*
******************************************************************************/


/******************************************************************************/
int fixture_write_read()
{
	XGpioPs_Config *ConfigPtr;	
	int Status;
	int i;

	char invert;
	char inbyte;

	int size;

	volatile int Delay;
	#define write_array_size 128
	char write_buffer[write_array_size];
	char read_buffer[write_array_size];
	char input_buffer[2];

	char *write_buffer_p = &write_buffer[0];
	char *read_buffer_p = &read_buffer[0];

while(1){
	printf("Atmel SPI Test Function\r\n");

	printf("Enter write string:\r\n");

	//size = read_ascii_hex_u16();

	//empty the buffers
	/*for(i = 0; i<write_array_size; i++){
		write_buffer[i] = 0x00;
		read_buffer[i] = 0x00;
    }*/



	printf("\r\n");

	//read the input

    xil_fgets(write_buffer, write_array_size);

    size = strlen(write_buffer);
    //write_buffer[size] = '\0';
    //size--;

	printf("\r\nsize: %d\r\n",size);

	printf("\r\n");

	print("Write Buffer Start: ");
	for(i = 0; i < size; i++){
	printf("%x ", write_buffer[i]);}
	printf("\r\n");


	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	gpio_bank = SPI0_GPIO_BANK;

	ss_pin = SPI0_SS1_PIN;

	mosi_pin = SPI0_MOSI_PIN;
	miso_pin = SPI0_MISO_PIN;
	clk_pin = SPI0_CLK_PIN;
	port_mask = SPI0_PORT_MASK;
	miso_pin_mask = SPI0_MISO_MASK;
	miso_shift = SPI0_MISO_SHIFT;

	XGpioPs_SetDirection(&Gpio, gpio_bank, port_mask);
	XGpioPs_SetOutputEnable(&Gpio, gpio_bank, port_mask);

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	//SS on is active low
	XGpioPs_WritePin(&Gpio, ss_pin, 0x0);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x0);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	//start a polled transfer
	invert = 0;
	if(invert){
		print("Starting inverted data transfer\r\n");
		soft_spi_transfer_invert(&Gpio, write_buffer_p, read_buffer_p,size);
	}
	else{
		soft_spi_transfer(&Gpio, write_buffer_p, read_buffer_p,size);
	}
	//printf("Clear the Select pin %d\r\n",Slave_Select_Var);

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	//printf("Clear the Select pin %d\r\n",Slave_Select_Var);
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);

	print("Read Buffer end: ");
	for(i = 0; i < size; i++){
	printf("%x ", read_buffer[i]);}
	printf("\r\n");

}
print("back to menu.\r\n");
}

