/*
 * Configuration for the Silver Bullet Technologies SDRDC board, common
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

#ifndef __CONFIG_ZYNQ_SDRDC_H
#define __CONFIG_ZYNQ_SDRDC_H

/* SDRDC uses a 38.4MHz VCTCXO */
#define CONFIG_ZYNQ_PS_CLK_FREQ 38400000

/* RAM size matches ZC702 (1GByte) using 2x16-bit parts rather than 4x8-bit */
#define CONFIG_SYS_SDRAM_SIZE		(1024 * 1024 * 1024)

/* No I2C, other periphs same as ZC702 */
#define CONFIG_ZYNQ_SERIAL_UART1

#define CONFIG_SYS_NO_FLASH

#define CONFIG_ZYNQ_SDHCI0
#define CONFIG_ZYNQ_USB
#define CONFIG_ZYNQ_QSPI
#define CONFIG_ZYNQ_EEPROM
#define CONFIG_ZYNQ_BOOT_FREEBSD

#include <configs/zynq-common.h>

/* Below may be deprecated information after change to Kconfig */
/*=============================================================*/

/* Default environment */
#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"ethaddr=00:0a:35:00:01:22\0"	\
	"kernel_image=uImage\0"	\
	"kernel_load_address=0x2080000\0" \
	"ramdisk_image=ramdisk.img\0"	\
	"ramdisk_load_address=0x4000000\0"	\
	"devicetree_image=devicetree.dtb\0"	\
	"devicetree_load_address=0x2000000\0"	\
	"bitstream_image=fpga.img\0"	\
	"boot_image=BOOT.bin\0"	\
	"loadbit_addr=0x100000\0"	\
	"loadbootenv_addr=0x2000000\0" \
	"kernel_size=0x500000\0"	\
	"devicetree_size=0x20000\0"	\
	"bitstream_size=0x3DBAFC\0"	\
	"ramdisk_size=0x5E0000\0"	\
	"boot_size=0xF00000\0"	\
	"kernel_qspi_offset=0x100000\0"        \
	"devicetree_qspi_offset=0x0D0000\0"       \
	"fpga_qspi_offset=0xB80000\0"          \
	"ramdisk_qspi_offset=0x400000\0"          \
	"fdt_high=0x20000000\0"	\
	"initrd_high=0x20000000\0"	\
	"sdio_dev=0\0"	\
	"usb_dev=0\0"	\
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc 0 ${loadbootenv_addr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from SD ...; " \
		"env import -t ${loadbootenv_addr} $filesize\0" \
	"mmc_loadbit=echo Loading bitstream from SD/MMC/eMMC to RAM.. && " \
		"mmcinfo && " \
		"load mmc ${sdio_dev} ${loadbit_addr} ${bitstream_image} && " \
		"fpga load 0 ${loadbit_addr} ${filesize}\0" \
	"qspiboot=echo Copying Linux from QSPI flash to RAM... && " \
		"sf probe 0 0 0 && " \
		"sf read ${kernel_load_address} 0x100000 ${kernel_size} && " \
		"sf read ${devicetree_load_address} 0x600000 ${devicetree_size} && " \
		"echo Copying ramdisk... && " \
		"sf read ${ramdisk_load_address} 0x620000 ${ramdisk_size} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"uenvboot=" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
		"fi; " \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...; " \
			"run uenvcmd; " \
		"fi\0" \
	"sdboot=if mmcinfo; then " \
			"run uenvboot; " \
			"echo Copying Linux from SD to RAM... && " \
			"load mmc ${sdio_dev} ${kernel_load_address} ${kernel_image} && " \
			"load mmc ${sdio_dev} ${devicetree_load_address} ${devicetree_image} && " \
			"load mmc ${sdio_dev} ${ramdisk_load_address} ${ramdisk_image} && " \
			"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	"usbboot=if usb start; then " \
			"run uenvboot; " \
			"echo Copying Linux from USB to RAM... && " \
			"load usb ${usb_dev} ${kernel_load_address} ${kernel_image} && " \
			"load usb ${usb_dev} ${devicetree_load_address} ${devicetree_image} && " \
			"load usb ${usb_dev} ${ramdisk_load_address} ${ramdisk_image} && " \
			"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	"jtagboot=echo TFTPing Linux to RAM... && " \
		"tftpboot ${kernel_load_address} ${kernel_image} && " \
		"tftpboot ${devicetree_load_address} ${devicetree_image} && " \
		"tftpboot ${ramdisk_load_address} ${ramdisk_image} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"rsa_qspiboot=echo Copying Image from QSPI flash to RAM... && " \
		"sf probe 0 0 0 && " \
		"sf read 0x100000 0x0 ${boot_size} && " \
		"zynqrsa 0x100000 && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"rsa_sdboot=echo Copying Image from SD to RAM... && " \
		"load mmc ${sdio_dev} 0x100000 ${boot_image} && " \
		"zynqrsa 0x100000 && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"rsa_jtagboot=echo TFTPing Image to RAM... && " \
		"tftpboot 0x100000 ${boot_image} && " \
		"zynqrsa 0x100000 && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"qspifpga=echo Loading FPGA from QSPI... && "                      \
		"sf read ${loadbit_addr} ${fpga_offs} ${fpga_size} && "            \
		"fpga load 0 ${fpga_load} ${fpga_size}\0"                       \
		"mmcinfo && " \
		"load mmc ${sdio_dev} ${loadbit_addr} ${bitstream_image} && " \
		"fpga load 0 ${loadbit_addr} ${filesize}\0" \
	"sdiofpga=run mmc_loadbit"               \
	"sdio_to_qspi="                                                     \
		"mmcinfo && sf probe 0 0 0 && "                                 \
		"fatload mmc ${sdio_dev} ${loadbit_addr} ${bitstream_image} && "         \
		"sf erase ${fpga_qspi_offset} +${filesize} && "                       \
		"sf write ${loadbit_addr} ${fpga_qspi_offset} ${filesize} && "           \
		"fatload mmc ${sdio_dev} ${kernel_load_address} ${kernel_image} && "     \
		"sf erase ${kernel_qspi_offset} +${filesize} && "                   \
		"sf write ${kernel_load_address} ${kernel_qspi_offset} ${filesize} && "     \
		"fatload mmc ${sdio_dev} ${devicetree_load_address} ${devicetree_image} && "   \
		"sf erase ${devicetree_qspi_offset} +${filesize} && "                 \
		"sf write ${devicetree_load_address} ${devicetree_qspi_offset} ${filesize} && "  \
		"fatload mmc ${sdio_dev} ${ramdisk_load_address} ${ramdisk_image} && "   \
		"sf erase ${ramdisk_qspi_offset} +${filesize} && "                 \
		"sf write ${ramdisk_load_address} ${ramdisk_qspi_offset} ${filesize}\0"    \
	"loadbist="                                                         \
		"mmc dev ${sdio_dev} && "                                                 \
		"fatload mmc ${sdio_dev} 0x100000 bist_app.bin\0"                         \
	"bist="                                                             \
		"run loadbist && "                                              \
		"go 0x100000 \0"                                                \
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




/* Temporary - ARCH_ZYNQ in newer Linux (3.12 at least) requires multi-arch support, which
 * requires a valid machine ID.  While we get one registered we'll use the generic "Xilinx
 * Zynq" entry's ID.
 */
#define CONFIG_MACH_TYPE 3378

#endif /* __CONFIG_ZYNQ_SDRDC_H */
