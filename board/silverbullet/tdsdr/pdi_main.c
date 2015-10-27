/**
 * \file main.c
 *
 * \brief PDI programmer unit test.
 *
 * This file defines various status codes returned by functions,
 * indicating success or failure as well as what kind of failure.
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
#include <common.h>

#include "config.h"
#include "low_level_pdi.h"
#include "atxmega128a1_nvm_regs.h"
#include "xmega_pdi_nvm.h"
#include "status_codes.h"
#include "routines.h"

#include "xparameters.h"
#include "xgpiops.h"
#include "xstatus.h"
#include "xuartps_hw.h"

/* Static strings */
/* Test program defined in this file */
//#include "test_prg.h"
//static char hello_world_buf[11] = "hello world";

/* A buffer for storing read flash page data */
char page_buffer[PAGE_SIZE];

char fuses[6];
char lock_bit = 0xFF;
char dev_id[3];


#define FLASH_PAGES 544

typedef struct S_record
    {
        int s_type;
        int byte_count;
        unsigned int address;
        char data[48];
        char checksum[3];
    }S_record;

typedef struct Flash_page_buffer
    {
        unsigned int address;
        char data[PAGE_SIZE];
        int byte_count;
    }Flash_page_buffer;

typedef struct Atmel_application
    {
        unsigned int used_buffers;
        Flash_page_buffer application_data[FLASH_PAGES];
    }Atmel_application;

static S_record parse_srec_line (char * line);
static int add_srec_to_app (S_record rec, Atmel_application *app);
void print_application (Atmel_application * app);
void print_srecord (S_record rec);
void print_flash_page_buffer (Flash_page_buffer buf);
int compare_mem(char * write, char * read, int length);
unsigned char srec_calc_checksum(unsigned char * line);

/*
 * \brief Main function for PDI driver test
 *
 * This function run different programming possibilities
 * inputs are a pointer to the firmware image and the size of the image, passed from uboot.
 */
int pdi_main (char *buffer, unsigned int size)
{

  S_record srec;
  Atmel_application flash_data;
  int num_srec;
  char line_buf[64];
  char *image;
  int i,j;
  char * bol;
  char * eol;
  int line_counter;
  unsigned int addr;
  int equ;

  image = buffer;
  eol = image;
  bol = image;

  flash_data.used_buffers = 0;
  flash_data.application_data[0].address = 0;
  flash_data.application_data[0].byte_count = 0;

  //Xil_DCacheFlush();
  //Xil_DCacheDisable();
  //Xil_ICacheDisable();
  Xil_ICacheEnable();
  Xil_DCacheEnable();

  printf("\r\nStart of parsing\r\n");
  //parse the srecord file passed from uboot

    num_srec = 0;
    while(strchr(bol,'\n')){
        //printf("\r\nSet loop variables\r\n");
        eol = strchr(bol, '\n');
        i=0;
	//printf("\r\nenter subloop\r\n");
        while(bol<=eol){
            line_buf[i] = *bol;
            i++;
            bol++;
        }
        line_buf[i] = '\0';
	
	//printf("parse Record: %d\r\n\r\n",num_srec);
        srec = parse_srec_line(line_buf);
        
	//printf("add srec to app\r\n",num_srec);
        //only add type 2 records to the program
        if(srec.s_type == 2)
            add_srec_to_app(srec, &flash_data);
	else if(srec.s_type == 8)
	    break;
	else if(srec.s_type == 0xff){
	    printf(line_buf);
	    print_srecord(srec);
            printf("\r\nChecksum Error record %d\r\n", num_srec);
            return 0;
	}
        num_srec++;
    }

    printf("\r\nend of parsing\r\n");
    //print_application(&flash_data);


  /* Initialize the PDI interface */
  printf("initializing\r\n");
  xnvm_init();

  /* Read device ID */
  
  pdi_mdelay(10);
  xnvm_read_memory(XNVM_DATA_BASE + NVM_MCU_CONTROL, dev_id, 3);
  pdi_mdelay(50);

  printf("\r\nDev ID: %02X ", dev_id[0]);
  printf("%2X ", dev_id[1]);
  printf("%2X \r\n", dev_id[2]);
  printf("\r\nUsed Pages: %d\r\n",flash_data.used_buffers+1);
  xnvm_deinit();
  xnvm_init();

  /* Erase the target device */
  
  xnvm_chip_erase();
  pdi_mdelay(100);

  /* Verify that the device is erased, at least the first page */
  /*xnvm_read_memory(XNVM_FLASH_BASE, page_buffer, 128);
  pdi_mdelay(100);
  i=0;
  j = 0;*/

  //program the flash with the application image.
  

  for(i=0;i<=flash_data.used_buffers;i++){
  
	  if(flash_data.application_data[i].address >= 0x20000)
	    xnvm_erase_program_boot_flash_page(flash_data.application_data[i].address, flash_data.application_data[i].data, flash_data.application_data[i].byte_count);
	  else
	    xnvm_erase_program_flash_page(flash_data.application_data[i].address, flash_data.application_data[i].data, flash_data.application_data[i].byte_count);
	  pdi_mdelay(10);

	  //Read back the flash contents to a buffer on the device
	  addr =  XNVM_FLASH_BASE+flash_data.application_data[i].address;
	  xnvm_read_memory(addr, page_buffer, PAGE_SIZE);

	  equ = compare_mem(flash_data.application_data[i].data, page_buffer, flash_data.application_data[i].byte_count);
	  if(equ){
	    printf(".");
	    pdi_mdelay(10);
	  } else {
	    xnvm_chip_erase();
	    pdi_mdelay(100);
	    printf("\r\nProgramming error:\r\n");
		    
		    print_flash_page_buffer(flash_data.application_data[i]);

		    printf("\r\nReadback page %d: \r\n",i);
		    line_counter = 0;
		    printf("\r\n%06X    ",addr);
		    for(j=0;j<PAGE_SIZE;j++){
			if(line_counter > 15){
			    printf("\r\n%06X    ",addr+j);
			    line_counter = 0;
			    }
			printf("%02X ",page_buffer[j]);
			line_counter++;
		    }

		printf("\r\n");
	 	xnvm_deinit();
	  	xnvm_init();
		break;
	  }	
  }
  
  xnvm_deinit();
  xnvm_init();
  xnvm_pull_dev_out_of_reset();

}

int compare_mem(char * write, char * read, int length){
  int i;
  for(i=0;i<length;i++){
    if(write[i] != read[i])
      return 0;
  }
  return 1;
}

void print_flash_page_buffer (Flash_page_buffer buf){

  unsigned int addr = buf.address;
  char pointer[PAGE_SIZE];
  int cnt = buf.byte_count;
  int line_counter;
  int i;

printf("Flash byte count: %d\r\n",cnt);

    line_counter = 0;
    printf("\r\n%06X    ",addr);
    for(i=0;i<cnt;i++){
        if(line_counter > 15){
            printf("\r\n%06X    ",addr+i);
            line_counter = 0;
            }
        printf("%02X ",buf.data[i]);
        line_counter++;
    }
}

int add_srec_to_app(S_record rec, Atmel_application *app){
    unsigned int next_page_address, s_address;
    int page_byte_count, s_byte_count, i;

    s_byte_count = rec.byte_count;
    i=0;
    while(i < s_byte_count){
        page_byte_count = app->application_data[app->used_buffers].byte_count;
        next_page_address = app->application_data[app->used_buffers].address+page_byte_count;
        s_address = rec.address+i;

        //printf("s_address: %06X next_page_address: %06X, page_byte_count: %d\r\n",s_address,next_page_address,page_byte_count);
        if(s_address == next_page_address && page_byte_count < PAGE_SIZE){
            //add the next byte to the page buffer.
            app->application_data[app->used_buffers].data[page_byte_count] = rec.data[i];
            app->application_data[app->used_buffers].byte_count++;

        } else{
            //create a new page and start it at the current s record address
            app->used_buffers++;
            app->application_data[app->used_buffers].byte_count = 0;
            page_byte_count = 0;
            app->application_data[app->used_buffers].address = s_address;

            //insert the srec data
            app->application_data[app->used_buffers].data[page_byte_count] = rec.data[i];
            app->application_data[app->used_buffers].byte_count++;

        }
        i++;
    }
return 0;
}

void print_application(Atmel_application *app){
int i,j;
int line_counter;

	printf("\r\n");

	for(j=0;j<app->used_buffers+1;j++){
	    printf("\r\nFlash Page %d: \r\n",j);
	    line_counter = 0;
	    printf("\r\n%06X    ",app->application_data[j].address);
	    for(i=0;i<app->application_data[j].byte_count;i++){
		if(line_counter > 15){
		    printf("\r\n%06X    ",(app->application_data[j].address)+i);
		    line_counter = 0;
		    }
		printf("%02X ",app->application_data[j].data[i]);
		line_counter++;
	    }
	}
	printf("\r\n");
}

void print_srecord(S_record srec){
int i;
int bytecount;

    bytecount = srec.byte_count;

    printf("s_type      : ");
    printf("S%d",srec.s_type);

    //get byte count
    printf("\r\nbyte_count  : ");
    printf("%02X",srec.byte_count);

    //get address
    printf("\r\naddress     : ");
    printf("%06X",srec.address);

    //get data
    printf("\r\ndata        : ");
    i=0;
    while(i<bytecount){
    printf("%02X",srec.data[i]);
    i++;
    }

    //get checksum
    printf("\r\nchecksum    : ");
    i=0;
    while(i<2){
    printf("%c",srec.checksum[i]);
    i++;
    }

    printf("\r\n");

}

//function calculates srecord checksum
unsigned char srec_calc_checksum(unsigned char * line){
	int i = 0;
	int length;
	unsigned int tempval;
	unsigned char crc = 0;
	unsigned char templine[3];
	unsigned char * eol;

	templine[2] = '\0';

	eol = strchr(line, '\n');
	length = eol-line-3;
	
	while(i<length){

	    templine[0] = line[i];
	    templine[1] = line[i+1];

	    tempval = hexToInt(templine);

	    crc = crc+tempval;
	    i = i+2;
	}
	crc = ~crc;

	return crc;
}

//function returns a S_record struct after parsing a null terminated string
S_record parse_srec_line (char * line){
    S_record srec;
    char tmp[8] = { '\0' };
    char temp;
    char * bof;
    char * eof;
    int bytecount,bytes;
    int i;
    unsigned char crc, calc_crc;

	//printf("\r\nget record type\r\n");
    bof = line;
    //get record type
    eof = bof+2;
    tmp[0] = *(bof+1);
    tmp[1] = '\0';
    srec.s_type = hexToInt(tmp);
    bof = eof;

	//printf("\r\nget byte count\r\n");
    //get byte count
    eof = bof+2;
    i=0;
    while(bof<eof){
    tmp[i] = *bof;
    i++;
    bof++;
    }
    tmp[i+1] = '\0';
	//printf("\r\nconvert byte count to int\r\n");
    //convert byte count to integer
    srec.byte_count = hexToInt(tmp)-4;
    bytecount = srec.byte_count;

	//printf("\r\nget address\r\n");
    //get address
    eof = bof+6;
    i=0;
    while(bof<eof){
    tmp[i] = *bof;
    i++;
    bof++;
    }
    tmp[i+1] = '\0';
    srec.address = hexToInt(tmp);

	//printf("\r\nget data\r\n");
    //get data
    eof = bof+(bytecount*2);
    bytes = 0;
    while(bof<eof){
    tmp[0] = *bof;
    tmp[1] = *(bof+1);
    tmp[2] = '\0';
    temp = hexToInt(tmp);
    srec.data[bytes] = temp;
    bof = bof+2;
    bytes++;
    }

    //get checksum
    eof = bof+2;
    i=0;
    while(bof<eof){
    srec.checksum[i] = *bof;
    i++;
    bof++;
    }
    srec.checksum[2] = '\0';

    crc = hexToInt(srec.checksum);

    calc_crc = srec_calc_checksum(line);

    //if checksum is bad mark srec as bad
    if(crc != calc_crc){
        srec.s_type = 0xFF;
	printf("int CRC: %02x\r\n", crc);
	printf("calc CRC: %02x\r\n", calc_crc);
    }

    return srec;
}
