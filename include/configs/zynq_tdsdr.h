/*
 * Configuration for the Silver Bullet Technologies TD-SDR board, common
 * Adapted from Xilinx ZC70x configs
 *
 * (C) Copyright 2015 Silver Bullet Technologies
 * (C) Copyright 2012 Xilinx
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_ZYNQ_TDSDR_H
#define __CONFIG_ZYNQ_TDSDR_H

/* SDRDC uses a 38.4MHz VCTCXO */
#define CONFIG_ZYNQ_PS_CLK_FREQ 40000000

/* RAM size matches ZC702 (1GByte) using 2x16-bit parts rather than 4x8-bit */
#define CONFIG_SYS_SDRAM_SIZE		(1024 * 1024 * 1024)

/* No I2C, other periphs same as ZC702 */
#define CONFIG_ZYNQ_SERIAL_UART1
#define CONFIG_ZYNQ_GEM0
#define CONFIG_ZYNQ_GEM_PHY_ADDR0	3
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ZYNQ_SDHCI0
#define CONFIG_ZYNQ_SDHCI1
#define CONFIG_ZYNQ_USB
#define CONFIG_ZYNQ_QSPI
#define CONFIG_ZYNQ_EEPROM
#define CONFIG_ZYNQ_BOOT_FREEBSD

#include <configs/zynq-common.h>

/* Override what's set in zynq-common: align env size to flash sector size and locate at
 * top of chip.  Use U-boot's protections for serial number and ethernet address. */
#undef  CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE                   0x00010000
#undef  CONFIG_ENV_OFFSET
#define CONFIG_ENV_OFFSET                 0x00FE0000
#define CONFIG_ENV_OFFSET_REDUND          0x00FF0000
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_OVERWRITE_ETHADDR_ONCE
#undef  CONFIG_ENV_OVERWRITE

/* Use sdboot by default rather than $modeboot, since FSBL failover from SD-card to QSPI
 * flash makes $modeboot problematic.  */
#undef  CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND		"run multiboot"

/* Below may be deprecated information after change to Kconfig */
/*=============================================================*/

/* Default environment */
#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"ethaddr=02:98:6E:DE:FA:00\0"	\
	"kernel_image=kernel.img\0"	\
	"kernel_load_address=0x2080000\0" \
	"devicetree_image=devtree.img\0"	\
	"devicetree_load_address=0x2000000\0"	\
	"boot_image=BOOT.bin\0"	\
	"loadbootenv_addr=0x2000000\0" \
	"kernel_size=0x940000\0"	\
	"devicetree_size=0x20000\0"	\
	"kernel_qspi_offset=0x6a0000\0"        \
	"devicetree_qspi_offset=0x680000\0"       \
	"fdt_high=0x20000000\0"	\
	"initrd_high=0x20000000\0"	\
	"sdio_dev=0\0"	\
	"usb_dev=0\0"	\
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc ${sdio_dev} ${loadbootenv_addr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from SD ...; " \
		"env import -t ${loadbootenv_addr} ${filesize}\0" \
	"multiboot=if mmc dev ${sdio_dev}; then " \
			"run sdboot; " \
		"else " \
			"run qspiboot; " \
		"fi\0" \
	"qspiboot=echo Copying Linux from QSPI flash to RAM... && " \
		"sf probe 0 0 0 && " \
		"sf read ${kernel_load_address} ${kernel_qspi_offset} ${kernel_size} && " \
		"sf read ${devicetree_load_address} ${devicetree_qspi_offset} ${devicetree_size} && " \
		"bootm ${kernel_load_address} - ${devicetree_load_address}\0" \
	"uenvboot=" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
		"fi; " \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...; " \
			"run uenvcmd; " \
		"fi\0" \
	"sdboot=" \
		"mmc dev ${sdio_dev} && "                                                 \
		"if mmcinfo; then " \
			"run uenvboot; " \
			"echo Copying Linux from SD to RAM... && " \
			"load mmc ${sdio_dev} ${kernel_load_address} ${kernel_image} && " \
			"load mmc ${sdio_dev} ${devicetree_load_address} ${devicetree_image} && " \
			"bootm ${kernel_load_address} - ${devicetree_load_address}; " \
		"fi\0" \
	"usbboot=if usb start; then " \
			"run uenvboot; " \
			"echo Copying Linux from USB to RAM... && " \
			"load usb ${usb_dev} ${kernel_load_address} ${kernel_image} && " \
			"load usb ${usb_dev} ${devicetree_load_address} ${devicetree_image} && " \
			"bootm ${kernel_load_address} - ${devicetree_load_address}; " \
		"fi\0" \
	"netboot=echo TFTPing Linux to RAM... && " \
		"tftpboot ${kernel_load_address} ${kernel_image} && " \
		"tftpboot ${devicetree_load_address} ${devicetree_image} && " \
		"bootm ${kernel_load_address} - ${devicetree_load_address}\0" \
	"sdio_to_qspi="                                                     \
		"mmc dev ${sdio_dev} && "                                                 \
		"mmcinfo && sf probe 0 0 0 && "                                 \
		"fatload mmc ${sdio_dev} ${kernel_load_address} ${boot_image} && "     \
		"sf erase 0 +${filesize} && "                   \
		"sf write ${kernel_load_address} 0 ${filesize} && "     \
		"fatload mmc ${sdio_dev} ${kernel_load_address} ${kernel_image} && "     \
		"sf erase ${kernel_qspi_offset} +${filesize} && "                   \
		"sf write ${kernel_load_address} ${kernel_qspi_offset} ${filesize} && "     \
		"fatload mmc ${sdio_dev} ${devicetree_load_address} ${devicetree_image} && "   \
		"sf erase ${devicetree_qspi_offset} +${filesize} && "                 \
		"sf write ${devicetree_load_address} ${devicetree_qspi_offset} ${filesize}\0"  \
	"loadbist="                                                         \
		"mmc dev ${sdio_dev} && "                                                 \
		"fatload mmc ${sdio_dev} 0x100000 system_top_bist.bit.bin && " \
		"fpga load 0 0x100000 ${filesize} && " \
		"fatload mmc ${sdio_dev} 0x100000 tdsdr_bist_app.img && " \
		"bootm 0x100000\0"                         \
	"bist="                                                             \
		"run loadbist && "                                              \
		"go 0x100000 \0"                                                \
	"loadbin="                                                         \
		"tftpboot 0x100000 boot.bin && "                            \
		"fatwrite mmc ${sdio_dev} 0x100000 boot.bin ${filesize} \0"                         \
	"testmmc="                                                          \
		"mmc dev 1 && "                                                 \
		"if mmc rescan; then "                                          \
			"echo MMC self-test PASS!; "                                \
		"else "                                                         \
			"echo nMMC self-test FAIL!; "                               \
		"fi\0"                                                          \
	"testsd="                                                           \
		"mmc dev 0 && "                                                 \
		"if mmc rescan; then "                                          \
			"echo SD self-test PASS!; "                                 \
		"else "                                                         \
			"echo SD self-test FAIL!; "                                 \
		"fi\0"                                                          \
	"testqspi="                                                         \
		"if sf probe 0 0 0; then "                                      \
			"echo QSPI self-test PASS!; "                               \
		"else "                                                         \
			"echo QSPI self-test FAIL!; "                               \
		"fi\0"                                                          \
	"flashtest="                                                        \
		"run testsd && "                                                \
		"run testmmc && "                                               \
		"run testqspi\0 "												\
	DFU_ALT_INFO


#endif /* __CONFIG_ZYNQ_TDSDR_H */
