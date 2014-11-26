
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpiops.h"
#include "xstatus.h"

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

extern XGpioPs Gpio; //gpio instance
extern u8 gpio_bank;
extern u8 ss_pin;
extern u8 mosi_pin;
extern u8 miso_pin;
extern u8 clk_pin;
extern u32 port_mask;
extern u32 miso_pin_mask;
extern u8 miso_shift;
