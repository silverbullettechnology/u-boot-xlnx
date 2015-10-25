

int atmel_write_read(void);
int amc_gpio_interface(void);
int ADI_read_reg(u16 SpiDeviceId, u16 SlaveSelectVar);
int TRSS_test(void);
int read_adi_reg(void);
int ADI_write_reg(u16 SpiDeviceId, u16 SlaveSelectVar);
int write_adi_reg(void);
int ADI_write_register(unsigned char SlaveSelectVar, u16 reg, unsigned char val);
int ADI_read_register(unsigned char SlaveSelectVar, u16 reg_address, unsigned char *reg_value);
void adi_xcvr_test(void);
