


/***************************** Include Files *********************************/

#include <common.h>

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

static char Slave_Select_Var = 3;
static char Device_Id_Var = 2;
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
int atmel_write_read(void)
{
	XGpioPs_Config *ConfigPtr;	
	int Status;
	int i;

	char invert;

	int size;

	volatile int Delay;
	#define write_array_size 128
	char write_buffer[write_array_size];
	char read_buffer[write_array_size];

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

    	xil_fgets(write_buffer, write_array_size-3);

    	size = strlen(write_buffer);

   	printf("\r\nsize: %d\r\n",size);

	for(i = size; i > 0; i--){
        	write_buffer[i] = write_buffer[i-1];
    	}
    	write_buffer[0] = 0x02;
    	write_buffer[size] = 0x03;
    	size++;
    	write_buffer[size] = '\n';
	size++;

	printf("\r\n");
	printf("%s",write_buffer);
	printf("\r\nsize: %d\r\n",size);
	printf("\r\n");

	printf("Write Buffer Start: \r\n");
	for(i = 0; i < size; i++){
	printf("%x ", write_buffer[i]);}
	write_buffer[size] = '\n';
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
		printf("Starting inverted data transfer\r\n");
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

	printf("Read Buffer end: ");
	for(i = 0; i < size; i++){
	printf("%x ", read_buffer[i]);}
	read_buffer[size] = '\0';
	printf("\r\n%s\r\n",read_buffer);

}
printf("back to menu.\r\n");
}

/*****************************************************************************/
int ADI_read_reg(u16 SpiDeviceId, u16 SlaveSelectVar)
{
	int Status;
	char a;
	char b;
	char reg;
	volatile int Delay;

	XGpioPs_Config *ConfigPtr;

#define write_array_size 3

	 char write_buffer[write_array_size];
	 char read_buffer[write_array_size];
	 char input_buffer[2];

	 char *write_buffer_p = &write_buffer[0];
	 char *read_buffer_p = &read_buffer[0];
	 char *input_buffer_p = &input_buffer[0];
	 char *reg_p = &reg;

	printf("Read Register Function\r\n");
	printf("Enter register in hex XXX\r\n");

	/*
	 * write the command to the sensor.
	 */

	read(0,input_buffer,3);
	//hex_decode(input_buffer_p, 3, reg_p);

	reg = hexToInt(input_buffer);

	printf("register access: %x \r\n",reg);


	//prepare the command word
	//first 6 bits are all zero to signal a read of 1 byte.  Next nine bits are the address register, plus 8 bits for padding to recive the data back

	write_buffer[0] = 0x00;
	write_buffer[1] = reg;
	write_buffer[2] = 0x00;
	//write_buffer[3] = 0x00;

	printf("Read Buffer Start: ");
	printf("%x ", read_buffer[0]);
	printf("%x ", read_buffer[1]);
	//printf("%x ", read_buffer[2]);
	printf("%x\r\n", read_buffer[2]);

	printf("Write Buffer Start: ");
	printf("%x ", write_buffer[0]);
	printf("%x ", write_buffer[1]);
	//printf("%x ", write_buffer[2]);
	printf("%x\r\n", write_buffer[2]);

	printf("initializing SPI driver for device %d\r\n",SpiDeviceId);

	//need to set pin assignments and output mask before transfer

	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	gpio_bank = SPI0_GPIO_BANK;

	if(SlaveSelectVar == 0){
		ss_pin = SPI1_SS0_PIN;
	} else if(SlaveSelectVar==1){
		ss_pin = SPI1_SS1_PIN;
	}
	mosi_pin = SPI1_MOSI_PIN;
	miso_pin = SPI1_MISO_PIN;
	clk_pin = SPI1_CLK_PIN;
	port_mask = SPI1_PORT_MASK;
	miso_pin_mask = SPI1_MISO_MASK;
	miso_shift = SPI1_MISO_SHIFT;

	/*
	 * Set the SPI device as a master with manual start and manual
	 * chip select mode options
	 */
	printf("Setting Master mode, manual start, manual chip select\r\n");
	/*XSpiPs_SetOptions(SpiInstancePtr, XSPIPS_MANUAL_START_OPTION | \
			XSPIPS_MASTER_OPTION | XSPIPS_CLK_PHASE_1_OPTION | XSPIPS_FORCE_SSELECT_OPTION);*/

	XGpioPs_SetDirection(&Gpio, gpio_bank, port_mask);
	XGpioPs_SetOutputEnable(&Gpio, gpio_bank, port_mask);

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	//printf("Setting Slave Select pin %d\r\n",Slave_Select_Var);
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	//SS on ADI is active low
	XGpioPs_WritePin(&Gpio, ss_pin, 0x0);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x0);

	//start a polled transfer
	printf("Starting data transfer\r\n");
	soft_spi_transfer(&Gpio, write_buffer_p, read_buffer_p,write_array_size);

	printf("Read Buffer end: ");
	printf("%x ", read_buffer[0]);
	printf("%x ", read_buffer[1]);
	//printf("%x ", read_buffer[2]);
	printf("%x\r\n", read_buffer[2]);

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	//printf("Clear the Select pin %d\r\n",Slave_Select_Var);
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);



}

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
int TRSS_test(void)
{

	int Status;
	char a;
	char b;
	char reg;
	volatile int Delay;

	XGpioPs_Config *ConfigPtr;

#define write_array_size 5

	 char write_buffer[write_array_size];
	 char read_buffer[write_array_size];
	 char input_buffer[2];

	 char *write_buffer_p = &write_buffer[0];
	 char *read_buffer_p = &read_buffer[0];
	 char *input_buffer_p = &input_buffer[0];
	 char *reg_p = &reg;

	printf("\n\r********************************************************");
	printf("\n\r********************************************************");
	printf("\n\r**             TRSS Read Test                         **");
	printf("\n\r********************************************************");
	printf("\n\r********************************************************\r\n");

	write_buffer[0] = 0x05;
	write_buffer[1] = 0x00;
	write_buffer[2] = 0x00;
	write_buffer[3] = 0x00;
	write_buffer[4] = 0x00;

	read_buffer[0] = 0x00;
	read_buffer[1] = 0x00;
	read_buffer[2] = 0x00;
	read_buffer[3] = 0x00;
	read_buffer[4] = 0x00;

	printf("Write Buffer Start: ");
	printf("%x ", write_buffer[0]);
	printf("%x ", write_buffer[1]);
	printf("%x ", write_buffer[2]);
	printf("%x ", write_buffer[3]);
	printf("%x\r\n", write_buffer[4]);

	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	gpio_bank = SPI0_GPIO_BANK;

	ss_pin = SPI0_SS0_PIN;

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

	//SS on ADI is active low
	XGpioPs_WritePin(&Gpio, ss_pin, 0x0);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x0);

	//start a polled transfer
	//printf("Starting data transfer\r\n");
	soft_spi_transfer_invert(&Gpio, write_buffer_p, read_buffer_p,write_array_size);
	//printf("Clear the Select pin %d\r\n",Slave_Select_Var);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);

	for (Delay = 0; Delay < 10000000; Delay++);

	//SS on ADI is active low
	XGpioPs_WritePin(&Gpio, ss_pin, 0x0);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x0);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	soft_spi_transfer_invert(&Gpio, write_buffer_p, read_buffer_p,write_array_size);

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	//printf("Clear the Select pin %d\r\n",Slave_Select_Var);
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);

	printf("Read Buffer end: ");
	printf("%x ", read_buffer[0]);
	printf("%x ", read_buffer[1]);
	printf("%x ", read_buffer[2]);
	printf("%x ", read_buffer[4]);
	printf("%x\r\n", read_buffer[4]);


	if ((read_buffer[1] == 0x11 || read_buffer[1] == 0x13) && read_buffer[2] == 0x80)
		printf("TRSS test PASS!\r\n");
	else
		printf("TRSS test fail!\r\n");

}


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
int read_adi_reg(void)
{
	int Status;

	printf("\n\r********************************************************");
	printf("\n\r********************************************************");
	printf("\n\r**     ADI Register Read Test                         **");
	printf("\n\r********************************************************");
	printf("\n\r********************************************************\r\n");

	printf("ADI Read Register\r\n");
	//printf("Select SPI Device ID (0,1):");
	Device_Id_Var = 0;
	//Device_Id_Var = Device_Id_Var-48;
	//printf("%X\r\n", Device_Id_Var);

	printf("Select SPI Slave Select (0,1):");
	Slave_Select_Var = inbyte();
	Slave_Select_Var = Slave_Select_Var-48;
	printf("%X\r\n", Slave_Select_Var);

	/*
	 * write the command to the sensor.
	 */
	ADI_read_reg(Device_Id_Var, Slave_Select_Var);


	printf("ADI read success\r\n");

}

/******************************************************************************/
int ADI_write_reg(u16 SpiDeviceId, u16 SlaveSelectVar)
{
	int Status;
	char a;
	char b;
	u16 reg;
	unsigned char reg_u;
	unsigned char reg_l;
	char val;


	XGpioPs_Config *ConfigPtr;

	volatile int Delay;

#define write_array_size 3

	 char write_buffer[write_array_size];
	 char read_buffer[write_array_size];
	 char input_buffer[2];

	 char *write_buffer_p = &write_buffer[0];
	 char *read_buffer_p = &read_buffer[0];
	 char *input_buffer_p = &input_buffer[0];
	u16 *reg_p = &reg;
	 char *val_p = &val;

	printf("Write Register Function\r\n");
	printf("Enter register address in hex XXX\r\n");

	/*
	 * write the command to the sensor.
	 */

	read(0,input_buffer,3);
	//hex_decode(input_buffer_p, 3, reg_p);

	reg = hexToInt(input_buffer);

	reg_u = reg>>8;
	reg_l = reg&0xFF;

	printf("register address: %x \r\n",reg);

	printf("Enter value in hex\r\n");

	/*
	 * write the command to the sensor.
	 */

	read(0,input_buffer,2);
	hex_decode(input_buffer, 2, val_p);

	printf("register Value: %x \r\n",val);


	//prepare the command word
	//first 6 bits are all zero to signal a read of 1 byte.  Next nine bits are the address register, plus 16 bits for padding to recive the data back

	write_buffer[0] = (reg>>8)+0x80;
	write_buffer[1] = reg;
	write_buffer[2] = val;
	//write_buffer[3] = 0x00;

	printf("Read Buffer Start: ");
	printf("%x ", read_buffer[0]);
	printf("%x ", read_buffer[1]);
	//printf("%x ", read_buffer[2]);
	printf("%x\r\n", read_buffer[2]);

	printf("Write Buffer Start: ");
	printf("%x ", write_buffer[0]);
	printf("%x ", write_buffer[1]);
	//printf("%x ", write_buffer[2]);
	printf("%x\r\n", write_buffer[2]);

	printf("initializing SPI driver for device %d\r\n",SpiDeviceId);

	//need to set pin assignments and output mask before transfer

	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	gpio_bank = SPI0_GPIO_BANK;

	if(SlaveSelectVar == 0){
		ss_pin = SPI1_SS0_PIN;
	} else if(SlaveSelectVar==1){
		ss_pin = SPI1_SS1_PIN;
	}
	mosi_pin = SPI1_MOSI_PIN;
	miso_pin = SPI1_MISO_PIN;
	clk_pin = SPI1_CLK_PIN;
	port_mask = SPI1_PORT_MASK;
	miso_pin_mask = SPI1_MISO_MASK;
	miso_shift = SPI1_MISO_SHIFT;
	printf("Setting Master mode, manual start, manual chip select\r\n");
	/*XSpiPs_SetOptions(SpiInstancePtr, XSPIPS_MANUAL_START_OPTION | \
			XSPIPS_MASTER_OPTION | XSPIPS_CLK_PHASE_1_OPTION| XSPIPS_FORCE_SSELECT_OPTION);*/

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	printf("Setting Slave Select pin %d\r\n",Slave_Select_Var);

	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	//SS on ADI is active low
	XGpioPs_WritePin(&Gpio, ss_pin, 0x0);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x0);

	//start a polled transfer
	printf("Starting data transfer\r\n");
	soft_spi_transfer(&Gpio, write_buffer_p, read_buffer_p,write_array_size);

	printf("Read Buffer end: ");
	printf("%x ", read_buffer[0]);
	printf("%x ", read_buffer[1]);
	//printf("%x ", read_buffer[2]);
	printf("%x\r\n", read_buffer[2]);

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	printf("Clear the Select pin %d\r\n",Slave_Select_Var);
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);



}


int write_adi_reg(void)
{
	int Status;

	printf("\n\r********************************************************");
	printf("\n\r********************************************************");
	printf("\n\r**     ADI Register Write Test                        **");
	printf("\n\r********************************************************");
	printf("\n\r********************************************************\r\n");

	printf("ADI Register Write\r\n");
	//printf("Select SPI Device ID (0,1):");
	Device_Id_Var = 0;
	//Device_Id_Var = Device_Id_Var-48;
	//printf("%X\r\n", Device_Id_Var);

	printf("Select SPI Slave Select (0,1):");
	Slave_Select_Var = inbyte();
	Slave_Select_Var = Slave_Select_Var-48;
	printf("%X\r\n", Slave_Select_Var);

	/*
	 * write the command to the sensor.
	 */
	ADI_write_reg(Device_Id_Var, Slave_Select_Var);


	printf("ADI Write success\r\n");
}
/******************************************************************************/
int ADI_write_register(unsigned char SlaveSelectVar, u16 reg, unsigned char val)
{
	int Status;

	XGpioPs_Config *ConfigPtr;

	volatile int Delay;

#define write_array_size 3

	 char write_buffer[write_array_size];
	 char read_buffer[write_array_size];

	 char *write_buffer_p = &write_buffer[0];
	 char *read_buffer_p = &read_buffer[0];

	printf("Write register address: %x \r\n",reg);

	printf("register Value: %x \r\n",val);


	//prepare the command word
	//first 6 bits are all zero to signal a read of 1 byte.  Next nine bits are the address register, plus 16 bits for padding to recive the data back

	write_buffer[0] = (reg>>8)+0x80;
	write_buffer[1] = reg;
	write_buffer[2] = val;
	//write_buffer[3] = 0x00;

	printf("Write Buffer Start: ");
	printf("%x ", write_buffer[0]);
	printf("%x ", write_buffer[1]);
	//printf("%x ", write_buffer[2]);
	printf("%x\r\n", write_buffer[2]);

	//need to set pin assignments and output mask before transfer

	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	gpio_bank = SPI0_GPIO_BANK;

	if(SlaveSelectVar == 0){
		ss_pin = SPI1_SS0_PIN;
	} else if(SlaveSelectVar==1){
		ss_pin = SPI1_SS1_PIN;
	}
	mosi_pin = SPI1_MOSI_PIN;
	miso_pin = SPI1_MISO_PIN;
	clk_pin = SPI1_CLK_PIN;
	port_mask = SPI1_PORT_MASK;
	miso_pin_mask = SPI1_MISO_MASK;
	miso_shift = SPI1_MISO_SHIFT;

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */

	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	//SS on ADI is active low
	XGpioPs_WritePin(&Gpio, ss_pin, 0x0);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x0);

	//start a polled transfer
	printf("Starting data transfer\r\n");
	soft_spi_transfer(&Gpio, write_buffer_p, read_buffer_p,write_array_size);

	printf("Read Buffer end: ");
	printf("%x ", read_buffer[0]);
	printf("%x ", read_buffer[1]);
	//printf("%x ", read_buffer[2]);
	printf("%x\r\n", read_buffer[2]);

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);



}

/**********************************************************************************/
int ADI_read_register(unsigned char SlaveSelectVar, u16 reg_address, unsigned char *reg_value)
{
	int Status;
	volatile int Delay;

	XGpioPs_Config *ConfigPtr;

#define write_array_size 3

	 char write_buffer[write_array_size];
	 char read_buffer[write_array_size];

	 char *write_buffer_p = &write_buffer[0];
	 char *read_buffer_p = &read_buffer[0];

	printf("register access: %x \r\n",reg_address);


	//prepare the command word
	//first 6 bits are all zero to signal a read of 1 byte.  Next nine bits are the address register, plus 8 bits for padding to recive the data back

	write_buffer[0] = reg_address>>8;
	write_buffer[1] = reg_address;
	write_buffer[2] = 0x00;
	//write_buffer[3] = 0x00;

	/*printf("Write Buffer Start: ");
	printf("%x ", write_buffer[0]);
	printf("%x ", write_buffer[1]);
	//printf("%x ", write_buffer[2]);
	printf("%x\r\n", write_buffer[2]);
	*/

	//need to set pin assignments and output mask before transfer

	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	gpio_bank = SPI0_GPIO_BANK;

	if(SlaveSelectVar == 0){
		ss_pin = SPI1_SS0_PIN;
	} else if(SlaveSelectVar==1){
		ss_pin = SPI1_SS1_PIN;
	}
	mosi_pin = SPI1_MOSI_PIN;
	miso_pin = SPI1_MISO_PIN;
	clk_pin = SPI1_CLK_PIN;
	port_mask = SPI1_PORT_MASK;
	miso_pin_mask = SPI1_MISO_MASK;
	miso_shift = SPI1_MISO_SHIFT;

	/*
	 * Set the SPI device as a master with manual start and manual
	 * chip select mode options
	 */

	XGpioPs_SetDirection(&Gpio, gpio_bank, port_mask);
	XGpioPs_SetOutputEnable(&Gpio, gpio_bank, port_mask);

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	//printf("Setting Slave Select pin %d\r\n",Slave_Select_Var);
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	//SS on ADI is active low
	XGpioPs_WritePin(&Gpio, ss_pin, 0x0);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x0);

	//start a polled transfer
	//printf("Starting data transfer\r\n");
	soft_spi_transfer(&Gpio, write_buffer_p, read_buffer_p,write_array_size);

	printf("Read Buffer end: ");
	printf("%x ", read_buffer[0]);
	printf("%x ", read_buffer[1]);
	//printf("%x ", read_buffer[2]);
	printf("%x\r\n", read_buffer[2]);

	// set the value of the returned data
	*reg_value = read_buffer[2];

	/*
	 * Set the chip select, must use GPIO due to Zynq errata?
	 */
	//printf("Clear the Select pin %d\r\n",Slave_Select_Var);
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	XGpioPs_WritePin(&Gpio, ss_pin, 0x1);
	XGpioPs_WritePin(&Gpio, EMIO_SS0_PIN, 0x1);



}
/**********************************************************************************/
void adi_xcvr_test(void)
{
	int Status;
	unsigned char reg_value;
	u16 reg_addr;
	unsigned char test;

	unsigned char test_value1, test_value2;
	unsigned char result[2] = {0,0};


	printf("\n\r********************************************************");
	printf("\n\r********************************************************");
	printf("\n\r**     ADI Transceiver Self-Test                      **");
	printf("\n\r********************************************************");
	printf("\n\r********************************************************\r\n");

	for(test = 0; test < 2; test++){

		printf("Start Test Transceiver %d\r\n", test+1);

		//read back register 0x37 to check revision
		reg_addr = 0x37;

		ADI_read_register(test,reg_addr,&reg_value);

		printf("Device Rev: %x\r\n", reg_value);

		//read register 0x20 back on each device and make sure it matches the expected value 0x33
		//write 0x55 to register 0x36 on each device and read back.
		reg_addr = 0x20;
		test_value1 = 0;
		test_value2 = 0;
		ADI_read_register(test,reg_addr,&test_value1);

		printf("address value: %x\r\n", test_value1);

		reg_addr = 0x36;

		ADI_write_register(test,reg_addr,0x55);

		ADI_read_register(test,reg_addr,&test_value2);

		if(test_value1 == test_value2)
			result[test] = 1;
		if(test_value2 != 0x55)
			result[test] = 1;
		if(test_value1 != 0x33)
			result[test] = 1;
	}

	if (result[0])
		printf("\r\nADI transceiver 1 fail!\r\n");
	else if (result[1])
		printf("\r\nADI transceiver 2 fail!\r\n");
	else
		printf("\r\nADI Transceiver self-test PASS!\r\n");

}
