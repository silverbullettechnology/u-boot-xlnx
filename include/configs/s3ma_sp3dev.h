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
//#define DEBUG
//#define CONFIG_DEBUG_BUILD
//#define CONFIG_RUN_ON_QEMU
//#define CONFIG_MMC_TRACE

/* Simulation in RTL */
//#define CONFIG_RTL_SIMULATION
/* Simulation on palladium */
//#define CONFIG_PALLADIUM

/* Cache settings */
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF
#define CONFIG_SYS_L2CACHE_OFF

/* Enable peripheral options */

#define CONFIG_S3MA_GPIO
#define CONFIG_S3MA_SDHCI0
//#define CONFIG_S3MA_FUSE
#define CONFIG_S3MA_SERIAL_UART0
#define CONFIG_S3MA_I2C
#define CONFIG_S3MA_SPI
//#define CONFIG_S3MA_USB
#define CONFIG_S3MA_DAC
//#define CONFIG_CMD_DFU /* firmware download gadget */
#define CONFIG_S3MA_AD9361

#define CONFIG_AD9361_MAX_DEVICE 4

/* Max sectors on one flash chip */
#define CONFIG_SYS_MAX_FLASH_SECT      512
#define CONFIG_SYS_MAX_FLASH_BANKS     1

/* Size the DRAM */
#ifdef CONFIG_PALLADIUM
# define CONFIG_S3MA_RAM_SIZE		(256 * 1024 * 1024)
#else
# define CONFIG_S3MA_RAM_SIZE		(1024 * 1024 * 1024)
#endif

/* Board LED Map */
#define GREEN_LED_GPIO                  21
#define GREEN_LED_DEV                   0
#define RED_LED_GPIO                    22
#define RED_LED_DEV                     1
#define STATUS_LED_BIT                  GREEN_LED_GPIO
#define STATUS_LED_BIT1                 RED_LED_GPIO
#define STATUS_LED_STATE                STATUS_LED_ON
#define STATUS_LED_STATE1               STATUS_LED_OFF
#define STATUS_LED_PERIOD               (CONFIG_SYS_HZ / 2)
#define STATUS_LED_PERIOD1              (CONFIG_SYS_HZ / 2)
#define STATUS_LED_BOOT                 GREEN_LED_DEV
#define STATUS_LED_RED                  RED_LED_DEV

#include "s3ma-common.h"

#endif	       /* __CONFIG_H */
