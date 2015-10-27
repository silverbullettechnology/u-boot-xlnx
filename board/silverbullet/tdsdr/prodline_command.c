/**************************************************************************
*
*     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
*     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
*     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
*     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS
*     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*     FOR A PARTICULAR PURPOSE.
*
*     (c) Copyright 2010 Xilinx, Inc.
*     All rights reserved.
*
**************************************************************************/
/**************************************************************************
* Filename:     menu.c
*
* Description:
* Menu that runs the board test applications.
*
*
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  cmc  8-27-09  Initial Release
*
*
******************************************************************************/
#include <common.h> 

#include "routines.h"
#include "fixture_spi_test.h"

#include "prodline_command.h"

/*
 * Macros
 */
/******************************************************************************/
/************************ Constants Definitions *******************************/
/******************************************************************************/
prod_command prodline_cmd_list[] = {
	{"help", "Displays all available commands.", "", prod_get_help},
	{"abt", "Runs the basic ADI functional test.", "", adi_basic_test},
	{"aloop", "Runs the internal BIST loopback test.", "", adi_loopback_test},
	{"atemp", "Outputs the temperature readings from the ADI chips.", "", adi_temperature},
	{"ainit", "Re-initializes the AD9361 devices.", "", adi_init},
	{"prl", "Runs the built-in RF loopback test.", "", rf_loopback_test},
	{"memtest", "Runs the BIST memory test.", "", memory_test},
	{"tpm", "Runs the BIST TPM module test.", "", tpm_test},
	{"led", "Runs the BIST LED test.", "", led_test},
	{"xadc", "Outputs the temperature readings from the Zynq onboard XADC.", "", xadc_temperature},
	//{"distortion_test", "Runs a distortion loopback test.", "", rftest},
	//{"srio", "Runs a basic test of the SRIO modules", "", bist_srio_test},
	{"load", "loads the bist application to memory", "", load_bist_image}
};
const char prod_cmd_no = (sizeof(prodline_cmd_list) / sizeof(prod_command));

prod_command amc_cmd_list[] = {
	{"help", "Displays all available commands.", "", amc_get_help},
	{"xtal_sw", "Sets the source for the main system/RF clock", "", amc_set_xtal_sw},
	{"bus_sw", "Sets the condition of the backplane bus switch", "", amc_set_bus_sw},
	{"led_sw", "Sets the source of the lock LED (green)", "", amc_set_led_sw},
	{"sync_sw", "Sets the source for the external clock synchronization (PLL1)", "", amc_set_sync_sw},
	{"status", "Reads the status indicators from the AMC MCU", "", amc_get_status},
	{"reset_amc", "Resets the AMC MCU (also the Zynq)", "", amc_reset_atmel},
	{"firmware", "Reads the AMC MCU firmware version", "", amc_get_firmware},
	{"spi_test", "Opens the spi interface for the Atmel MCU", "", amc_spi_test},
	{"gpio", "Opens the gpio test interface", "", amc_gpio_test},
	{"lmk_prog", "Programs the clock module from the registers located at an address in memory", "", lmk_prog},
};
const char amc_cmd_no = (sizeof(amc_cmd_list) / sizeof(prod_command));

char bist_loaded = 0;

#define LOAD_ADDR 0xc100168
/************************** Variable Definitions *****************************/


/************************** Function Definitions *****************************/

int load_bist_image(void){
	int i;
	int retries;

	//void crc32_wd_buf(const unsigned char *input, uint ilen, unsigned char *output, uint chunk_sz);
	if(!bist_loaded){
		if (run_command("run loadbist", 0) < 1){
			bist_loaded = 1;	
			return 0;	
		} else{
			return 1;
		}
	}else{
		return 0;
	}

}

void prod_get_help(char (*param)[50], char param_no){
	int i;
	printf("Running Help Command.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	printf("available commands:\r\n");

	for(i=0;i<prod_cmd_no;i++){
		printf("'%s' %s\r\n",prodline_cmd_list[i].name, prodline_cmd_list[i].description);
	}

}

void adi_basic_test(char (*param)[50], char param_no){
	int i;
	printf("Running ADI basic test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	Xil_ICacheEnable();
	Xil_DCacheEnable();
	adi_xcvr_test();
	Xil_DCacheFlush();
	Xil_DCacheDisable();
	Xil_ICacheDisable();
}


void rf_loopback_test(char (*param)[50], char param_no){
	int i;
	int max;
	int min;
	int arg1, arg2, arg3;
	double freq;
	char * argv[5];
	printf("Running rf Loopback test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	if(param_no == 2){
		sscanf(param[0],"%d",&arg1);
		sscanf(param[1],"%d",&arg2);
		//sscanf(param[2],"%d",&arg3);
		//sscanf(param[3],"%lf",&freq);

		if (load_bist_image() < 1){

			//then execute
			//do_go_exec ((void *)addr, argc - 1, argv + 1);
			argv[0] = "prodline_RF_loopback_test";
			argv[1] = param[0];
			argv[2] = param[1];
			argv[3] = "0";
			argv[4] = "2400";
			do_go_exec ((void *)LOAD_ADDR, 4, argv);
		}

		//prodline_RF_loopback_test(RSSI_UPPER,RSSI_LOWER, arg1, arg2, arg3, freq);
	}else{
		printf("\r\nInvalid number of arguments; expected 2. Example usage to test all channels:\r\n'bist prl 2 2'\r\nFirst argument is the desired AD9361 to test, 0:ADI1, 1:ADI2, 2:both.\r\nSecond argument is the desired TX/RX paths AD9361 to test, 0:TX1/RX1, 1:TX2/RX2, 2:both.\r\n");}
}

void memory_test(char (*param)[50], char param_no){
	int i;
	printf("Running RAM tests.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	Xil_DCacheFlush();
	Xil_DCacheEnable();
	Xil_ICacheEnable();
	hello_mem();
	Xil_DCacheFlush();
	Xil_DCacheDisable();
	Xil_ICacheDisable();
	bist_loaded = 0;
}

void adi_loopback_test(char (*param)[50], char param_no){
	int i;
	
	char * argv[2];
	printf("Running onboard ADI loopback Test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	
	if(param_no == 1){
		//first load the image from some location storend in env, make sure to check result
		if (load_bist_image() < 1){

			//then execute
			//do_go_exec ((void *)addr, argc - 1, argv + 1);
			argv[0] = "int_loopback";
			argv[1] = param[0];
			do_go_exec ((void *)LOAD_ADDR, 1, argv);
		}
	} else
		printf("\r\nInvalid number of arguments; expected 1. Example usage to test adi 1:\r\n'bist adi_loopback 1\r\n");
	
}

void adi_temperature(char (*param)[50], char param_no){
	int i;
	int ret;
	char * argv[2];
	printf("Reading AD9361 temperature.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	ret = load_bist_image();
	
	if (ret < 1){	
		argv[0] = "adi_temperature";
		argv[1] = "";
		do_go_exec ((void *)LOAD_ADDR, 1, argv);
	}
}

void adi_init(char (*param)[50], char param_no){
	int i;
	char * argv[2];
	int ret;
	printf("Initializaing AD9361.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	ret = load_bist_image();
	printf("ret: %d\r\n");
	if (ret<1){	
		argv[0] = "adi_init";
		argv[1] = "";
		do_go_exec ((void *)LOAD_ADDR, 1, argv);
	}

}

void bist_srio_test(char (*param)[50], char param_no){
	int i;
	char * argv[2];
	printf("Initializaing AD9361.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }	
	if (load_bist_image() < 1){	
		argv[0] = "srio_test";
		argv[1] = "";
		do_go_exec ((void *)LOAD_ADDR, 1, argv);
	}

}

void tpm_test(char (*param)[50], char param_no){
	int i;
	printf("Running TPM test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	TRSS_test();

}

void led_test(char (*param)[50], char param_no){
	int i;
	char * argv[2];
	printf("Running LED test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }	
	if (load_bist_image() < 1){	
		argv[0] = "hello_led";
		argv[1] = "";
		Xil_ICacheEnable();
		Xil_DCacheEnable();
		do_go_exec ((void *)LOAD_ADDR, 1, argv);
		Xil_DCacheFlush();
		Xil_DCacheDisable();
		Xil_ICacheDisable();
	}	
}

void xadc_temperature(char (*param)[50], char param_no){
	int i;
	printf("Running XADC test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	Xil_ICacheEnable();
	Xil_DCacheEnable();
	xadc_main();
	Xil_DCacheFlush();
	Xil_DCacheDisable();
	Xil_ICacheDisable();

}

void rfinit(char (*param)[50], char param_no){
	int i;
	int gainoffset = 0;
	printf("param number = %d\r\n",param_no);

	printf("Running RF test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	if(param_no ==1 ){
		sscanf(param[0],"%d",&gainoffset);

		printf("gainoffset = %d\r\n",gainoffset);
		//rf_test_init(gainoffset);
	}else{
		printf("\r\nInvalid argument.\r\n");}
}

void rftest(char (*param)[50], char param_no){
	int i;
	int adi_num;
	printf("Running LED test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	if(param_no == 1){
		sscanf(param[0],"%d",&adi_num);

		//rf_test(adi_num);
	}else{
		printf("\r\nInvalid number of arguments.  Enter the desired AD9361 to test, 0:ADI1, 1:ADI2, 2:both.\r\n");}

}

/************************** AMC Function Definitions *****************************/
void amc_get_help(char (*param)[50], char param_no){
	int i;
	printf("Running Help Command.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	printf("available commands:\r\n");

	for(i=0;i<amc_cmd_no;i++){
		printf("'%s' %s\r\n",amc_cmd_list[i].name, amc_cmd_list[i].description);
	}

}

void amc_spi_test(char (*param)[50], char param_no){
	int i;
	printf("Running AMC MMC SPI test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    	}
	//Xil_ICacheEnable();
	//Xil_DCacheEnable();
	atmel_write_read();
}

void amc_gpio_test(char (*param)[50], char param_no){
	int i;
	printf("Running AMC gpio test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    	}
	//Xil_ICacheEnable();
	//Xil_DCacheEnable();
	amc_gpio_interface();
}

void lmk_prog(char (*param)[50], char param_no){
	int i;

	uint32_t buf_loc;
	char *image;

	printf("LMK Programming Utility.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    	}
	
	buf_loc = hexToInt(param[0]);
	image = (char*)buf_loc;	
	Xil_ICacheEnable();
	Xil_DCacheEnable();
	program_lmk(image);
	Xil_DCacheFlush();
	Xil_DCacheDisable();
	Xil_ICacheDisable();
}

void amc_set_xtal_sw(char (*param)[50], char param_no){
	int i, val, size;
	#define write_array_size 128
	char write_buffer[write_array_size];
	char read_buffer[write_array_size];

	printf("Setting system clock switch.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    	}

	if(param_no == 1){
		sscanf(param[0],"%d",&val);

		write_buffer[0] = 's';
		write_buffer[1] = 'x';
		write_buffer[2] = 's';
		write_buffer[3] = ' ';
		write_buffer[4] = val+48;
		size = 5;
		Xil_ICacheEnable();
		Xil_DCacheEnable();
		atmel_spi_transaction(write_buffer, read_buffer, size);
		Xil_DCacheFlush();
		Xil_DCacheDisable();
		Xil_ICacheDisable();	
		
	} else {
		printf("\r\nInvalid number of arguments; Enter the desired clock, 0:onboard, 1:TCLKA, 2:Front Panel.\r\n");
	}
}

void amc_set_bus_sw(char (*param)[50], char param_no){
	int i, val, size;
	#define write_array_size 128
	char write_buffer[write_array_size];
	char read_buffer[write_array_size];
	printf("Setting AMC backplane switches\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    	}
	if(param_no == 1){
		sscanf(param[0],"%d",&val);

		write_buffer[0] = 's';
		write_buffer[1] = 'b';
		write_buffer[2] = 's';
		write_buffer[3] = ' ';
		write_buffer[4] = val+48;
		size = 5;
		Xil_ICacheEnable();
		Xil_DCacheEnable();
		atmel_spi_transaction(write_buffer, read_buffer, size);
		Xil_DCacheFlush();
		Xil_DCacheDisable();
		Xil_ICacheDisable();
	} else {
		printf("\r\nInvalid number of arguments; Enter the backplane settings, 0:None Connected, 1:SW1, 2:SW2, 3:Both\r\n");
	}
}

void amc_set_led_sw(char (*param)[50], char param_no){
	int i, val, size;
	#define write_array_size 128
	char write_buffer[write_array_size];
	char read_buffer[write_array_size];
	printf("Setting Lock LED source\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    	}
	if(param_no == 1){
		sscanf(param[0],"%d",&val);

		write_buffer[0] = 's';
		write_buffer[1] = 'l';
		write_buffer[2] = 's';
		write_buffer[3] = ' ';
		write_buffer[4] = val+48;
		size = 5;
		Xil_ICacheEnable();
		Xil_DCacheEnable();
		atmel_spi_transaction(write_buffer, read_buffer, size);
		Xil_DCacheFlush();
		Xil_DCacheDisable();
		Xil_ICacheDisable();
	} else {
		printf("\r\nInvalid number of arguments; Enter the LED source, 0:PLL2, 1:PLL1 and PLL2, 2:NO LED\r\n");
	}
}

void amc_set_sync_sw(char (*param)[50], char param_no){
	int i, val, size;
	#define write_array_size 128
	char write_buffer[write_array_size];
	char read_buffer[write_array_size];
	printf("Setting clock synchronization source\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    	}
	if(param_no == 1){
		sscanf(param[0],"%d",&val);

		write_buffer[0] = 's';
		write_buffer[1] = 's';
		write_buffer[2] = 's';
		write_buffer[3] = ' ';
		write_buffer[4] = val+48;
		size = 5;
		Xil_ICacheEnable();
		Xil_DCacheEnable();
		atmel_spi_transaction(write_buffer, read_buffer, size);
		Xil_DCacheFlush();
		Xil_DCacheDisable();
		Xil_ICacheDisable();
	} else {
		printf("\r\nInvalid number of arguments; Enter the synchronization clock source, 0:TCLKA, 1:TCLKC, 2:Front Panel\r\n");
	}
}

void amc_reset_atmel(char (*param)[50], char param_no){
	int i, val, size;
	#define write_array_size 128
	char write_buffer[write_array_size];
	char read_buffer[write_array_size];
	printf("Resetting AMC MCU, system rebooting now\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    	}

	write_buffer[0] = 'r';
	write_buffer[1] = 's';
	write_buffer[2] = 't';
	size = 3;
	Xil_ICacheEnable();
	Xil_DCacheEnable();
	atmel_spi_transaction(write_buffer, read_buffer, size);
	Xil_DCacheFlush();
	Xil_DCacheDisable();
	Xil_ICacheDisable();

}

void amc_get_status(char (*param)[50], char param_no){
	int i, size;
	char val;
	#define write_array_size 128
	char write_buffer[write_array_size];
	char read_buffer[write_array_size];
	printf("Getting AMC MCU status\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
	}

	write_buffer[0] = 'g';
	write_buffer[1] = 's';
	write_buffer[2] = 't';
	size = 3;

	Xil_ICacheEnable();
	Xil_DCacheEnable();
	atmel_spi_transaction(write_buffer, read_buffer, size);
	Xil_DCacheFlush();
	Xil_DCacheDisable();
	Xil_ICacheDisable();

	for(i=0;i<32;i++){
		write_buffer[i] = ' ';
	}
	
	size = 16;
	
	Xil_ICacheEnable();
	Xil_DCacheEnable();
	atmel_spi_transaction(write_buffer, read_buffer, size);
	Xil_DCacheFlush();
	Xil_DCacheDisable();
	Xil_ICacheDisable();
	
	printf("LED Status : %c\r\n", read_buffer[2]);
	printf("XTAL Source: %c\r\n", read_buffer[4]);
	printf("AMC Switch : %c\r\n", read_buffer[6]);
	printf("Sync Source: %c\r\n", read_buffer[8]);
	printf("LED Source : %c\r\n", read_buffer[10]);
}

void amc_get_firmware(char (*param)[50], char param_no){
	int i, val, size;
	#define write_array_size 128
	char write_buffer[write_array_size];
	char read_buffer[write_array_size];
	printf("Getting AMC MCU firmware version\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
	}

	write_buffer[0] = 'g';
	write_buffer[1] = 'f';
	write_buffer[2] = 'w';
	size = 3;

	Xil_ICacheEnable();
	Xil_DCacheEnable();
	atmel_spi_transaction(write_buffer, read_buffer, size);
	Xil_DCacheFlush();
	Xil_DCacheDisable();
	Xil_ICacheDisable();

	for(i=0;i<127;i++){
		write_buffer[i] = ' ';
	}
	
	size = 16;
	
	Xil_ICacheEnable();
	Xil_DCacheEnable();
	atmel_spi_transaction(write_buffer, read_buffer, size);
	Xil_DCacheFlush();
	Xil_DCacheDisable();
	Xil_ICacheDisable();
	
	read_buffer[size] = '\0';
	printf("AMC Firmware Version: %c%c%c%c", read_buffer[4],read_buffer[5],read_buffer[6],read_buffer[7]);
}
