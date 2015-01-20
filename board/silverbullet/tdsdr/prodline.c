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
//#include "ctype.h"
#include <common.h>

//#define _GNU_SOURCE

//#include <stdio.h>
//#include <common.h> 
//#include "definitions.h"
#include "routines.h"



//#include "xil_cache.h"

//#include "xgpiops.h"
//#include "xstatus.h"
//#include "xuartps_hw.h"

//#include "xparameters.h"	/* defines XPAR values */
//#include "xuartps.h"

#include "routines.h"
#include "prodline_command.h"
#include "prodline.h"

/*
 * Macros
 */

/************************** Variable Definitions *****************************/

extern prod_command	  	prodline_cmd_list[];
extern char				prod_cmd_no;
//main function for production test
int prod_main(int argc, char * const argv[])
{
	//int stop = 1;
	int i;

    //char linebuffer[128];
    char arg[10][50];

    char (*arguments)[50] = arg;
    //char (*param)[50] = arg;

    char targ[50];

    int argcount = 0;

    //ADI_initialization();

    /*printf("FPGA0_CLK_CTRL (F8000170): 0x%08X\r\n", *(int*) 0xf8000170);
    printf("FPGA1_CLK_CTRL (F8000180): 0x%08X\r\n", *(int*) 0xf8000180);
    printf("FPGA2_CLK_CTRL (F8000190): 0x%08X\r\n", *(int*) 0xf8000190);
    printf("FPGA2_CLK_CTRL (F80001A0): 0x%08X\r\n", *(int*) 0xf80001A0);*/

	printf("\r\n");
	printf("\r\n");

	printf("\n\r********************************************************");
        printf("\n\r********************************************************");
	printf("\n\r**     TDSDR - Production Test Interface              **");
	printf("\n\r********************************************************");
	printf("\n\r********************************************************\r\n");
	//printf("\r\nEnter 'help' to see available commands. Enter 'stop' to return to the main menu.\r\n");

		printf("\r\n#prodline: ");

		//copy argv into arguments

		//i = argc - 2;  //count is argc minus the command

		for(argcount=0;argcount<argc - 2; argcount++){
			rsscanf(argv[argcount+2], "%s", targ);
			strncpy(arguments[argcount], targ, 50);
		}

		//now scan the command list for the specified command
		i=0;
	    while(i<prod_cmd_no){
			//printf("%s",prodline_cmd_list[i].name);
			//printf(" %d\r\n",i);
			//printf(" %s Command String\r\n",argv[1]);
			if(strcmp(argv[1],prodline_cmd_list[i].name) == 0){
				printf("\r\n");
				//execute the command if it was found.
			    prodline_cmd_list[i].function(arguments, argcount);
			    break;
			}
			i++;
		}

	    if(i >= prod_cmd_no)
	    	printf("\r\nCommand not found.\r\n");

	    //for(i=i;i<50;i++){
	    //command[i] = '\0';}

	    //for(i=i;i<128;i++){
	    //	    line[i] = '\0';}

	//adi_hw_reset();
	printf("\r\n");
    return 0;
}
