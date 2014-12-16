/*
  * Atmel PDI Programming Function
  */
 #include <common.h>
 #include <command.h>

#include "pdi_main.h"
#include "atxmega128a1_nvm_regs.h"
#include "xparameters.h"
#include "xuartps_hw.h"
#include "routines.h"
 DECLARE_GLOBAL_DATA_PTR;



 static int do_ja(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
   {
   int rcode = 0;

        char buf[16];
        char current_char;
        char buf_counter = 0;
        char command_active = 0;
        char tempchar[2];
        unsigned char tempval;
        unsigned char statusbyte;
        unsigned char cmd_buffer[20];
        uint8_t dev_id[3];
	uint32_t buf_loc;
	char *image;
	unsigned int size;
	int i;

	printf("argc: %d\r\n",argc);
	printf("argv[1]: %s\r\n",argv[1]);
	printf("argv[2]: %s\r\n",argv[2]);

	buf_loc = hexToInt(argv[1]);
	size = atoint (argv[2]);

	printf("location: 0x%X\r\n",buf_loc);
	printf("    size: %d\r\v",size);	
	
	image = buf_loc;

	printf("pointer: 0x%X\r\n",image);

   puts("in ja\n");
   for (;;)
     {
     puts("\n--------------\n");
     puts(" q    quit\n");
     puts(" a    Spin LEDs\n");
     puts(" b    Run Programmer\n");
     puts(" c    SPI Test Interface\n");
     puts(" d    programming manual commands\n");
     puts(" p    print loaded image to screen\n");
     puts("-- enter cmd: ");
     char c = getc();
     switch (c)
       {
       case 'q':
         puts("bye\n");
         return 0;
       case 'a':
         printf("doing 'a'!\n");
	 hello_led();
         break;
       case 'b':
         printf("doing 'b'!\n");
	 pdi_main(image,size);
         break;
       case 'c':
	 fixture_write_read();
         printf("doing 'c'!\n");
         break;
	case 'p':
         printf("doing 'p'!\n");
	 for(i=0;i<size;i++){
	 	printf("%c",*(image+i));
	 }
	 printf("\r\n**END**\r\n");	
         break;
       case 'd':
	 //fixture_write_read();
         printf("doing 'd'!\n");

        xnvm_init();
        while (true) {
          pdi_clock_pulse();
          //printf("clockpulse\r\n");
          if (XUartPs_IsReceiveData(STDIN_BASEADDRESS)) {
	  //if (true) {
            //printf("gotchar: ");
            current_char = getc();
            //printf("%s ",current_char);
            buf[buf_counter] = current_char;
            buf_counter++;
            if (buf_counter > 15)
            buf_counter = 0;

            if (current_char == '\n' || current_char == '\r') {
              //printf("newline detected\r\n");
              //printf("buf_counter: %d\r\n", buf_counter);
              command_active = 1;

              //reset the counter
              buf_counter = 0;
            }
          }
	 if (command_active == 1) {
            //printf("Command Active %s\r\n", buf);
            if (buf[0] == 'w') {
              tempchar[0] = buf[1];
              tempchar[1] = buf[2];
              //sscanf(tempchar, "%2x",&tempval);
	      tempval = hexToInt(tempchar);
              pdi_write(&tempval,1);
              //printf("Write Command: %2X\r\n", tempval);
            } else if (buf[0] == 'k') {
		cmd_buffer[0] = XNVM_PDI_KEY_INSTR;
		cmd_buffer[1] = NVM_KEY_BYTE0;
		cmd_buffer[2] = NVM_KEY_BYTE1;
		cmd_buffer[3] = NVM_KEY_BYTE2;
		cmd_buffer[4] = NVM_KEY_BYTE3;
		cmd_buffer[5] = NVM_KEY_BYTE4;
		cmd_buffer[6] = NVM_KEY_BYTE5;
		cmd_buffer[7] = NVM_KEY_BYTE6;
		cmd_buffer[8] = NVM_KEY_BYTE7;

		pdi_write(cmd_buffer, 9);
              //printf("Write Command: %2X\r\n", tempval);
            } else if (buf[0] == 'i') {
                xnvm_ioread_byte(XNVM_CONTROLLER_BASE + XNVM_CONTROLLER_STATUS_REG_OFFSET, &statusbyte);    
            } else if (buf[0] == 'z') {
                pdi_init();                   
            } else if (buf[0] == 'r') {
                xnvm_read_memory(XNVM_DATA_BASE + NVM_MCU_CONTROL, dev_id, 3);
              //pdi_read(&tempval,1);           
            } else if (buf[0] == 'e') {
                xnvm_chip_erase();
            } else if (buf[0] == 'q'){              
              pdi_deinit();
              printf("\r\nDev ID: %2X ",dev_id[0]);
              printf("%2X ",dev_id[1]);
              printf("%2X \r\n",dev_id[2]);
              printf("status: %2X\r\n", statusbyte);     
              break;
            }
            else{
              //printf("invalid command \r\n");
            }
            buf[0] = '\0';
            buf[1] = '\0';
            buf[2] = '\0';
            command_active = 0;
          }
        }



         break;

       case '\r':
         break;
       default:
         printf("huh? don't know: '%c'\n", c);
         break;
       }
     }
 
   printf("exiting. rcode=%d\n\n", rcode);
   return rcode;
   }
 
 /***************************************************/
 //        name,#parms, isrepeatable?, cmd list, help text
 U_BOOT_CMD(ja, 3, 1, do_ja, "do cool things", "invoke the 'ja' application");
