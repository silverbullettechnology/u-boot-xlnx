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
#if 0
	struct anatop_regs *anatop = (struct anatop_regs *)ANATOP_BASE_ADDR;
	u32 reg = readl(&anatop->digprog_sololite);
	u32 type = ((reg >> 16) & 0xff);

	if (type != MXC_CPU_MX6SL) {
		reg = readl(&anatop->digprog);
		struct scu_regs *scu = (struct scu_regs *)SCU_BASE_ADDR;
		u32 cfg = readl(&scu->config) & 3;
		type = ((reg >> 16) & 0xff);
		if (type == MXC_CPU_MX6DL) {
			if (!cfg)
				type = MXC_CPU_MX6SOLO;
		}

		if (type == MXC_CPU_MX6Q) {
			if (cfg == 1)
				type = MXC_CPU_MX6D;
		}

	}
	reg &= 0xff;		/* mx6 silicon revision */
	return (type << 12) | (reg + 0x10);
#else
/* This may not be needed at all, but...
 * TODO -Figure out how to get silicon revision
*/

	return 0;
#endif
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

#if 0

#ifdef CONFIG_REVISION_TAG
u32 __weak get_board_rev(void)
{
	u32 cpurev = get_cpu_rev();
	u32 type = ((cpurev >> 12) & 0xff);
	if (type == MXC_CPU_MX6SOLO)
		cpurev = (MXC_CPU_MX6DL) << 12 | (cpurev & 0xFFF);

	if (type == MXC_CPU_MX6D)
		cpurev = (MXC_CPU_MX6Q) << 12 | (cpurev & 0xFFF);

	return cpurev;
}
#endif

#endif


#if 0
static void clear_ldo_ramp(void)
{
	struct anatop_regs *anatop = (struct anatop_regs *)ANATOP_BASE_ADDR;
	int reg;

	/* ROM may modify LDO ramp up time according to fuse setting, so in
	 * order to be in the safe side we neeed to reset these settings to
	 * match the reset value: 0'b00
	 */
	reg = readl(&anatop->ana_misc2);
	reg &= ~(0x3f << 24);
	writel(reg, &anatop->ana_misc2);
}

/*
 * Set the VDDSOC
 *
 * Mask out the REG_CORE[22:18] bits (REG2_TRIG) and set
 * them to the specified millivolt level.
 * Possible values are from 0.725V to 1.450V in steps of
 * 0.025V (25mV).
 */
static int set_ldo_voltage(enum ldo_reg ldo, u32 mv)
{
	struct anatop_regs *anatop = (struct anatop_regs *)ANATOP_BASE_ADDR;
	u32 val, step, old, reg = readl(&anatop->reg_core);
	u8 shift;

	if (mv < 725)
		val = 0x00;	/* Power gated off */
	else if (mv > 1450)
		val = 0x1F;	/* Power FET switched full on. No regulation */
	else
		val = (mv - 700) / 25;

	clear_ldo_ramp();

	switch (ldo) {
	case LDO_SOC:
		shift = 18;
		break;
	case LDO_PU:
		shift = 9;
		break;
	case LDO_ARM:
		shift = 0;
		break;
	default:
		return -EINVAL;
	}

	old = (reg & (0x1F << shift)) >> shift;
	step = abs(val - old);
	if (step == 0)
		return 0;

	reg = (reg & ~(0x1F << shift)) | (val << shift);
	writel(reg, &anatop->reg_core);

	/*
	 * The LDO ramp-up is based on 64 clock cycles of 24 MHz = 2.6 us per
	 * step
	 */
	udelay(3 * step);

	return 0;
}
#endif

#if 0
static void imx_set_wdog_powerdown(bool enable)
{
	struct wdog_regs *wdog1 = (struct wdog_regs *)WDOG1_BASE_ADDR;
	struct wdog_regs *wdog2 = (struct wdog_regs *)WDOG2_BASE_ADDR;

	/* Write to the PDE (Power Down Enable) bit */
	writew(enable, &wdog1->wmcr);
	writew(enable, &wdog2->wmcr);
}
#endif

#if 0
static void set_ahb_rate(u32 val)
{
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	u32 reg, div;

	div = get_periph_clk() / val - 1;
	reg = readl(&mxc_ccm->cbcdr);

	writel((reg & (~MXC_CCM_CBCDR_AHB_PODF_MASK)) |
		(div << MXC_CCM_CBCDR_AHB_PODF_OFFSET), &mxc_ccm->cbcdr);
}
#endif

#if 0
static void clear_mmdc_ch_mask(void)
{
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	/* Clear MMDC channel mask */
	writel(0, &mxc_ccm->ccdr);
}
#endif

int arch_cpu_init(void)
{
#if 0
	init_aips();
#endif

#if 0
	/* Need to clear MMDC_CHx_MASK to make warm reset work. */
	clear_mmdc_ch_mask();

	/*
	 * When low freq boot is enabled, ROM will not set AHB
	 * freq, so we need to ensure AHB freq is 132MHz in such
	 * scenario.
	 */
	if (mxc_get_clock(MXC_ARM_CLK) == 396000000)
		set_ahb_rate(132000000);

	imx_set_wdog_powerdown(false); /* Disable PDE bit of WMCR register */
#endif

#ifdef CONFIG_APBH_DMA
	/* Start APBH DMA */
	mxs_dma_init();
#endif

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


void s_init(void)
{
	/* Disable clocks not needed */

#ifdef CONFIG_RTL_SIMULATION
	/* These clocks will need a processor reset to re-enable */
	setbits_le32(CRT_CLK_DIS, (DMA_DIS_BITMASK | USB_DIS_BITMASK | SD_DIS_BITMASK));
#endif
	setbits_le32(CRT_WFE_DIS, (WFE_AHB_DIS_BITMASK | WFE_APB_DIS_BITMASK | \
			WFE_TX_DIS_BITMASK |WFE_RX_DIS_BITMASK |WFE_ADC_DAC_DIS_BITMASK));

	setbits_le32(CRT_LLM_DIS, (LLM_MAIN_DIS_BITMASK | LLM_AHB_DIS_BITMASK | \
			LLM_APB_DIS_BITMASK | LLM_SAMPLE_DIS_BITMASK));

	setbits_le32(CRT_DICEU_DIS, (DICEU_AHB_DIS_BITMASK | DICEU_APB_DIS_BITMASK | \
			DICEU_IQ_DIS_BITMASK | DICEU_OFDM_DIS_BITMASK | DICEU_SYS_DIS_BITMASK));

	setbits_le32(CRT_RF_DIS, WFE_DIS_BITMASK);
	setbits_le32(CRT_MDM_DIS, MDM_DIS_BITMASK);

	s3ma_setup_pll();
//	s3ma_ddr_clock_enable();
//	s3ma_ddr_setup();
}


void reset_cpu(ulong addr)
{
	/* Write trigger value to SW_RESET register */
	writel(0x5B31F086, CRT_SW_RESET);
}

