#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

ccflags-y=-I$(srctree)/$(src)/bist_bsp/ps7_cortexa9_0/include/
ccflags-y+=-I$(srctree)/include/linux/

#FILES = bist_bsp/src/$(wildcard *.c)
#OBJS = $(patsubst %.c,%.o,$(FILES))

obj-y	+= bist_bsp/ps7_cortexa9_0/libsrc/gpiops_v1_01_a/src/xgpiops.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/gpiops_v1_01_a/src/xgpiops_sinit.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/gpiops_v1_01_a/src/xgpiops_g.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v3_10_a/src/xil_assert.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v3_10_a/src/xil_io.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v3_10_a/src/xil_cache.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/standalone_v3_10_a/src/xil_testmem.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/scugic_v1_04_a/src/xscugic.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/gpiops_v1_01_a/src/xgpiops_intr.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/xadcps_v1_01_a/src/xadcps.o \
	   bist_bsp/ps7_cortexa9_0/libsrc/uartps_v1_04_a/src/xuartps_hw.o

obj-y	+= board.o tdsdr_cmd.o routines.o low_level_pdi.o pdi_main.o stream.o xmega_pdi_nvm.o soft_spi_routines.o fixture_spi_test.o stream.o prodline.o prodline_command.o memorytest.o xadc_example.o


# Please copy ps7_init_gpl.c/h from hw project to this directory
obj-$(CONFIG_SPL_BUILD) += \
		$(if $(wildcard $(srctree)/$(src)/ps7_init_gpl.c), ps7_init_gpl.o, \
			$(if $(wildcard $(srctree)/$(src)/ps7_init.c), ps7_init.o legacy.o))

# Suppress "warning: function declaration isn't a prototype"
CFLAGS_REMOVE_ps7_init_gpl.o := -Wstrict-prototypes
CFLAGS_REMOVE_ps7_init.o := -Wstrict-prototypes

