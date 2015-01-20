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
//#include <stdio.h>
//#include "definitions.h"

#include "routines.h"
#include "fixture_spi_test.h"

//#include "xparameters.h"
//#include "xil_cache.h"

//#include "xgpiops.h"
//#include "xstatus.h"
//#include "xuartps_hw.h"

#include "prodline_command.h"

/*
 * Macros
 */
/******************************************************************************/
/************************ Constants Definitions *******************************/
/******************************************************************************/
prod_command prodline_cmd_list[] = {
	{"help", "Displays all available commands.", "", prod_get_help},
	{"adapt_usb_test", "Runs the ADAPT connector USB test. Initializes the peripheral as a USB storage device.", "", adapt_usb_test},
	{"adi_loopback", "Runs the internal BIST loopback test.", "", adi_loopback_test},
	{"adi_temperature", "Outputs the temperature readings from the ADI chips.", "", adi_temperature},
	{"adi_init", "Re-initializes the AD9361 devices.", "", adi_init},
	{"asfe_rf_loopback", "Runs the built-in RF loopback test.", "", rf_loopback_test},
	{"memtest", "Runs the BIST memory test.", "", memory_test},
	{"tpm_test", "Runs the BIST TPM module test.", "", tpm_test},
	{"led_test", "Runs the BIST LED test.", "", led_test},
	{"xadc_temperature", "Outputs the temperature readings from the Zynq onboard XADC.", "", xadc_temperature},
	{"distortion_test", "Runs a distortion loopback test.", "", rftest},
};
const char prod_cmd_no = (sizeof(prodline_cmd_list) / sizeof(prod_command));

/************************** Variable Definitions *****************************/


/************************** Function Definitions *****************************/
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

void adapt_usb_test(char (*param)[50], char param_no){
	int i;
	printf("Running ADAPT USB test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	//usb_main();
}


void rf_loopback_test(char (*param)[50], char param_no){
	int i;
	int max;
	int min;
	int arg1, arg2, arg3;
	double freq;
	printf("Running rf Loopback test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	if(param_no == 4){
		sscanf(param[0],"%d",&arg1);
		sscanf(param[1],"%d",&arg2);
		sscanf(param[2],"%d",&arg3);
		sscanf(param[3],"%lf",&freq);

		//prodline_RF_loopback_test(RSSI_UPPER,RSSI_LOWER, arg1, arg2, arg3, freq);
	}else{
		printf("\r\nInvalid number of arguments; expected 4. Example usage to test all channels:\r\n'asfe_rf_loopback 2 2 2 800'\r\nFirst argument is the desired AD9361 to test, 0:ADI1, 1:ADI2, 2:both.\r\nSecond argument is the desired TX/RX paths AD9361 to test, 0:TX1/RX1, 1:TX2/RX2, 2:both.\r\nThird argument is the desired sub-path to test 0:TXA/RXA, 1:TXC/RXC, 2:both\r\nFourth Argument is the test frequency in MHz\r\n");}
}

void memory_test(char (*param)[50], char param_no){
	int i;
	printf("Running RAM tests.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	//hello_mem();

}

void adi_loopback_test(char (*param)[50], char param_no){
	int i;
	printf("Running onboard ADI loopback Test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	//ADI_Loopback_Test();

}

void adi_temperature(char (*param)[50], char param_no){
	int i;
	printf("Reading AD9361 temperature.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }

	//ADI_Get_Temperature();
}

void adi_init(char (*param)[50], char param_no){
	int i;
	printf("Initializaing AD9361.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	//ADI_initialization();

}

void tpm_test(char (*param)[50], char param_no){
	int i;
	printf("Running TPM test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	//TRSS_test();

}

void led_test(char (*param)[50], char param_no){
	int i;
	printf("Running LED test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	//hello_led();

}

void xadc_temperature(char (*param)[50], char param_no){
	int i;
	printf("Running XADC test.\r\n");
	for (i = 0; i < param_no; i++) {
        printf("Argument %i is: %s\n", i, param[i]);
    }
	//xadc_main();

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
