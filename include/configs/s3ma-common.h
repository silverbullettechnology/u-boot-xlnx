/*
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __S3MA_COMMON_H
#define __S3MA_COMMON_H

#include <asm/arch/s3ma-regs.h>
#include <asm/arch/gpio.h>

#define CONFIG_SYS_GENERIC_BOARD

/* System clock rates */
#define CONFIG_CPU_CLK_HZ			768000000
#define CONFIG_PERIPHCLK_HZ			(CONFIG_CPU_CLK_HZ/4)

//#define CONFIG_ARM_ERRATA_742230
//#define CONFIG_ARM_ERRATA_743622
//#define CONFIG_ARM_ERRATA_751472
#define CONFIG_ARM_ERRATA_794072
//#define CONFIG_ARM_ERRATA_761320

/*
 * High level configuration
 */
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO
#define CONFIG_TIMESTAMP		/* Print image info with timestamp */
#define CONFIG_BOARD_EARLY_INIT_F
//#define CONFIG_ARCH_EARLY_INIT_R
#define CONFIG_MISC_INIT_R
#define CONFIG_BOARD_POSTCLK_INIT
//#define CONFIG_CLOCKS

#define CONFIG_FIT
#define CONFIG_OF_LIBFDT
#define CONFIG_SYS_BOOTMAPSZ		(64 * 1024 * 1024)

/*
 * U-Boot general configurations
 */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_CBSIZE	1024		/* Console I/O buffer size */
#define CONFIG_SYS_PBSIZE	\
	(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
						/* Print buffer size */
#define CONFIG_SYS_MAXARGS	32		/* Max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE
						/* Boot argument buffer size */
#define CONFIG_VERSION_VARIABLE			/* U-BOOT version */
#define CONFIG_AUTO_COMPLETE			/* Command auto complete */
#define CONFIG_CMDLINE_EDITING			/* Command history etc */
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT	       "S3MA U-Boot > "
#define CONFIG_ENV_OVERWRITE  /* allow to overwrite serial and ethaddr addr */
#define CONFIG_BOOTDELAY	       3


/*
 * Memory configurations
 */
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE		0x80000000
#define CONFIG_SYS_SDRAM_SIZE		CONFIG_S3MA_RAM_SIZE
#define CONFIG_SYS_TEXT_BASE	    0x00000000
#define CONFIG_S3MA_OCM_RAM_BASE		(OCM_S_ABSOLUTE_BASE)
#define CONFIG_S3MA_OCM_RAM_SIZE		(OCM_S_SIZE/2) //Upper half of OCM can not be used for execution

#define CONFIG_SYS_MALLOC_LEN		(1 * 1024 * 1024)
#define CONFIG_SYS_FALLBACK_MALLOC_LEN		(32 * 1024)

#define CONFIG_SYS_INIT_RAM_ADDR	(CONFIG_S3MA_OCM_RAM_BASE + CONFIG_S3MA_OCM_RAM_SIZE) //Use upper half of OCM
#define CONFIG_SYS_INIT_RAM_SIZE	0x2000
#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR					\
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_RAM_SIZE -	\
	GENERATED_GBL_DATA_SIZE)

/* Memory test commands */
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START       (CONFIG_SYS_SDRAM_BASE)
#define CONFIG_SYS_MEMTEST_END	       (CONFIG_SYS_MEMTEST_START + CONFIG_S3MA_RAM_SIZE - 8*1024*1024)
#define CONFIG_SYS_MEMTEST_SCRATCH     0

/*
 * Standalone applications
 */
#define CONFIG_STANDALONE_LOAD_ADDR	CONFIG_SYS_SDRAM_BASE
#define CONFIG_LOADADDR			       CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_LOAD_ADDR	       CONFIG_LOADADDR
//#define CONFIG_API

/*
 * Power-on self test
 */
#ifndef CONFIG_PALLADIUM
# define CONFIG_POST	(CONFIG_SYS_POST_MEMORY)
# define CONFIG_SYS_POST_WORD_ADDR	(CONFIG_S3MA_OCM_RAM_BASE)
#endif
//#define CONFIG_POST_STD_LIST

/*
 * Cache
 */
#ifndef CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_CACHELINE_SIZE 32
#define CONFIG_SYS_L2_PL310
#define CONFIG_SYS_PL310_BASE	PL310_L2_ABSOLUTE_BASE
#endif

/*This one has to be defined by
 * http://www.arm.linux.org.uk/developer/machines/
 * We don't have the number assigned, so just pick something for now
 */
#define CONFIG_MACH_TYPE	5000

// TBD:  Do we need tags?
#if 0
#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG
#endif

/*
 * GPIO
 */
#ifdef CONFIG_S3MA_GPIO
# define CONFIG_CMD_GPIO
# define CONFIG_PL061_GPIO
# define CONFIG_GPIO_BASE	(GPIO0_APB_ABSOLUTE_BASE)
# define CONFIG_GPIO_BASE1	(GPIO1_APB_ABSOLUTE_BASE)
# define CONFIG_GPIO_BASE2	(GPIO2_APB_ABSOLUTE_BASE)
#endif

/*
 * eFuse
 */
// TBD: Fuse support
#ifdef CONFIG_S3MA_FUSE
# define CONFIG_CMD_FUSE
#endif

/*
 * UART
 */
#if defined(CONFIG_S3MA_SERIAL_UART0) || defined(CONFIG_S3MA_SERIAL_UART1)
# define CONFIG_PL011_SERIAL
# define CONFIG_PL011_CLOCK				38400000
# define CONFIG_PL01x_PORTS				\
			{(void *)CONFIG_SYS_SERIAL0,	\
			 (void *)CONFIG_SYS_SERIAL1 }

# define	CONFIG_SYS_SERIAL0	UART0_APB_ABSOLUTE_BASE
# define	CONFIG_SYS_SERIAL1	UART1_APB_ABSOLUTE_BASE
//# define CONFIG_PL011_SERIAL_FLUSH_ON_INIT

# define CONFIG_CONS_INDEX	0

#ifdef CONFIG_PALLADIUM
# define CONFIG_BAUDRATE			2064227
#else
# define CONFIG_BAUDRATE			115200
#endif

#endif

/*
 * I2C
 */
#ifdef CONFIG_S3MA_I2C
# define CONFIG_CMD_I2C
# define CONFIG_SYS_I2C
# define CONFIG_SYS_I2C_DW
# define CONFIG_SYS_I2C_SPEED		100000
# define CONFIG_SYS_I2C_BASE		I2C_APB_ABSOLUTE_BASE
# define CONFIG_SYS_I2C_SLAVE		0x90
#endif

/*
 * MMC/SD
 */
#ifdef CONFIG_S3MA_SDHCI0
# define CONFIG_SDHCI
# define CONFIG_MMC
# define CONFIG_CMD_MMC
# define CONFIG_GENERIC_MMC
# define CONFIG_MMC_SDMA
# define CONFIG_BOUNCE_BUFFER
# define CONFIG_CMD_EXT2
# define CONFIG_CMD_FAT
# define CONFIG_DOS_PARTITION
# define CONFIG_SYS_MMC_MAX_DEVICE	1
#endif

/*
 * SPI
 */
#ifdef CONFIG_S3MA_SPI
# define CONFIG_CMD_SPI
# define CONFIG_PL022_SPI
# define CONFIG_SYS_SPI_CLK		38400000
# define CONFIG_SYS_SPI_BASE	(SPI0_APB_ABSOLUTE_BASE)
# define CONFIG_SYS_SPI_BASE1	(SPI1_APB_ABSOLUTE_BASE)
# define CONFIG_SYS_SPI_BASE2	(SPI2_APB_ABSOLUTE_BASE)
# define CONFIG_SYS_SPI_BASE3	(SPI3_APB_ABSOLUTE_BASE)
# define CONFIG_SYS_SPI_BASE4	(SPI4_APB_ABSOLUTE_BASE)
#endif

/*
 * USB
 */
#ifdef CONFIG_S3MA_USB
/* USB gadget interface */
#define CONFIG_USB_GADGET
#define CONFIG_USB_GADGET_S3C_UDC_OTG
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET_VBUS_DRAW	2

#if defined(CONFIG_CMD_DFU) || defined(CONFIG_CMD_USB_MASS_STORAGE)

/* USB Composite download gadget - g_dnl */
#define CONFIG_USBDOWNLOAD_GADGET
#define CONFIG_USB_GADGET_MASS_STORAGE

#define CONFIG_DFU_FUNCTION
#define CONFIG_DFU_MMC
#define CONFIG_SYS_DFU_DATA_BUF_SIZE	(32 * 1024 * 1024)
#define DFU_DEFAULT_POLL_TIMEOUT	300

/* USB IDs */
#define CONFIG_G_DNL_VENDOR_NUM		0x0525	/* NetChip */
#define CONFIG_G_DNL_PRODUCT_NUM	0xA4A5	/* Linux-USB File-backed Storage Gadget */
#define CONFIG_G_DNL_UMS_VENDOR_NUM	CONFIG_G_DNL_VENDOR_NUM
#define CONFIG_G_DNL_UMS_PRODUCT_NUM	CONFIG_G_DNL_PRODUCT_NUM
#ifndef CONFIG_G_DNL_MANUFACTURER
#define CONFIG_G_DNL_MANUFACTURER	"Exelis"
#endif
#endif

#ifdef CONFIG_USB_ETH_RNDIS
/* USB RNDIS driver */
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_CDC
#define CONFIG_USB_ETH_RNDIS
#define CONFIG_CMD_PING
#define CONFIG_CMD_NET
#define CONFIG_CMD_LINK_LOCAL
#define CONFIG_LIB_RAND
#define CONFIG_USB_MAX_CONTROLLER_COUNT	2
#define CONFIG_USBNET_DEV_ADDR "DE:AD:BE:EF:10:10"
#define CONFIG_USBNET_HOST_ADDR "DE:AD:BE:EF:10:10"
#endif

#endif


/* Command definition */
#include <config_cmd_default.h>

#undef CONFIG_CMD_IMLS

#define CONFIG_DRIVE_TYPES "mmc "

/* Default environment */
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"ethaddr=00:0a:35:00:01:22\0"	\
	"kernel_image=uImage\0"	\
	"kernel_load_address=0x2080000\0" \
	"ramdisk_image=uramdisk.image.gz\0"	\
	"ramdisk_load_address=0x4000000\0"	\
	"devicetree_image=devicetree.dtb\0"	\
	"devicetree_load_address=0x2000000\0"	\
	"bitstream_image=system.bit.bin\0"	\
	"boot_image=BOOT.bin\0"	\
	"loadbit_addr=0x100000\0"	\
	"loadbootenv_addr=0x2000000\0" \
	"kernel_size=0x500000\0"	\
	"devicetree_size=0x20000\0"	\
	"ramdisk_size=0x5E0000\0"	\
	"boot_size=0xF00000\0"	\
	"fdt_high=0x20000000\0"	\
	"initrd_high=0x20000000\0"	\
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc 0 ${loadbootenv_addr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from SD ...; " \
		"env import -t ${loadbootenv_addr} $filesize\0" \
	"mmc_loadbit=echo Loading bitstream from SD/MMC/eMMC to RAM.. && " \
		"mmcinfo && " \
		"load mmc 0 ${loadbit_addr} ${bitstream_image} && " \
		"fpga load 0 ${loadbit_addr} ${filesize}\0" \
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
			"load mmc 0 ${kernel_load_address} ${kernel_image} && " \
			"load mmc 0 ${devicetree_load_address} ${devicetree_image} && " \
			"load mmc 0 ${ramdisk_load_address} ${ramdisk_image} && " \
			"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	"usbboot=if usb start; then " \
			"run uenvboot; " \
			"echo Copying Linux from USB to RAM... && " \
			"load usb 0 ${kernel_load_address} ${kernel_image} && " \
			"load usb 0 ${devicetree_load_address} ${devicetree_image} && " \
			"load usb 0 ${ramdisk_load_address} ${ramdisk_image} && " \
			"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	"jtagboot=echo TFTPing Linux to RAM... && " \
		"tftpboot ${kernel_load_address} ${kernel_image} && " \
		"tftpboot ${devicetree_load_address} ${devicetree_image} && " \
		"tftpboot ${ramdisk_load_address} ${ramdisk_image} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0"


/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_CMD_SF
#define CONFIG_CMD_SF_TEST
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_SST

/* SPI flash settings for boot */
#define CONFIG_SF_DEFAULT_BUS		(4)
#define CONFIG_SF_DEFAULT_CS		(GPIO33_3) //GPIO33_3
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#define CONFIG_SF_DEFAULT_SPEED	(1000000)

#if defined(CONFIG_RUN_ON_QEMU) || defined(CONFIG_PALLADIUM)
# define CONFIG_ENV_IS_IN_MMC
# define CONFIG_ENV_SIZE			(16 * 1024)
#else
# define CONFIG_ENV_IS_IN_SPI_FLASH
# define CONFIG_ENV_SIZE			(16 * 1024)
#endif

#if defined(CONFIG_ENV_IS_IN_MMC)
# define CONFIG_ENV_OFFSET		(6 * 64 * 1024)
# define CONFIG_SYS_MMC_ENV_DEV		0
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
# define CONFIG_ENV_OFFSET		(0)
# define CONFIG_ENV_SECT_SIZE	(16 * 1024)
# define CONFIG_ENV_SPI_BUS		(CONFIG_SF_DEFAULT_BUS)
# define CONFIG_ENV_SPI_CS		(CONFIG_SF_DEFAULT_CS)
# define CONFIG_ENV_SPI_MODE		(CONFIG_SF_DEFAULT_MODE)
# define CONFIG_ENV_SPI_MAX_HZ	(CONFIG_SF_DEFAULT_SPEED)
#endif

/*
 * AD9361
 */
#ifdef CONFIG_S3MA_AD9361
# define CONFIG_AD9361
# ifndef CONFIG_AD9361_MAX_DEVICE
#  define CONFIG_AD9361_MAX_DEVICE 4
# endif
# define CONFIG_AD9361_RAM_BUFFER_ADDR	(OCM_S_ABSOLUTE_BASE + CONFIG_S3MA_OCM_RAM_SIZE)
# define CONFIG_AD9361_RAM_BUFFER_SIZE	(CONFIG_S3MA_OCM_RAM_SIZE)
#endif

#endif /* __S3MA_COMMON_H */

