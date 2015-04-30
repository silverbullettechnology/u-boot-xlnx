#include "ad9361_api.h"
#include "parameters.h"
#include "platform.h"
#include "srio_mod.h"
#include "spi.h"


#include "command.h"
#include "console.h"


#include <xil_cache.h>
#include "xspips.h"


#include "adc_core.h"
#include "dac_core.h"


#include "sbt_mod.h"
#include "rxtest.h"
#include "sleep.h"


#define RSSI_PAUSE 95000000
#define RSSI_AVG 5
#define RSSI_TIMEOUT 15
#define RSSI_LOWER 4
#define RSSI_UPPER 25
#define DELTA1_TARGET 25
#define DELTA2_TARGET 24
#define SNR_TARGET 75

#define FFT_SIZE 2048
#define SAMPLE_OFFSET 1000
#define TIMEOUT 200
#define AVG_SIZE 10

/***************************************************************************//**
 * @brief ADI temperature function
*******************************************************************************/
extern AD9361_InitParam default_init_param;
extern AD9361_RXFIRConfig rx_fir_config;
extern AD9361_TXFIRConfig tx_fir_config;

extern struct ad9361_rf_phy *ad9361_phy_0;
extern struct ad9361_rf_phy *ad9361_phy_1;

extern char received_cmd[];

void ADI_Get_Temperature(void){

	int temp;
	int adi_num;
	struct ad9361_rf_phy *adi_phy;

	uint8_t  delay_vec[16];


	Xil_ICacheEnable();
	Xil_DCacheEnable();

#define ADI_TEMP_UPPER 95
#define ADI_TEMP_LOWER 20

	int temp1 = 0;
	int temp2 = 0;


	//int calc1, calc2;

	u8 done;
	int count;

	// make sure that PL clocks are set correctly
//	printf ("SLCR_LOCKSTA (%08x): %08x\r\n", 0xf800000C, Xil_In32(0xf800000C));
	Xil_Out32(0xf8000008, 0xdf0d); // unlock System Level Control Registers
//	printf ("SLCR_LOCKSTA (%08x): %08x\r\n", 0xf800000C, Xil_In32(0xf800000C));
	Xil_Out32(0xf8000170, 0x00100A00);
	Xil_Out32(0xf8000180, 0x00100500);
	Xil_Out32(0xf8000190, 0x00100400);
	Xil_Out32(0xf8000004, 0x767B); // lock System Level Control Registers
//	printf ("SLCR_LOCKSTA (%08x): %08x\r\n", 0xf800000C, Xil_In32(0xf800000C));

	// NOTE: The user has to choose the GPIO numbers according to desired
	// carrier board.
	default_init_param.gpio_resetb = GPIO_RESET_PIN;
	default_init_param.gpio_sync = -1;
	default_init_param.gpio_cal_sw1 = -1;
	default_init_param.gpio_cal_sw2 = -1;
	gpio_init(GPIO_DEVICE_ID);
//	gpio_direction(default_init_param.gpio_resetb, 1);


	for (temp=54; temp<(54+54); temp++)
	{
		gpio_direction(temp, 0);
	}

	printf("************ TOGGLE LED *********************\n\r");

	// SDRDC pins
	gpio_direction(LED0_pin, 1);   //
	gpio_direction(LED1_pin, 1);   //
	gpio_direction(LED2_pin, 1);   //
	gpio_direction(LED3_pin, 1);   //
	gpio_direction(AD0_reset_pin, 1);   //
	gpio_direction(AD1_reset_pin, 1);   //
	gpio_direction(SRIO_PCIE_SEL_pin, 1);   //

	gpio_direction(AD0_enable_pin, 1);   //
	gpio_direction(AD0_txnrx_pin, 1);   //
	gpio_direction(AD1_enable_pin, 1);   //
	gpio_direction(AD1_txnrx_pin, 1);   //

	gpio_data(AD0_enable_pin, 1);   //
	gpio_data(AD0_txnrx_pin, 1);   //
	gpio_data(AD1_enable_pin, 1);   //
	gpio_data(AD1_txnrx_pin, 1);   //

	printf("************ SPI INIT *********************\n\r");
	
	printf("Init SPI\r\n",done);
	HAL_SPIInit();
	set_spi_ss(0);

	CMB_SPIWriteByte (0x00C, 0x00);
	CMB_SPIWriteByte (0x00B, 0x00);
	CMB_SPIWriteByte (0x00C, 0x0F);
	CMB_SPIWriteByte (0x00D, 0x00);
	CMB_SPIWriteByte (0x00F, 0x00);
	CMB_SPIWriteByte (0x01D, 0x01);
	CMB_SPIWriteByte (0x035, 0x1E);
	CMB_SPIWriteByte (0x036, 0xFF);

	CMB_SPIReadByte(0x00C, &done);
	done = done & 0x02;

	while(!done){
		CMB_SPIReadByte(0x00C, &done);
		printf("waiting %02x\r\n",done);
		done = done & 0x02;
	}

	CMB_SPIReadByte (0x00E, &temp1);

	set_spi_ss(1);

	CMB_SPIWriteByte (0x00C, 0x00);
	CMB_SPIWriteByte (0x00B, 0x00);
	CMB_SPIWriteByte (0x00C, 0x0F);
	CMB_SPIWriteByte (0x00D, 0x00);
	CMB_SPIWriteByte (0x00F, 0x00);
	CMB_SPIWriteByte (0x01D, 0x01);
	CMB_SPIWriteByte (0x035, 0x1E);
	CMB_SPIWriteByte (0x036, 0xFF);

	CMB_SPIReadByte(0x00C, &done);
	done = done & 0x02;

	while(!done){
		CMB_SPIReadByte(0x00C, &done);
		printf("waiting %02x\r\n",done);
		done = done & 0x02;
	}

	CMB_SPIReadByte (0x00E, &temp2);

	printf("\r\nUncorrected Temperature AD1: %d\r\n", temp1);

	printf("Uncorrected Temperature AD2: %d\r\n", temp2);

	/*calc1 = temp1-64;
	calc1 = calc1/1.1;
	temp1 = calc1;

	calc2 = temp2-64;
	calc2 = calc2/1.1;
	temp2 = calc2;*/

	temp1 = (10*(temp1-64))/11;
	temp2 = (10*(temp2-64))/11;

	printf("\r\nCorrected Temperature AD1: %d\r\n",temp1);
	if(ADI_TEMP_UPPER < temp1 || ADI_TEMP_LOWER > temp1)
		printf("ADI 1 Temperature check FAIL!\r\n");
	else
		printf("ADI 1 Temperature check Pass!\r\n");

	printf("Corrected Temperature AD2: %d\r\n",temp2);
	if(ADI_TEMP_UPPER < temp2 || ADI_TEMP_LOWER > temp2)
		printf("ADI 2 Temperature check FAIL!\r\n");
	else
		printf("ADI 2 Temperature check Pass!\r\n");

	Xil_DCacheDisable();
	Xil_ICacheDisable();

	return 0;
}

void ADI_Set_XO_DAC(void){

	set_spi_ss(0);
	CMB_SPIWriteByte(0x26,0x80);
	CMB_SPIWriteByte(0x23,0x3F);
	CMB_SPIWriteByte(0x18,0xC8);
	CMB_SPIWriteByte(0x19,0xC8);
	CMB_SPIWriteByte(0x1A,0x10);
	CMB_SPIWriteByte(0x1B,0x10);
	set_spi_ss(1);
	CMB_SPIWriteByte(0x26,0x80);
	CMB_SPIWriteByte(0x23,0x3F);
	CMB_SPIWriteByte(0x18,0xC8);
	CMB_SPIWriteByte(0x19,0xC8);
	CMB_SPIWriteByte(0x1A,0x10);
	CMB_SPIWriteByte(0x1B,0x10);
}
/***************************************************************************//**
 * @brief ADI hw reset function
*******************************************************************************/
void adi_hw_reset(){

	int temp;
	int adi_num;
	int errors;
	struct ad9361_rf_phy *adi_phy;

	uint8_t  delay_vec[16];
	
	Xil_ICacheEnable();
	Xil_DCacheEnable();

	// make sure that PL clocks are set correctly
//	printf ("SLCR_LOCKSTA (%08x): %08x\r\n", 0xf800000C, Xil_In32(0xf800000C));
	Xil_Out32(0xf8000008, 0xdf0d); // unlock System Level Control Registers
//	printf ("SLCR_LOCKSTA (%08x): %08x\r\n", 0xf800000C, Xil_In32(0xf800000C));
	Xil_Out32(0xf8000170, 0x00100A00);
	Xil_Out32(0xf8000180, 0x00100500);
	Xil_Out32(0xf8000190, 0x00100400);
	Xil_Out32(0xf8000004, 0x767B); // lock System Level Control Registers
//	printf ("SLCR_LOCKSTA (%08x): %08x\r\n", 0xf800000C, Xil_In32(0xf800000C));



	// NOTE: The user has to choose the GPIO numbers according to desired
	// carrier board.
	default_init_param.gpio_resetb = GPIO_RESET_PIN;
	default_init_param.gpio_sync = -1;
	default_init_param.gpio_cal_sw1 = -1;
	default_init_param.gpio_cal_sw2 = -1;
	gpio_init(GPIO_DEVICE_ID);
//	gpio_direction(default_init_param.gpio_resetb, 1);


	for (temp=54; temp<(54+54); temp++)
	{
		gpio_direction(temp, 0);
	}


	// SDRDC pins
	gpio_direction(LED0_pin, 1);   //
	gpio_direction(LED1_pin, 1);   //
	gpio_direction(LED2_pin, 1);   //
	gpio_direction(LED3_pin, 1);   //
	gpio_direction(AD0_reset_pin, 1);   //
	gpio_direction(AD1_reset_pin, 1);   //
	gpio_direction(SRIO_PCIE_SEL_pin, 1);   //

	gpio_direction(AD0_enable_pin, 1);   //
	gpio_direction(AD0_txnrx_pin, 1);   //
	gpio_direction(AD1_enable_pin, 1);   //
	gpio_direction(AD1_txnrx_pin, 1);   //

	gpio_data(AD0_enable_pin, 1);   //
	gpio_data(AD0_txnrx_pin, 1);   //
	gpio_data(AD1_enable_pin, 1);   //
	gpio_data(AD1_txnrx_pin, 1);   //

	gpio_data(LED0_pin, 1);
	gpio_data(AD0_reset_pin, 1);
	gpio_data(AD1_reset_pin, 1);

	udelay(10000);

	gpio_data(LED0_pin, 0);
	gpio_data(AD0_reset_pin, 0);
	gpio_data(AD1_reset_pin, 0);

	udelay(10000);

	gpio_data(LED0_pin, 1);
	gpio_data(AD0_reset_pin, 1);
	gpio_data(AD1_reset_pin, 1);
}


int measure_rssi_loopback(struct ad9361_rf_phy *phy, int channel){
	//measures the AGC register until the standard deviation is small and then returns the mean.

	u8 temp1;
	int meas1_std, meas1_avg;

	int i,j, delay;

	int meas_arr[RSSI_AVG] = { 0 };

	//measure channel 1
	meas1_std = 99;
	meas1_avg = 0;
	i=0;
	while(i<RSSI_TIMEOUT){
		//reset the AGC
		ad9361_set_en_state_machine_mode(phy,ENSM_STATE_ALERT);
		//ENSM move to FDD
		ad9361_set_en_state_machine_mode(phy,ENSM_STATE_FDD);
		for (delay = 0; delay < RSSI_PAUSE; delay ++){
			if(delay == 500000){
				printf(".");
			}
		}
		if(channel == 1)
			CMB_SPIReadByte (0x2b0, &temp1);
		else
			CMB_SPIReadByte (0x2b5, &temp1);
		
		//printf("measure: %d\n",temp1);
		
		meas_arr[i%RSSI_AVG] = temp1;
		if(i>=RSSI_AVG){
			meas1_std = standard_deviation(meas_arr, RSSI_AVG);
		}
		if (meas1_std < 10){
			for (j=0; j<RSSI_AVG; j++){
				//printf("Array: %d\r\n", meas_arr[j]);
				meas1_avg = meas1_avg + meas_arr[j];
			}
			meas1_avg = meas1_avg/RSSI_AVG;
			printf("Average: %d\r\n", meas1_avg);
			printf("StdDev : %d\r\n", meas1_std);
			return meas1_avg;
		}
		i++;
	}

	return -1;

}

void prodline_RF_loopback_test(int arg1, int arg2, int arg3, double freq){

	int temp;
	extern struct ad9361_rf_phy *ad9361_phy_0;
	extern struct ad9361_rf_phy *ad9361_phy_1;

	extern AD9361_InitParam default_init_param;

	u8 upper, lower, temp1, temp2;

	int temp1avg, temp2avg;

	int i;
	u8 result = 0;
	
	int max = RSSI_UPPER;
	int min = RSSI_LOWER;

	int rssi_ad1rx1a = 0;
	int rssi_ad1rx2a = 0;

	int rssi_ad1rx1c = 0;
	int rssi_ad1rx2c = 0;

	int rssi_ad2rx1a = 0;
	int rssi_ad2rx2a = 0;

	int rssi_ad2rx1c = 0;
	int rssi_ad2rx2c = 0;

	int delay;

	printf("arg1: %d, arg2: %d, arg3: %d, freq: %d\r\n",arg1, arg2, arg3, freq);

	printf("hardware reset\r\n");
	adi_hw_reset();
	//ADI_Set_XO_DAC();
	for (delay = 0; delay < RSSI_PAUSE; delay ++){
		if(delay == 500000)
			printf("pausing\r\n");
	}
	
	//start the transceiver for AD1 TX1A to RX1A and TX2A to RX2A
	printf("Initialize Hardware\r\n");	
	adi_transceiver_init();
	printf("start the tone generator\r\n");
	set_spi_ss(0);
	CMB_SPIWriteByte (0x3f4, 0xc3);
	//measure RSSI for both channels.

	temp1 = 0;
	temp2 = 0;

	/*CMB_SPIReadByte (0x2b0, &temp1);
	CMB_SPIReadByte (0x2b5, &temp2);

	rssi_ad1rx1a = temp1;
	rssi_ad1rx2a = temp2;*/

	//printf("Doing STD rssi_ad1rx1a.\r\n");

	rssi_ad1rx1a = (int) measure_rssi_loopback(ad9361_phy_0,1);
	rssi_ad1rx2a = (int) measure_rssi_loopback(ad9361_phy_0,2);

	printf("stop the tone generator\r\n");
	CMB_SPIWriteByte (0x3f4, 0x0);

	//start the transceiver for AD1 TX1B to RX1C and TX2B to RX2C
	//adi_hw_reset();
	//adi_transceiver_init();
	//startTransceiver_rfloopback(&txFilterCoeWrData[0], &rxFilterCoeWrData[0],1,48);

	//measure RSSI for both channels.
	temp1 = 0;
	temp2 = 0;

	//CMB_SPIReadByte (0x2b0, &temp1);
	//CMB_SPIReadByte (0x2b5, &temp2);

	//printf("Doing STD meas.\r\n");
	//rssi_ad1rx1c = (int) measure_rssi_loopback(1);
	//rssi_ad1rx2c = (int) measure_rssi_loopback(2);

	//switch SPI configuration to AD2
	set_spi_ss(1);

	//start the transceiver for AD2 TX1A to RX1A and TX2A to RX2A
	//adi_hw_reset();
	//startTransceiver_rfloopback(&txFilterCoeWrData[0], &rxFilterCoeWrData[0],0,3);
	//adi_transceiver_init();	
	printf("start the tone generator\r\n");
	set_spi_ss(1);
	CMB_SPIWriteByte (0x3f4, 0xc3);
	//measure RSSI for both channels.
	temp1 = 0;
	temp2 = 0;

	//CMB_SPIReadByte (0x2b0, &temp1);
	//CMB_SPIReadByte (0x2b5, &temp2);

	rssi_ad2rx1a = (int) measure_rssi_loopback(ad9361_phy_1,1);
	rssi_ad2rx2a = (int) measure_rssi_loopback(ad9361_phy_1,2);

	printf("stop the tone generator\r\n");
	CMB_SPIWriteByte (0x3f4, 0x0);

	//start the transceiver for AD2 TX1B to RX1C and TX2B to RX2C
	//adi_hw_reset();
	//startTransceiver_rfloopback(&txFilterCoeWrData[0], &rxFilterCoeWrData[0],1,48);

	//measure RSSI for both channels.
	temp1 = 0;
	temp2 = 0;

	//CMB_SPIReadByte (0x2b0, &temp1);
	//CMB_SPIReadByte (0x2b5, &temp2);

	//rssi_ad2rx1c = (int) measure_rssi_loopback(1);
	//rssi_ad2rx2c = (int) measure_rssi_loopback(2);

	//printf("arg1: %d, arg2 %d, arg3: %d \r\n",arg1,arg2,arg3);


	if(arg1 == 0 || arg1 == 2){
		if(arg2 == 0 || arg2 == 2){
			if(arg3 == 0 || arg3 == 2){
				printf("\r\n\r\nRSSI AD1 RX1A = %d",rssi_ad1rx1a);
				if(rssi_ad1rx1a < min || rssi_ad1rx1a > max){
					printf("****\r\n");
					result++;}
				else
					printf("\r\n");
			}
			if(arg3 == 1 || arg3 == 2){
				printf(        "RSSI AD1 RX1C = %d",rssi_ad1rx1c);
				if(rssi_ad1rx1c < min || rssi_ad1rx1c > max){
					printf("****\r\n");
					result++;}
				else
					printf("\r\n");
			}
		}

		if(arg2 == 1 || arg2 == 2){
			if(arg3 == 0 || arg3 == 2){
				printf(        "RSSI AD1 RX2A = %d",rssi_ad1rx2a);
				if(rssi_ad1rx2a < min || rssi_ad1rx2a > max){
					printf("****\r\n");
					result++;}
				else
					printf("\r\n");
			}
			if (arg3 == 1 || arg3 ==2){
				printf(        "RSSI AD1 RX2C = %d",rssi_ad1rx2c);
				if(rssi_ad1rx2c < min || rssi_ad1rx2c > max){
					printf("****\r\n");
					result++;}
				else
					printf("\r\n");
			}
		}
	}

	//ADI2
	if(arg1 == 1 || arg1 == 2){
		if(arg2 == 0 || arg2 == 2){
			if(arg3 == 0 || arg3 == 2){
				printf(        "RSSI AD2 RX1A = %d",rssi_ad2rx1a);
				if(rssi_ad2rx1a < min || rssi_ad2rx1a > max){
					printf("****\r\n");
					result++;}
				else
					printf("\r\n");
			}
			if(arg3 == 1 || arg3 == 2){
				printf(        "RSSI AD2 RX1C = %d",rssi_ad2rx1c);
				if(rssi_ad2rx1c < min || rssi_ad2rx1c > max){
					printf("****\r\n");
					result++;}
				else
					printf("\r\n");
			}
		}

		if(arg2 == 1 || arg2 == 2){
			if(arg3 == 0 || arg3 == 2){
				printf(        "RSSI AD2 RX2A = %d",rssi_ad2rx2a);
				if(rssi_ad2rx2a < min || rssi_ad2rx2a > max){
					printf("****\r\n");
					result++;}
				else
					printf("\r\n");
			}
			if (arg3 == 1 || arg3 ==2){
				printf(        "RSSI AD2 RX2C = %d",rssi_ad2rx2c);
				if(rssi_ad2rx2c < min || rssi_ad2rx2c > max){
					printf("****\r\n");
					result++;}
				else
					printf("\r\n");
			}
		}
	}

	//printf("\r\nresult = %d\r\n",result);

	if(result)
		printf("\r\n%d Loopback Tests FAILED!\r\n",result);
	else
		printf("\r\nRF Loopback Tests PASS!\r\n");

	return;
}

