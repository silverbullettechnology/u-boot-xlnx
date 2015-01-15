/* $Id: */
/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2009 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file menu.h
*
* Defines the functions and includes the proper headers for the board_test_app.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a cmc  08/27/09 First release
* </pre>
*
******************************************************************************/
/***************************** Include Files *********************************/

//#include "xparameters.h"	/* defines XPAR values */
//#include "xuartps.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "prodline.h"

/************************** Constant Definitions ****************************/


/***************** Macros (Inline Functions) Definitions *********************/


/**************************** Type Definitions ******************************/

typedef void (*prod_cmd_function)(char (*param)[50], char param_no);
typedef struct
{
	const char* name;
	const char* description;
	const char* example;
	prod_cmd_function function;
}prod_command;
/************************** Function Prototypes *****************************/


/* Displays all available commands. */
void prod_get_help(char (*param)[50], char param_no);
void adapt_usb_test(char (*param)[50], char param_no);
void rf_loopback_test(char (*param)[50], char param_no);
void memory_test(char (*param)[50], char param_no);
void adi_loopback_test(char (*param)[50], char param_no);
void adi_temperature(char (*param)[50], char param_no);
void adi_init(char (*param)[50], char param_no);
void tpm_test(char (*param)[50], char param_no);
void led_test(char (*param)[50], char param_no);
void rfinit(char (*param)[50], char param_no);
void xadc_temperature(char (*param)[50], char param_no);
void rftest(char (*param)[50], char param_no);
/************************** Variable Definitions ****************************/


