/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <exports.h>
#include "routines.h"


int tdsdr_bist_app (int argc, char * const argv[])
{
	int i;

	int max;
	int min;
	int arg1, arg2, arg3;
	double freq;

	int adi_to_test;
	/* Print the ABI version */
	app_startup(argv);
	printf ("Expected ABI version %d\n", XF_VERSION);
	printf ("Actual U-Boot ABI version %d\n", (int)get_version());

	printf ("ADI Tester\n");

	printf ("argc = %d\n", argc);

	for (i=0; i<=argc; ++i) {
		printf ("argv[%d] = \"%s\"\n",
			i,
			argv[i] ? argv[i] : "<NULL>");
	}
	
	
	if (strcmp(argv[0], "int_loopback") == 0){
		printf("Running internal loopback\r\n");
	
		sscanf(argv[1],"%d",&adi_to_test);
		printf("ADI to test: %d\r\n", adi_to_test);
		if(adi_to_test == 1){
			adi_main(0);
		}
		else if(adi_to_test == 2){
			adi_main(1);
		}
		else
			printf("Incorrect ADI specified!\r\n");
	
	}

	else if (strcmp(argv[0], "adi_init") == 0){
		printf("Initializing ADI chips\r\n");
		adi_transceiver_init();
	
	}
	else if (strcmp(argv[0], "adi_temperature") == 0){
		printf("Reading ADI Temperatures.\r\n");
		ADI_Get_Temperature();
	
	}
	else if (strcmp(argv[0], "adi_reset") == 0){
		printf("Resetting AD9361.\r\n");
		adi_hw_reset();
	
	}
	else if (strcmp(argv[0], "srio_test") == 0){
		printf("Running SRIO test.\r\n");
		srio_test();
	
	}	
	else if (strcmp(argv[0], "prodline_RF_loopback_test") == 0){
		printf("Checking loopback connections.\r\n");

		sscanf(argv[0],"%d",&arg1);
		sscanf(argv[2],"%d",&arg2);
		sscanf(argv[3],"%d",&arg3);
		sscanf(argv[4],"%d",&freq);

		prodline_RF_loopback_test(arg1, arg2, arg3, freq);
	
	}
	else if (strcmp(argv[0], "hello_led") == 0){
		printf("Running LED test.\r\n");
		hello_led();
	
	}

	/*printf ("Hit any key to exit ... ");
	while (!tstc())
		;
	//consume input
	(void) getc();
	*/
	printf ("\n\n");

	Xil_DCacheDisable();
	Xil_ICacheDisable();
	return (0);
}
