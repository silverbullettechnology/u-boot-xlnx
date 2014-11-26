source /opt/Xilinx/SDK/2014.3.1/settings64.sh
export ARCH=arm
export CROSS_COMPILE=arm-xilinx-linux-gnueabi-
export LD_LIBRARY_PATH=/home/andrew/Silver-Bullet/tdsdr-uboot/bist_bsp/ps7_cortexa9_0/lib/:$LD_LIBRARY_PATH
export LIBRARY_PATH=/home/andrew/Silver-Bullet/tdsdr-uboot/bist_bsp/ps7_cortexa9_0/lib/:$LIBRARY_PATH
export PATH=/home/andrew/Silver-Bullet/tdsdr-uboot/bist_bsp/ps7_cortexa9_0/lib/:$PATH
export PATH=/home/andrew/Silver-Bullet/tdsdr-uboot/bist_bsp/ps7_cortexa9_0/include/:$PATH
#PATH=/opt/Xilinx/SDK/2014.3.1/gnu/arm/lin/arm-xilinx-linux-gnueabi/libc/usr/include/:$PATH
export C_INCLUDE_PATH=/home/andrew/Silver-Bullet/tdsdr-uboot/bist_bsp/ps7_cortexa9_0/include/:$C_INCLUDE_PATH
export C_INCLUDE_PATH=/opt/Xilinx/SDK/2014.3.1/gnu/arm/lin/arm-xilinx-linux-gnueabi/libc/usr/include/:$C_INCLUDE_PATH
#C_INCLUDE_PATH=/home/andrew/Silver-Bullet/tdsdr-uboot/u-boot-xlnx/board/silverbullet/sdrdc/include/$C_INCLUDE_PATH

