


/***************************** Include Files *********************************/

#include "definitions.h"
#include "soft_spi_routines.h"
#include "routines.h"


//static XSpiPs SpiInstance;
extern XGpioPs Gpio; //gpio instance

//extern u8 Slave_Select_Var = 3;
//extern u8 Device_Id_Var = 2;

extern u8 gpio_bank;
extern u8 ss_pin;
extern u8 mosi_pin;
extern u8 miso_pin;
extern u8 clk_pin;
extern u32 port_mask;
extern u32 miso_pin_mask;
extern u8 miso_shift;

//static u8 Slave_Select_Var = 3;
//static u8 Device_Id_Var = 2;
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
int fixture_write_read(u16 SpiDeviceId, u16 SlaveSelectVar)
{
	int Status;
	int i;

	char invert;

	int size;


	XGpioPs_Config *ConfigPtr;

	volatile int Delay;

#define write_array_size 10

	u8 write_buffer[write_array_size] = { 0 };
	u8 read_buffer[write_array_size] = { 0 };
	u8 input_buffer[2];

	u8 *write_buffer_p = &write_buffer[0];
	u8 *read_buffer_p = &read_buffer[0];
while(1){
	print("Write flash Function\r\n");
	print("Enter write array size in hex XXX\r\n");
	size = read_ascii_hex_u16();

	xil_printf("\r\n");

	if(size == 0xff)
		break;

	if(size == 0xFE){
		invert = 1;
		size = 1;}

	if(size == 0xFD){
		invert = 0;
		size = 1;}

	if (size > write_array_size)
		size = write_array_size;

	for (i=0;i<size;i++){
		xil_printf("enter value %d ", i);
		write_buffer[i] = read_ascii_hex_u16();
		xil_printf("%02x\r\n",write_buffer[i]);
	}

	xil_printf("\r\n");

	print("Write Buffer Start: ");
	for(i = 0; i < size; i++){
	xil_printf("%x ", write_buffer[i]);}
	xil_printf("\r\n");


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
	if(invert){
		print("Starting inverted data transfer\r\n");
		soft_spi_transfer_invert(&Gpio, write_buffer_p, read_buffer_p,size);
	}
	else{
		soft_spi_transfer(&Gpio, write_buffer_p, read_buffer_p,size);
	}
	//xil_printf("Clear the Select pin %d\r\n",Slave_Select_Var);

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	//xil_printf("Clear the Select pin %d\r\n",Slave_Select_Var);
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);

	print("Read Buffer end: ");
	for(i = 0; i < size; i++){
	xil_printf("%x ", read_buffer[i]);}
	xil_printf("\r\n");

}
print("back to menu.\r\n");
}

