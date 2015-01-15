
make tdsdr
rm u-boot.elf
cp u-boot u-boot.elf
rm boot.bin
bootgen -image boot.bif -o i boot.bin
rm ./images/bist_app.bin
arm-xilinx-linux-gnueabi-objcopy -O binary boot.elf ./images/bist_app.bin
gzip < ./images/bist_app.bin > images/bist.bin.gz
mkimage \
	-A $ARCH \
	-O u-boot \
	-T standalone \
	-C gzip \
	-a 0x0c100000 \
	-e 0x0c100000 \
	-n "BIST" \
	-d images/bist.bin.gz \
	-v bist.img
echo "done!"

