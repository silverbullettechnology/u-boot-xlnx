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
#include "config.h"
#include "low_level_pdi.h"
#include "atxmega128a1_nvm_regs.h"
#include "xmega_pdi_nvm.h"
#include "status_codes.h"

/* PDI driver includes */
#include "atxmega128a1_nvm_regs.h"
#include "xmega_pdi_nvm.h"

#include "status_codes.h"

/* Static strings */
/* Test program defined in this file */
#include "test_prg.h"
static uint8_t hello_world_buf[11] = "hello world";

/* A buffer for storing read flash page data */
uint8_t page_buffer[PAGE_SIZE];

uint8_t fuses[6];
uint8_t lock_bit = 0xFF;
uint8_t dev_id[3];


#define FLASH_PAGES 544

typedef struct S_record
    {
        int s_type;
        int byte_count;
        uint32_t address;
        uint8_t data[48];
        uint8_t checksum[3];
    }S_record;

typedef struct Flash_page_buffer
    {
        uint32_t address;
        uint8_t data[PAGE_SIZE];
        int byte_count;
    }Flash_page_buffer;

typedef struct Atmel_application
    {
        uint16_t used_buffers;
        Flash_page_buffer application_data[FLASH_PAGES];
    }Atmel_application;

S_record parse_srec_line (uint8_t * line);
int add_srec_to_app (S_record rec, Atmel_application *app);
void print_application (Atmel_application * app);
void print_srecord (S_record rec);
void program_flash_page_buffer (Flash_page_buffer *buf);

/*
 * \brief Main function for PDI driver test
 *
 * This function run different programming possibilities
 * inputs are a pointer to the firmware image and the size of the image, passed from uboot.
 */
int pdi_main (uint8_t *buffer, uint16_t size)
{

  S_record srec;
  Atmel_application flash_data;
  int num_srec;
  uint8_t line_buf[64];
  uint8_t *image;
  int i,j;
  uint8_t * bol;
  uint8_t * eol;
  int line_counter;
  uint32_t addr;

  image = buffer;
  eol = image;
  bol = image;

  flash_data.used_buffers = 0;
  flash_data.application_data[0].address = 0;
  flash_data.application_data[0].byte_count = 0;


 /*        printf("printing srecord:\r\n");
	 for(i=0;i<size;i++){
	 	printf("%c",*(image+i));
	 }
	 printf("\r\n**END**\r\n");
*/
  //parse the srecord file passed from uboot

    num_srec = 0;
    while(strchr(bol,'\n')){
        eol = strchr(bol, '\n');
        i=0;
        while(bol<=eol){
            line_buf[i] = *bol;
            i++;
            bol++;
        }
        line_buf[i] = '\0';

        srec = parse_srec_line(line_buf);
        //print_srecord(srec);
        //printf("parsed Record: %d\r\n\r\n",num_srec);

        //only add type 2 records to the program
        if(srec.s_type == 2)
            add_srec_to_app(srec, &flash_data);

        num_srec++;
    }

    printf("end of parsing\r\n");
    //print_application(&flash_data);
	
  /* Wait to make sure that the target device has power */
  //pdi_mdelay(100);

  /* Initialize the PDI interface */
  //printf("initializing\r\n");
  xnvm_init();

  /* Read device ID */
  pdi_mdelay(100);
  xnvm_read_memory(XNVM_DATA_BASE + NVM_MCU_CONTROL, dev_id, 3);
  pdi_mdelay(50);

  printf("\r\nDev ID: %02X ", dev_id[0]);
  printf("%2X ", dev_id[1]);
  printf("%2X \r\n", dev_id[2]);

  /* Erase the target device */
  pdi_mdelay(100);
  xnvm_chip_erase();

  /* Wait to make sure that the device has been erased before
   * next command */
  //pdi_mdelay(100);

  /* Verify that the device is erased, at least the first page */
  xnvm_read_memory(XNVM_FLASH_BASE, page_buffer, 128);
  pdi_mdelay(100);
  i=0;
  j = 0;

//program some shit

printf("\r\nUsed Buffers: %d\r\n",flash_data.used_buffers);
  
for(i=0;i<=flash_data.used_buffers;i++){

  xnvm_erase_program_flash_page(flash_data.application_data[i].address, flash_data.application_data[i].data, flash_data.application_data[i].byte_count);
  program_flash_page_buffer(&flash_data.application_data[i]);
  pdi_mdelay(100);

  //Read back the flash contents to a buffer on the device
  /*addr =  XNVM_FLASH_BASE+flash_data.application_data[i].address;
  xnvm_read_memory(addr, page_buffer, PAGE_SIZE);
  pdi_mdelay(100);

	printf("\r\n");


	    printf("\r\nFlash Read Page %d: \r\n",i);
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

	printf("\r\n");*/

  xnvm_deinit();
  xnvm_init();
}

  /* Write "hello world" to the EEPROM of the target device */
  //xnvm_erase_program_eeprom_page(0x0000, hello_world_buf, 11);
  //pdi_mdelay(100);

  /* Read the EEPROM content */
  //xnvm_read_memory(XNVM_EEPROM_BASE, page_buffer, 512);

  /* Read fuses */
  //xnvm_read_memory(XNVM_FUSE_BASE, fuses, 6);
  //pdi_mdelay(100);

  /* Option to write to fuses */
  //xnvm_write_fuse_bit(0, 0xFF, 1000);
  //xnvm_write_fuse_bit(1, 0x72, 1000);
  //xnvm_write_fuse_bit(2, 0xFE, 1000);
  /* Fuse byte number three is always 0x00 */
  //xnvm_write_fuse_bit(4, 0xFE, 1000);
  //xnvm_write_fuse_bit(5, 0xE2, 1000);

  /* Read fuses */
  //xnvm_read_memory(XNVM_FUSE_BASE, fuses, 6);

  /* Option to write lock bits */
  /* THIS WILL PREVENT FURTHER ACCESS TO THE DEVICE MEMORIES, RESET
   *  BY ERASING THE DEVICE
   */
  //xnvm_write_fuse_bit(NVM_LOCKBIT_ADDR, 0x00, 1000);

  /* Read lock bits */
  //xnvm_read_memory(XNVM_FUSE_BASE + NVM_LOCKBIT_ADDR, &lock_bit, 1);

  //disconnect the PDI interface.
  xnvm_deinit();

}

void program_flash_page_buffer (Flash_page_buffer *buf){

  uint32_t addr = buf->address;
  uint8_t pointer[PAGE_SIZE];
  int cnt = buf->byte_count;
  int line_counter;
  int i;

for (i=0;i<cnt;i++){
	pointer[i] = buf->data[i];}

printf("\r\nprogram count: %d\r\n",cnt);

printf("\r\n");

    line_counter = 0;
    printf("\r\n%06X    ",addr);
    for(i=0;i<cnt;i++){
        if(line_counter > 15){
            printf("\r\n%06X    ",addr+i);
            line_counter = 0;
            }
        printf("%02X ",pointer[i]);
        line_counter++;
    }



  //xnvm_erase_program_flash_page(addr, pointer, cnt);
  //pdi_mdelay(100);	
}

int add_srec_to_app(S_record rec, Atmel_application *app){
    uint32_t next_page_address, s_address;
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

//function returns a S_record struct after parsing a null terminated string
S_record parse_srec_line (uint8_t * line){
    S_record srec;
    uint8_t tmp[8];
    uint8_t temp;
    uint8_t * bof;
    uint8_t * eof;
    int bytecount,bytes;
    int i;

    bof = line;
    //get record type
    eof = bof+2;
    tmp[0] = *(bof+1);
    tmp[1] = '\0';
    srec.s_type = hexToInt(tmp);
    bof = eof;

    //get byte count
    eof = bof+2;
    i=0;
    while(bof<eof){
    tmp[i] = *bof;
    i++;
    bof++;
    }
    tmp[i+1] = '\0';
    //convert byte count to integer
    srec.byte_count = hexToInt(tmp)-4;
    bytecount = srec.byte_count;

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

    return srec;
}
