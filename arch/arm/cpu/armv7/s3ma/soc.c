/*
 * (C) Copyright 2007
 * Sascha Hauer, Pengutronix
 *
 * (C) Copyright 2009 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <asm/armv7.h>
#include <asm/pl310.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/arch/s3ma-regs.h>
#include <asm/arch/boot_mode.h>
#include <asm/arch/clock.h>
#include <asm/arch/ddr.h>
#include <asm/arch/sys_proto.h>

enum ldo_reg {
	LDO_ARM,
	LDO_SOC,
	LDO_PU,
};

struct scu_regs {
	u32	ctrl;
	u32	config;
	u32	status;
	u32	invalidate;
	u32	fpga_rev;
};

const uint32_t por_config[POR_CONFIG_LEN] __attribute__ ((section (".por_config"))) = {POR_CONFIG};


u32 get_cpu_rev(void)
{
	/* This may not be needed at all, but...
	 * TODO -Figure out how to get silicon revision
	 */

	return 0;

}

#if defined(CONFIG_DISPLAY_CPUINFO)
/*
 * Print CPU information
 */
int print_cpuinfo(void)
{
	puts("CPU:   Exelis S3MA SoC\n");
	return 0;
}
#endif

int arch_cpu_init(void)
{
	s3ma_setup_pll();

	return 0;
}

int board_postclk_init(void)
{
#if 0
	set_ldo_voltage(LDO_SOC, 1175);	/* Set VDDSOC to 1.175V */

#endif
	return 0;
}

#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
	/* Avoid random hang when download by usb */
	invalidate_dcache_all();
	/* Enable D-cache. I-cache is already enabled in start.S */
	dcache_enable();
}
#endif

static inline void delay(ulong loops)
{
	__asm__ volatile ("1:\n"
		"subs %0, %1, #1\n"
		"bne 1b" : "=r" (loops) : "0" (loops));
}

void s_init(void)
{
	/* Disable clocks not needed */

	setbits_le32(CRT_CLK_DIS, (DMA_DIS_BITMASK | USB_DIS_BITMASK | SD_DIS_BITMASK));

	setbits_le32(CRT_WFE_DIS, (WFE_AHB_DIS_BITMASK | WFE_APB_DIS_BITMASK | \
			WFE_TX_DIS_BITMASK |WFE_RX_DIS_BITMASK |WFE_ADC_DAC_DIS_BITMASK));

	setbits_le32(CRT_LLM_DIS, (LLM_MAIN_DIS_BITMASK | LLM_AHB_DIS_BITMASK | \
			LLM_APB_DIS_BITMASK | LLM_SAMPLE_DIS_BITMASK));

	setbits_le32(CRT_DICEU_DIS, (DICEU_AHB_DIS_BITMASK | DICEU_APB_DIS_BITMASK | \
			DICEU_IQ_DIS_BITMASK | DICEU_OFDM_DIS_BITMASK | DICEU_SYS_DIS_BITMASK));

	setbits_le32(CRT_RF_DIS, WFE_DIS_BITMASK);
	setbits_le32(CRT_MDM_DIS, MDM_DIS_BITMASK);
/*
	Per Aragio IO spec:
	Slew Rate Control: SR = 0 – Slow, SR = 1 – Fast

	Output Drive Strength Selection (E2, E1):
	E2 (MSB) 	E1 (LSB) 	Rated Drive Strength
	0 	        0 			2mA
	0 	        1 			4mA
	1 			0 			8mA
	1 			1 			12mA
*/

	/* Initialize Boot QSPI flash drive strength to max*/
	setbits_le32(IO_CTL_E1_CFG,(uint32_t)0x00020000);
	setbits_le32(IO_CTL_E2_CFG,(uint32_t)0x00020000);
	/* Initialize Boot QSPI flash Slew Rate to fast */
	setbits_le32(IO_CTL_SR_CFG,(uint32_t)0x00020000);
	delay(1000000);
}

void reset_cpu(ulong addr)
{
	/* Write trigger value to SW_RESET register */
	writel(0x5B31F086, CRT_SW_RESET);
}

