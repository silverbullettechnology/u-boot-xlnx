/*
 * routines.h
 *
 * Created on: Sep 17, 2013
 *      Author: andrew
 */

#ifndef ROUTINES_H_
#define ROUTINES_H_

int hexToInt(char s[]);
int atoint(const char *str);
char* hex_decode(const char *in, char len,char *out);
unsigned short read_ascii_hex_u16();
void adi_hw_reset();
char *xil_fgets(char *buf, int bsize);
float standard_deviation(float data[], int n);


#endif /* ROUTINES_H_ */
