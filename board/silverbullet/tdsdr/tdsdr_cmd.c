/*
  * TDSDR Commands
  */
 #include <common.h> 
 #include <command.h>

#include "pdi_main.h"
#include "prodline.h"
 DECLARE_GLOBAL_DATA_PTR;

static int atoint(const char *str);
static int hexToInt(char s[]);
static int do_pdi(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
static int do_bist(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

 static int do_pdi(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
   {
   int rcode = 0;

	uint32_t buf_loc;
	char *image;
	unsigned int size;
	int i;

	buf_loc = hexToInt(argv[1]);
	size = atoint (argv[2]);	
	
	image = buf_loc;

   printf("\n\nTD-SDRDC MMC Firmware Programmer\n\n");

   printf("location: 0x%X\r\n",buf_loc);
   printf("    size: %d\r\v",size);

   for (;;)
     {
     printf("\n--------------\n");
     printf(" q    Quit\n");
     printf(" i    Initiate PDI firmware programming\n");
     printf(" p    print loaded image to screen\n");
     printf("-- enter cmd: ");
     char c = getc();
     switch (c)
       {
       case 'q':
         printf("exiting\n");
         return 0;
       case 'a':
         printf("doing 'a'!\n");
	 //hello_led();
         break;
       case 'i':
	 pdi_main(image,size);
         break;
       case 'c':
	 //fixture_write_read();
         printf("doing 'c'!\n");
         break;
	case 'p':
         printf("Printing Image:\n\n");
	 for(i=0;i<size;i++){
	 	printf("%c",*(image+i));
	 }
	 printf("\r\n**END**\r\n");	
         break;
       case '\r':
         break;
       default:
         printf("Invalid command: '%c'\n", c);
         break;
       }
     }
 
   printf("exiting. rcode=%d\n\n", rcode);
   return rcode;
   }
 
 /***************************************************/
 //        name,#parms, isrepeatable?, cmd list, help text
 U_BOOT_CMD(pdi, 3, 1, do_pdi, "Perform firmware update on MMC module", "<ADDRESS> <Length>");


 /***************************************************/

 static int do_bist(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
   {
   int rcode = 0;

   printf("\n\nTD-SDRDC BIST\n\n");

	prod_main();
 
   printf("exiting. rcode=%d\n\n", rcode);
   return rcode;
   }
 
 /***************************************************/
 //        name,#parms, isrepeatable?, cmd list, help text
 U_BOOT_CMD(bist, 1, 1, do_bist, "Perform firmware update on MMC module", "<ADDRESS> <Length>");

static int hexToInt(char s[]) {

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

static int atoint(const char *str)
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
