#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

ccflags-y+=-I$(srctree)/$(src)/bist_bsp/ps7_cortexa9_0/include/
ccflags-y+=-I$(srctree)/include/linux/

extra-y        := tdsdr_bist_app

#
# Some versions of make do not handle trailing white spaces properly;
# leading to build failures. The problem was found with GNU Make 3.80.
# Using 'strip' as a workaround for the problem.
#
ELF := $(strip $(extra-y))

extra-y += $(addsuffix .srec,$(extra-y)) $(addsuffix .bin,$(extra-y))
clean-files  := *.srec *.bin

COBJS	:= $(ELF:=.o)

LIB	= $(obj)/libstubs.o

LIBOBJS-y+= bist_bsp/ps7_cortexa9_0/libsrc/gpiops_v2_1/src/xgpiops.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/gpiops_v2_1/src/xgpiops_sinit.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/gpiops_v2_1/src/xgpiops_g.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v4_1/src/xil_assert.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v4_1/src/xil_io.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v3_10_a/src/xil_cache.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v4_1/src/sleep.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v4_1/src/usleep.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v4_1/src/xtime_l.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v4_1/src/inbyte.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v4_1/src/outbyte.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/scugic_v2_1/src/xscugic.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/gpiops_v2_1/src/xgpiops_intr.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/xadcps_v2_0/src/xadcps.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/uartps_v2_1/src/xuartps_hw.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/axidma_v8_0/src/xaxidma.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/axidma_v8_0/src/xaxidma_bdring.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/emacps_v2_0/src/xemacps_bdring.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/llfifo_v4_0/src/xllfifo_sinit.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/llfifo_v4_0/src/xllfifo.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/llfifo_v4_0/src/xllfifo_g.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/llfifo_v4_0/src/xstreamer.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/spips_v2_0/src/xspips_sinit.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/spips_v2_0/src/xspips_options.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/spips_v2_0/src/xspips_g.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/spips_v2_0/src/xspips.o

LIBOBJS-y += atof.o sbt_mod.o soft_spi_routines.o routines.o fixture_spi_test.o low_level_pdi.o adi_main.o stubs.o string.o ctype.o second.o dac_core.o adc_core.o ad9361.o ad9361_api.o rxtest.o srio_mod.o spi.o util.o platform.o console.o command.o rf_tests.o

#LIBOBJS-y += stubs.o

.SECONDARY: $(call objectify,$(COBJS))
targets += $(patsubst $(obj)/%,%,$(LIB)) $(COBJS) $(LIBOBJS-y)

LIBOBJS	:= $(addprefix $(obj)/,$(LIBOBJS-y))
ELF	:= $(addprefix $(obj)/,$(ELF))


# We don't want gcc reordering functions if possible.  This ensures that an
# application's entry point will be the first function in the application's
# source file.
ccflags-y += $(call cc-option,-fno-toplevel-reorder)

#########################################################################

quiet_cmd_link_lib = LD      $@
      cmd_link_lib = $(LD) $(ld_flags) -r -o $@ $(filter $(LIBOBJS), $^)

$(LIB):	$(LIBOBJS) FORCE
	$(call if_changed,link_lib)

quiet_cmd_link_elf = LD      $@
      cmd_link_elf = $(LD) $(LDFLAGS) -g -Ttext $(CONFIG_STANDALONE_LOAD_ADDR) \
		     -o $@ -e $(SYM_PREFIX)$(@F) $< $(LIB) $(PLATFORM_LIBGCC)

$(ELF): $(obj)/%: $(obj)/%.o $(LIB) FORCE
	$(call if_changed,link_elf)

$(obj)/%.srec: OBJCOPYFLAGS := -O srec
$(obj)/%.srec: $(obj)/% FORCE
	$(call if_changed,objcopy)

$(obj)/%.bin: OBJCOPYFLAGS := -O binary
$(obj)/%.bin: $(obj)/% FORCE
	$(call if_changed,objcopy)
