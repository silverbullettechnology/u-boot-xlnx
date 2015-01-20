/*
 * routines.c
 *
 */
/*****************************************************************************/


#include <stdarg.h>
#include <common.h> 
//#include <math.h>

#include "definitions.h"
#include "routines.h"

#include "xparameters.h"
#include "xgpiops.h"
#include "xstatus.h"
#include "xuartps_hw.h"

#define EMIO_BANK		XGPIOPS_BANK2	/* Bank to be used for emio */
#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID
#define LED_DELAY		1000000

#define SCAN_BUF_SIZE		100

extern XGpioPs Gpio; //gpio instance


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

char* hex_decode(const char *in, char len,char *out)
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
			printf("%c",input);

			if(input == '\r' || input == '\n')
				break;

			if(input_counter+1 > MAX_LEN){
				printf("\r\ntoo many characters, try again\r\n");
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

	//LEDs
	XGpioPs_WritePin(&Gpio, 54, 0x1);
	XGpioPs_WritePin(&Gpio, 55, 0x1);
	XGpioPs_WritePin(&Gpio, 56, 0x1);
	XGpioPs_WritePin(&Gpio, 57, 0x1);

	for (Delay = 0; Delay < LED_DELAY; Delay++);

	XGpioPs_WritePin(&Gpio, 58, 0x0);
	XGpioPs_WritePin(&Gpio, 59, 0x0);
	//XGpioPs_WritePin(&Gpio, 70, 0x0);

	XGpioPs_WritePin(&Gpio, 54, 0x0);
	XGpioPs_WritePin(&Gpio, 55, 0x0);
	XGpioPs_WritePin(&Gpio, 56, 0x0);
	XGpioPs_WritePin(&Gpio, 57, 0x0);


	for (Delay = 0; Delay < LED_DELAY; Delay++);

	XGpioPs_WritePin(&Gpio, 58, 0x1);
	XGpioPs_WritePin(&Gpio, 59, 0x1);
	//XGpioPs_WritePin(&Gpio, 70, 0x1);

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
				printf("%c",input);
				buf[input_counter] = input;
				input_counter++;
				break;
			}

			if(input_counter+1 > bsize){
				printf("\r\nBuffer Overflow\r\n");
				return 0;
			}

			if (input == 127 || input == 8){
				input_counter--;
				if(input_counter<0)
					input_counter=0;
				else
					printf("%c",input);
			}else{
				printf("%c",input);
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

//
// Reduced version of sscanf (%d, %x, %c, %n are supported)
// %d dec integer (E.g.: 12)
// %x hex integer (E.g.: 0xa0)
// %b bin integer (E.g.: b1010100010)
// %n hex, de or bin integer (e.g: 12, 0xa0, b1010100010)
// %c any character
//

static int r_sscanf(const char* str, const char* format, va_list ap)
{
	//va_list ap;
	int value, tmp;
	int count;
	int pos;
	char neg, fmt_code;
	const char* pf;
	char* sval;
	//va_start(ap, format);

	for (pf = format, count = 0; *format != 0 && *str != 0; format++, str++)
	{
		while (*format == ' ' && *format != 0) format++;//
		if (*format == 0) break;

		while (*str == ' ' && *str != 0) str++;//increment pointer of input string
		if (*str == 0) break;

		if (*format == '%')//recognize how to format
		{
			format++;
			if (*format == 'n')
			{
                if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))//if in str sth like 0xff
                {
                    fmt_code = 'x';
                    str += 2;
                }
                else
                if (str[0] == 'b')
                {
                    fmt_code = 'b';
                    str++;
                }
                else
                    fmt_code = 'd';
			}
			else
				fmt_code = *format; //it is format letter

			switch (fmt_code)
			{
			case 'x':
			case 'X':
				for (value = 0, pos = 0; *str != 0; str++, pos++)
				{
					if ('0' <= *str && *str <= '9')
						tmp = *str - '0';
					else
					if ('a' <= *str && *str <= 'f')
						tmp = *str - 'a' + 10;
					else
					if ('A' <= *str && *str <= 'F')
						tmp = *str - 'A' + 10;
					else
						break;

					value *= 16;
					value += tmp;
				}
				if (pos == 0)
					return count;
				*(va_arg(ap, int*)) = value;
				count++;
				break;

            case 'b':
				for (value = 0, pos = 0; *str != 0; str++, pos++)
				{
					if (*str != '0' && *str != '1')
                        break;
					value *= 2;
					value += *str - '0';
				}
				if (pos == 0)
					return count;
				*(va_arg(ap, int*)) = value;
				count++;
				break;

			case 'd':
				if (*str == '-')
				{
					neg = 1;
					str++;
				}
				else
					neg = 0;
				for (value = 0, pos = 0; *str != 0; str++, pos++)
				{
					if ('0' <= *str && *str <= '9')
						value = value*10 + (int)(*str - '0');
					else
						break;
				}
				if (pos == 0)
					return count;
				*(va_arg(ap, int*)) = neg ? -value : value;
				count++;
				break;

			case 'c':
				*(va_arg(ap, char*)) = *str;
				count++;
				break;
			case 's':
				sval = va_arg(ap, char*);


				while(*str){
				 *sval++ = *str++;
				count++;
				}
				*sval = NULL;

				break;

			default:
				return count;
			}
		}
		else
		{
			if (*format != *str)//
				break;
		}
	}



	return count;
}

int rsscanf(char* input_string, const char* format, ...){
	va_list args;
	va_start( args, format );
	int count = 0;

	count =  r_sscanf(input_string, format, args);
	va_end(args);
	return count;

}
/******************************************************************************/

#define EPSILON 0.0000001

double sqrt(double val) {

        double low = 0;
        double high = 10000000; // a sufficiently big number
        double mid = 0;


        while (high - low > EPSILON) {
                mid = low + (high - low) / 2;
                if (mid*mid > val) {
                        high = mid;
                } else {
                        low = mid;
                }
        }

        return mid;

}

/******************************************************************************
* END OF FILE
******************************************************************************/
