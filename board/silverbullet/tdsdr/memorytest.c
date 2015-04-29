/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "common.h"
#include "xparameters.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xil_testmem.h"

struct memory_range_s {
    char *name;
    char *ip;
    unsigned base;
    unsigned size;
};


struct memory_range_s memory_ranges[] = {
	{
		"ps7_ddr_0",
		"ps7_ddr",
		0x01000000,
        1070596096,
//		1072693248,
	},
	/* ps7_ram_0 memory will not be tested since application resides in the same memory */
	{
		"ps7_ram_1",
		"ps7_ram",
		0xffff0000,
		65024,
	},
};

int n_memory_ranges = 2;


void putnum(unsigned int num)
{
  char  buf[9];
  int   cnt;
  char  *ptr;
  int   digit;
  
  ptr = buf;
  for (cnt = 7 ; cnt >= 0 ; cnt--) {
    digit = (num >> (cnt * 4)) & 0xf;
    
    if (digit <= 9)
      *ptr++ = (char) ('0' + digit);
    else
      *ptr++ = (char) ('a' - 10 + digit);
  }

  *ptr = (char) 0;
  printf (buf);
}


void test_memory_range_ddr(struct memory_range_s *range) {
    XStatus status;

    /* This application uses printf statements instead of xil_printff/printff
     * to reduce the text size.
     *
     * The default linker script generated for this application does not have
     * heap memory allocated. This implies that this program cannot use any
     * routines that allocate memory on heap (printff is one such function).
     * If you'd like to add such functions, then please generate a linker script
     * that does allocate sufficient heap memory.
     */

    printf("Testing memory region: "); printf(range->name);  printf("\n\r");
    printf("    Memory Controller: "); printf(range->ip);  printf("\n\r");
    printf("         Base Address: 0x"); putnum(range->base); printf("\n\r");
    printf("                 Size: 0x"); putnum(range->size); printf (" bytes \n\r");

    status = Xil_TestMem32((u32*)range->base, 0x8000000, 0xAAAA5555, XIL_TESTMEM_ALLMEMTESTS);
    printf("          32-bit test: "); printf(status == XST_SUCCESS? "PASSED!":"FAILED!"); printf("\n\r");

    //status = Xil_TestMem16((u16*)range->base, 128*1024*2048, 0xAA55, XIL_TESTMEM_ALLMEMTESTS);
    //printf("          16-bit test: "); printf(status == XST_SUCCESS? "PASSED!":"FAILED!"); printf("\n\r");

    //status = Xil_TestMem8((u8*)range->base, 128*1024*4096, 0xA5, XIL_TESTMEM_ALLMEMTESTS);
    //printf("           8-bit test: "); printf(status == XST_SUCCESS? "PASSED!":"FAILED!"); printf("\n\r");

}

void test_memory_range_ram(struct memory_range_s *range) {
    XStatus status;

    /* This application uses printf statements instead of xil_printff/printff
     * to reduce the text size.
     *
     * The default linker script generated for this application does not have
     * heap memory allocated. This implies that this program cannot use any
     * routines that allocate memory on heap (printff is one such function).
     * If you'd like to add such functions, then please generate a linker script
     * that does allocate sufficient heap memory.
     */

    printf("Testing memory region: "); printf(range->name);  printf("\n\r");
    printf("    Memory Controller: "); printf(range->ip);  printf("\n\r");
    printf("         Base Address: 0x"); putnum(range->base); printf("\n\r");
    printf("                 Size: 0x"); putnum(range->size); printf (" bytes \n\r");

    status = Xil_TestMem32((u32*)range->base, 1024, 0xAAAA5555, XIL_TESTMEM_ALLMEMTESTS);
    printf("          32-bit test: "); printf(status == XST_SUCCESS? "PASSED!":"FAILED!"); printf("\n\r");

    //status = Xil_TestMem16((u16*)range->base, 2048, 0xAA55, XIL_TESTMEM_ALLMEMTESTS);
    //printf("          16-bit test: "); printf(status == XST_SUCCESS? "PASSED!":"FAILED!"); printf("\n\r");

    //status = Xil_TestMem8((u8*)range->base, 4096, 0xA5, XIL_TESTMEM_ALLMEMTESTS);
    //printf("           8-bit test: "); printf(status == XST_SUCCESS? "PASSED!":"FAILED!"); printf("\n\r");

}

int hello_mem()
{


    printf("\n\r********************************************************");
    printf("\n\r********************************************************");
    printf("\n\r**              SDRDC - DDR3 Memory TEST              **");
    printf("\n\r********************************************************");
    printf("\n\r********************************************************\r\n");
    printf("NOTE: This application runs with D-Cache disabled.\r\n");
    printf("As a result, cacheline requests will not be generated\r\n");

    Xil_DCacheEnable();
    test_memory_range_ddr(&memory_ranges[0]);
    test_memory_range_ram(&memory_ranges[1]);

    return 0;
}

