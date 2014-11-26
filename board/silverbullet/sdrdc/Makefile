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



include $(TOPDIR)/config.mk
ifneq ($(OBJTREE),$(SRCTREE))
$(shell mkdir -p $(obj)../common)
endif

LIB	= $(obj)lib$(BOARD).o

COBJS-y	:= board.o # ../../xilinx/zynq/libzynq.o


LIB_DIR = /home/andrew/Silver-Bullet/tdsdr-uboot/bist_bsp/ps7_cortexa9_0/lib/

LDFLAGS += -L$(LIB_DIR) -static

LIBFLAG += -lxil
COBJS-y += program_atmel.o low_level_pdi.o pdi_main.o stream.o xmega_pdi_nvm.o


COBJS	:= $(sort $(COBJS-y))

SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS)) $(LIBFLAG)
	

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
