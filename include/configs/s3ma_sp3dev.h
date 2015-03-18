/*
 * Copyright (C) 2010-2011 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Boundary Devices Nitrogen6X
 * and Freescale i.MX6Q Sabre Lite boards.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* Custom build options for debugging  */
#define DEBUG
//#define CONFIG_DEBUG_BUILD
//#define CONFIG_RUN_ON_QEMU

/* Simulation in RTL */
//#define CONFIG_RTL_SIMULATION
/* Simulation on palladium */
#define CONFIG_PALLADIUM

/* Cache settings */
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF
#define CONFIG_SYS_L2CACHE_OFF

/* No SPI or NOR flash */
#define CONFIG_SYS_NO_FLASH

/* Enable peripheral options */

#define CONFIG_S3MA_GPIO
#define CONFIG_S3MA_SDHCI0
//#define CONFIG_S3MA_FUSE
#define CONFIG_S3MA_SERIAL_UART0
#define CONFIG_S3MA_I2C
#define CONFIG_S3MA_SPI
//#define CONFIG_S3MA_USB
//#define CONFIG_CMD_DFU /* firmware download gadget */
//#define CONFIG_S3MA_AD9361

#define CONFIG_AD9361_MAX_DEVICE 4

/* Size the DRAM */
#ifdef CONFIG_PALLADIUM
# define CONFIG_S3MA_RAM_SIZE		(256 * 1024 * 1024)
#else
# define CONFIG_S3MA_RAM_SIZE		(1024 * 1024 * 1024)
#endif

#include "s3ma-common.h"

#endif	       /* __CONFIG_H */
