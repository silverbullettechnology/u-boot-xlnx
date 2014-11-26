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

/* PDI driver includes */
#include "atxmega128a1_nvm_regs.h"
#include "xmega_pdi_nvm.h"

#include "status_codes.h"

/* Static strings */
/* Test program defined in this file */
#include "test_prg.h"
static uint8_t hello_world_buf[11] = "hello world";

/* A buffer for storing read flash page data */
uint8_t page_buffer[512];

uint8_t fuses[6];
uint8_t lock_bit = 0xFF;
uint8_t dev_id[3];

/*
 * \brief Main function for PDI driver test
 *
 * This function run different programming possibilities
 */
int pdi_main (void)
{
	/* Wait to make sure that the target device has power */
	mdelay(100);

	/* Initialize the PDI interface */
	xnvm_init();

	/* Erase the target device */
	xnvm_chip_erase();

	/* Wait to make sure that the device has been erased before
	 * next command */
	mdelay(100);

	/* Verify that the device is erased, at least the first page */
	xnvm_read_memory(XNVM_FLASH_BASE, page_buffer, 512);

	/* Program the target device using the test program supplied in the test_prg.h file */
	xnvm_erase_program_flash_page(0x0000, program, 102);

	/* Read back the flash contents to a buffer on the device */
	xnvm_read_memory(XNVM_FLASH_BASE, page_buffer, 512);

	/* Write "hello world" to the EEPROM of the target device */
	xnvm_erase_program_eeprom_page(0x0000, hello_world_buf, 11);

	/* Read the EEPROM content */
	xnvm_read_memory(XNVM_EEPROM_BASE, page_buffer, 512);

	/* Read fuses */
	xnvm_read_memory(XNVM_FUSE_BASE, fuses, 6);

	/* Option to write to fuses */
	xnvm_write_fuse_bit(0, 0xFF, 1000);
	xnvm_write_fuse_bit(1, 0x72, 1000);
	xnvm_write_fuse_bit(2, 0xFE, 1000);
	/* Fuse byte number three is always 0x00 */
	xnvm_write_fuse_bit(4, 0xFE, 1000);
	xnvm_write_fuse_bit(5, 0xE2, 1000);

	/* Read fuses */
	xnvm_read_memory(XNVM_FUSE_BASE, fuses, 6);

	/* Option to write lock bits */
	/* THIS WILL PREVENT FURTHER ACCESS TO THE DEVICE MEMORIES, RESET
	 *  BY ERASING THE DEVICE
	 */
	xnvm_write_fuse_bit(NVM_LOCKBIT_ADDR, 0x00, 1000);

	/* Read lock bits */
	xnvm_read_memory(XNVM_FUSE_BASE + NVM_LOCKBIT_ADDR, &lock_bit, 1);

	/* Read device ID */
	xnvm_read_memory(XNVM_DATA_BASE + NVM_MCU_CONTROL, dev_id, 3);

	/* Unload and stop the PDI interface */
	xnvm_deinit();

}
