/*
 * Configuration for the Silver Bullet Technologies SDRDC board, common
 * Adapted from Xilinx ZC70x configs
 *
 * (C) Copyright 2013 Silver Bullet Technologies
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

#ifndef __CONFIG_SDRDC_H
#define __CONFIG_SDRDC_H

// SDRDC uses a 38.4MHz VCTCXO
#define CONFIG_ZYNQ_PS_CLK_FREQ 38400000

// RAM size matches ZC702 (1GByte) using 2x16-bit parts rather than 4x8-bit
#define PHYS_SDRAM_1_SIZE (1024 * 1024 * 1024)

// No I2C, other periphs same as ZC702 
#define CONFIG_ZYNQ_SERIAL_UART1
#define CONFIG_SYS_NO_FLASH
#define CONFIG_MMC
#define CONFIG_ZYNQ_SPI

/* High Level Configuration Options */
#define CONFIG_ARMV7 /* CPU */
#define CONFIG_ZYNQ /* SoC */

#define CONFIG_SYS_SDRAM_BASE   0
#define CONFIG_SYS_SDRAM_SIZE   PHYS_SDRAM_1_SIZE

/* Total Size of Environment Sector - 64Kbyte*/
#define CONFIG_ENV_SIZE  (64 << 10)

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN      0x400000

/* Serial drivers */
#define CONFIG_BAUDRATE            115200
#define CONFIG_SYS_BAUDRATE_TABLE  { 9600, 38400, 115200, 230400 }

/* Zynq serial driver */
#ifdef CONFIG_ZYNQ_SERIAL_UART0
# define CONFIG_ZYNQ_SERIAL_BASEADDR0  0xE0000000
# define CONFIG_ZYNQ_SERIAL_BAUDRATE0  CONFIG_BAUDRATE
# define CONFIG_ZYNQ_SERIAL_CLOCK0     50000000
#endif
#ifdef CONFIG_ZYNQ_SERIAL_UART1
# define CONFIG_ZYNQ_SERIAL_BASEADDR1  0xE0001000
# define CONFIG_ZYNQ_SERIAL_BAUDRATE1  CONFIG_BAUDRATE
# define CONFIG_ZYNQ_SERIAL_CLOCK1     50000000
#endif
#if defined(CONFIG_ZYNQ_SERIAL_UART0) || defined(CONFIG_ZYNQ_SERIAL_UART1)
#define CONFIG_ZYNQ_SERIAL
#endif

/* SCU timer address is hardcoded */
#define CONFIG_SCUTIMER_BASEADDR  0xF8F00600
#ifndef CONFIG_CPU_FREQ_HZ
#define CONFIG_CPU_FREQ_HZ        800000000
#endif
#define CONFIG_SYS_HZ             1000

/* Miscellaneous configurable options */
#define CONFIG_SYS_PROMPT           "sdrdc-uboot> "
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2  "> "

#define CONFIG_CMDLINE_EDITING
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_LONGHELP
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_SYS_MAXARGS      16
#define CONFIG_SYS_CBSIZE       2048
#define CONFIG_SYS_PBSIZE       (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/* Open Firmware flat tree */
#define CONFIG_OF_LIBFDT

#include <config_cmd_default.h>

// No Ethernet on SDRDC
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS

/* QSPI */
#ifdef CONFIG_ZYNQ_SPI
# define CONFIG_SF_DEFAULT_SPEED    30000000
# define CONFIG_SPI_FLASH
# define CONFIG_SPI_FLASH_SPANSION
# define CONFIG_SPI_FLASH_STMICRO
# define CONFIG_SPI_FLASH_WINBOND
# define CONFIG_CMD_SPI
# define CONFIG_CMD_SF
#endif

/* MMC */
#ifdef CONFIG_MMC
# define CONFIG_GENERIC_MMC
# define CONFIG_SDHCI
# define CONFIG_ZYNQ_SDHCI
# define CONFIG_CMD_MMC
# define CONFIG_CMD_FAT
# define CONFIG_SUPPORT_VFAT
# define CONFIG_CMD_EXT2
# define CONFIG_DOS_PARTITION
#endif

/* Environment always in Serial Flash */
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SECT_SIZE      CONFIG_ENV_SIZE
#define CONFIG_ENV_OFFSET         0xE0000
#define CONFIG_ENV_OFFSET_REDUND  0xF0000
#define CONFIG_CMD_SAVEENV
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT

/* For development/debugging */
#ifdef DEBUG
# define CONFIG_CMD_REGINFO
# define CONFIG_PANIC_HANG
#endif

/* Default environment */
#define CONFIG_EXTRA_ENV_SETTINGS   \
	"ethaddr=00:0a:35:00:01:22\0"   \
	"kernel_name=kernel.img\0"      \
	"kernel_offs=0x100000\0"        \
	"kernel_size=0x300000\0"        \
	"kernel_load=0x03000000\0"      \
	"ramdisk_name=ramdisk.img\0"    \
	"ramdisk_offs=0x400000\0"       \
	"ramdisk_size=0x780000\0"       \
	"ramdisk_load=0x02000000\0"     \
	"devtree_name=devtree.img\0"    \
	"devtree_offs=0x0D0000\0"       \
	"devtree_size=0x010000\0"       \
	"devtree_load=0x02A00000\0"     \
	"fpga_name=fpga.img\0"          \
	"fpga_offs=0xB80000\0"          \
	"fpga_size=0x3DBAFC\0"          \
	"fpga_load=0x00100000\0"        \
	"fdt_high=0x20000000\0"         \
	"initrd_high=0x20000000\0"      \
	"sdio_dev=mmc 0\0"                                                  \
	"sdio_cmd=fatload\0"                                                \
	"qspi_fpga=echo Loading FPGA from QSPI... && "                      \
		"sf read ${fpga_load} ${fpga_offs} ${fpga_size} && "            \
		"fpga load 0 ${fpga_load} ${fpga_size}\0"                       \
	"qspi_boot=echo Loading Linux from QSPI... && "                     \
		"sf probe 0 0 0 && "                                            \
		"sf read ${kernel_load}  ${kernel_offs}  ${kernel_size}  && "   \
		"sf read ${devtree_load} ${devtree_offs} ${devtree_size} && "   \
		"sf read ${ramdisk_load} ${ramdisk_offs} ${ramdisk_size} && "   \
		"bootm ${kernel_load} ${ramdisk_load} ${devtree_load}\0"        \
	"sdio_fpga=echo Loading FPGA from ${sdio_dev}... && "               \
		"${sdio_cmd} ${sdio_dev} ${fpga_load} ${fpga_name} && "         \
		"fpga load 0 ${fpga_load} ${filesize}\0"                        \
	"sdio_boot=echo Loading Linux from ${sdio_dev}... && "              \
		"mmcinfo && "                                                   \
		"${sdio_cmd} ${sdio_dev} ${kernel_load}  ${kernel_name} && "    \
		"${sdio_cmd} ${sdio_dev} ${devtree_load} ${devtree_name} && "   \
		"${sdio_cmd} ${sdio_dev} ${ramdisk_load} ${ramdisk_name} && "   \
		"bootm ${kernel_load} ${ramdisk_load} ${devtree_load}\0"        \
	"sdio_to_qspi="                                                     \
		"mmcinfo && sf probe 0 0 0 && "                                 \
		"${sdio_cmd} ${sdio_dev} ${fpga_load} ${fpga_name} && "         \
		"sf erase ${fpga_offs} +${fpga_size} && "                       \
		"sf write ${fpga_load} ${fpga_offs} ${fpga_size} && "           \
		"${sdio_cmd} ${sdio_dev} ${kernel_load} ${kernel_name} && "     \
		"sf erase ${kernel_offs} +${kernel_size} && "                   \
		"sf write ${kernel_load} ${kernel_offs} ${kernel_size} && "     \
		"${sdio_cmd} ${sdio_dev} ${devtree_load} ${devtree_name} && "   \
		"sf erase ${devtree_offs} +${devtree_size} && "                 \
		"sf write ${devtree_load} ${devtree_offs} ${devtree_size} && "  \
		"${sdio_cmd} ${sdio_dev} ${ramdisk_load} ${ramdisk_name} && "   \
		"sf erase ${ramdisk_offs} +${ramdisk_size} && "                 \
		"sf write ${ramdisk_load} ${ramdisk_offs} ${ramdisk_size}\0"    \
	"loadbist="                                                         \
		"mmc dev 0 && "                                                 \
		"fatload mmc 0 0x100000 bist_app.bin\0"                         \
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
		"run testqspi\0 "

/* default boot is according to the bootmode switch settings */
#define CONFIG_BOOTCOMMAND      "run sdio_boot"
#define CONFIG_BOOTDELAY        3 /* -1 to Disable autoboot */
#define CONFIG_SYS_LOAD_ADDR    0 /* default? */

/* Keep L2 Cache Disabled */
#define CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_CACHELINE_SIZE  32

/* Physical Memory map */
#define CONFIG_NR_DRAM_BANKS  1
#define PHYS_SDRAM_1          0
#define CONFIG_SYS_TEXT_BASE  0x04000000

#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM_1
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + \
					PHYS_SDRAM_1_SIZE - (16 * 1024 * 1024))

#define CONFIG_SYS_INIT_RAM_ADDR    0xFFFF0000
#define CONFIG_SYS_INIT_RAM_SIZE    0x1000
#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_INIT_RAM_ADDR + \
                                    CONFIG_SYS_INIT_RAM_SIZE - \
                                    GENERATED_GBL_DATA_SIZE)

/* Enable the PL to be downloaded */
#define CONFIG_FPGA
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_ZYNQPL
#define CONFIG_CMD_FPGA

/* FIT support */
#define CONFIG_FIT          1
#define CONFIG_FIT_VERBOSE  1 /* enable fit_format_{error,warning}() */

#undef CONFIG_BOOTM_NETBSD

/* FIXME this should be removed pretty soon */
#define XPSS_QSPI_BASEADDR          0xE000D000
#define XPSS_NAND_BASEADDR          0xE1000000
#define XPSS_CRTL_PARPORT_BASEADDR  0xE000E000
#define SDIO0_BASEADDR              0xE0100000
#define SDIO1_BASEADDR              0xE0101000
#define XPSS_QSPI_LIN_BASEADDR      0xFC000000

/* Temporary - ARCH_ZYNQ in newer Linux (3.12 at least) requires multi-arch support, which
 * requires a valid machine ID.  While we get one registered we'll use the generic "Xilinx
 * Zynq" entry's ID.
 */
#define CONFIG_MACH_TYPE 3378

#endif /* __CONFIG_SDRDC_H */
