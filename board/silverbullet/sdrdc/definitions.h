

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_


#define RSSI_PAUSE 150000
#define RSSI_AVG 40
#define RSSI_TIMEOUT 100
#define RSSI_LOWER 20
#define RSSI_UPPER 55
#define DELTA1_TARGET 25
#define DELTA2_TARGET 24
#define SNR_TARGET 75

#define FFT_SIZE 2048
#define SAMPLE_OFFSET 1000
#define TIMEOUT 200
#define AVG_SIZE 10

#define ADI_TEMP_UPPER 40
#define ADI_TEMP_LOWER 20

//put build definition here.  CUT2_BUILD for Cut 2, PPR_BUILD for PPR
#define PPR_BUILD

extern char gpio_bank;
extern char ss_pin;
extern char mosi_pin;
extern char miso_pin;
extern char clk_pin;
extern unsigned long port_mask;
extern unsigned long miso_pin_mask;
extern char miso_shift;
