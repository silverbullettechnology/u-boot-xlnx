/*
 * routines.c
 *
 *  Created on: Sep 17, 2013
 *      Author: andrew
 */
/*****************************************************************************/

#include "definitions.h"
#include "xuartps_hw.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMIO_BANK		XGPIOPS_BANK2	/* Bank to be used for emio */
#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID
#define LED_DELAY		1000000

XGpioPs Gpio;	/* The driver instance for GPIO Device. */
XGpioPs Gpio; //gpio instance
u8 gpio_bank;
u8 ss_pin;
u8 mosi_pin;
u8 miso_pin;
u8 clk_pin;
u32 port_mask;
u32 miso_pin_mask;
u8 miso_shift;

int hexToInt(char s[]) {

    #define YES 1
    #define NO  0

    int hexdigit, i, inhex, n;
    i=0;
    if(s[i] == '0') {
        ++i;
        if(s[i] == 'x' || s[i] == 'X'){
            ++i;
        }
    }

    n = 0;
    inhex = YES;
    for(; inhex == YES; ++i) {
        if(s[i] >= '0' && s[i] <= '9') {
            hexdigit = s[i] - '0';
        } else if(s[i] >= 'a' && s[i] <= 'f') {
            hexdigit = s[i] - 'a' + 10;
        } else if(s[i] >= 'A' && s[i] <= 'F') {
            hexdigit = s[i] - 'A' + 10;
        } else {
            inhex = NO;
        }

        if(inhex == YES) {
            n = 16 * n + hexdigit;
        }
    }

    return n;
}

/*****************************************************************************/

int atoint(const char *str)
{
   int ret;
   u8 neg;

   if (!str)
   return 0;
   else{

   while (*str == ' ' || *str == '\t'){
    str++;
   }

   ret = 0;

   neg = (*str == '-');

   if (neg || *str == '+') str++;

   while (*str >= '0' && *str <= '9')
   {
      ret = ret * 10 + *str++ - '0';
   }

   return (neg ? -ret : ret);
   }
}

/*****************************************************************************/

u8* hex_decode(const char *in, u8 len,u8 *out)
{
        unsigned int i, t, hn, ln;

        for (t = 0,i = 0; i < len; i+=2,++t) {

                hn = in[i] > '9' ? in[i] - 'A' + 10 : in[i] - '0';
                ln = in[i+1] > '9' ? in[i+1] - 'A' + 10 : in[i+1] - '0';

                out[t] = (hn << 4 ) | ln;
        }

        return out;
}
/*****************************************************************************/

u16 read_ascii_hex_u16(){

#define MAX_LEN 5

	char input_buffer[MAX_LEN];

	char *input_buffer_pointer;

	int input_counter;

	char input;

	int i;

	int ret;

	input_buffer_pointer = &input_buffer[0];



	for(i=0;i<MAX_LEN;i++){
		input_buffer[i] = '\0';
	}

	input_counter = 0;

	while(1){
		if(XUartPs_IsReceiveData(STDIN_BASEADDRESS)){
			input = inbyte();
			xil_printf("%c",input);

			if(input == '\r' || input == '\n')
				break;

			if(input_counter+1 > MAX_LEN){
				xil_printf("\r\ntoo many characters, try again\r\n");
				for(i=0;i<10;i++){
					input_buffer[i] = '\0';
				}
				input_counter = 0;
			}

			*(input_buffer_pointer+input_counter) = input;
			input_counter++;
		}
	}

	ret = hexToInt(input_buffer_pointer);

	return ret;
}

void adi_hw_reset(){

    //need to enable ADI reset pins

	XGpioPs_Config *ConfigPtr;
	volatile int Delay;
	u32 Status = XST_SUCCESS;

	/*
	 * Initialize the GPIO driver.
	 */
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XGpioPs_SetDirection(&Gpio, EMIO_BANK, 0xFFFF);
	XGpioPs_SetOutputEnable(&Gpio, EMIO_BANK, 0xFFFF);

	//ADI Pins
	XGpioPs_WritePin(&Gpio, 58, 0x1);
	XGpioPs_WritePin(&Gpio, 59, 0x1);
	//XGpioPs_WritePin(&Gpio, 70, 0x1);

	//USB Reset
	XGpioPs_WritePin(&Gpio, 87, 0x1);

	//Emmc Reset
	XGpioPs_WritePin(&Gpio, 88, 0x1);

	//LEDs
	XGpioPs_WritePin(&Gpio, 54, 0x1);
	XGpioPs_WritePin(&Gpio, 55, 0x1);
	XGpioPs_WritePin(&Gpio, 56, 0x1);
	XGpioPs_WritePin(&Gpio, 57, 0x1);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	XGpioPs_WritePin(&Gpio, 58, 0x0);
	XGpioPs_WritePin(&Gpio, 59, 0x0);
	//XGpioPs_WritePin(&Gpio, 70, 0x0);

	//USB Reset
	XGpioPs_WritePin(&Gpio, 87, 0x0);

	//Emmc Reset
	XGpioPs_WritePin(&Gpio, 88, 0x0);

	XGpioPs_WritePin(&Gpio, 54, 0x0);
	XGpioPs_WritePin(&Gpio, 55, 0x0);
	XGpioPs_WritePin(&Gpio, 56, 0x0);
	XGpioPs_WritePin(&Gpio, 57, 0x0);


	for (Delay = 0; Delay < LED_DELAY; Delay++);

	XGpioPs_WritePin(&Gpio, 58, 0x1);
	XGpioPs_WritePin(&Gpio, 59, 0x1);
	//XGpioPs_WritePin(&Gpio, 70, 0x1);

	//USB Reset
	XGpioPs_WritePin(&Gpio, 87, 0x1);

	//Emmc Reset
	XGpioPs_WritePin(&Gpio, 88, 0x1);

	XGpioPs_WritePin(&Gpio, 54, 0x1);
	XGpioPs_WritePin(&Gpio, 55, 0x1);
	XGpioPs_WritePin(&Gpio, 56, 0x1);
	XGpioPs_WritePin(&Gpio, 57, 0x1);

}
//reads the serial port input until enter is pressed.  Handles backspace?
char *xil_fgets(char *buf, int bsize){
	int input_counter = 0;
	u8 input;
	if (buf == 0 || bsize <= 0)
		return 0;

	while(1){
		if(XUartPs_IsReceiveData(STDIN_BASEADDRESS)){
			input = inbyte();

			if(input == '\r' || input == '\n'){
				xil_printf("%c",input);
				break;
			}

			if(input_counter+1 > bsize){
				xil_printf("\r\nBuffer Overflow\r\n");
				return 0;
			}

			if (input == 127 || input == 8){
				input_counter--;
				if(input_counter<0)
					input_counter=0;
				else
					xil_printf("%c",input);
			}else{
				xil_printf("%c",input);
				buf[input_counter] = input;
				input_counter++;
			}

		}
	}

	buf[input_counter] = '\0';
	if (input_counter == 0)
		return 0;
	else
		return buf;
}


float standard_deviation(float data[], int n)
{
    float mean=0.0, sum_deviation=0.0;
    int i;
    for(i=0; i<n;++i)
    {
        mean+=data[i];
    }
    mean=mean/n;
    for(i=0; i<n;++i)
    sum_deviation+=(data[i]-mean)*(data[i]-mean);
    return sqrt(sum_deviation/n);
}
