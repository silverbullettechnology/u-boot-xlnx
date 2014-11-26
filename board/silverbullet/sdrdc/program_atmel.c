/*
  * Atmel PDI Programming Function
  */
 #include <common.h>
 #include <command.h>

 DECLARE_GLOBAL_DATA_PTR;



 static int do_ja(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
   {
   int rcode = 0;
   puts("in ja\n");
   for (;;)
     {
     puts("\n--------------\n");
     puts(" q    quit\n");
     puts(" a    Spin LEDs\n");
     puts(" b    Run Programmer\n");
     puts(" c    run 'c'\n");
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
	 pdi_main();
         break;
       case 'c':
	 fixture_write_read();
         printf("doing 'c'!\n");
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
 U_BOOT_CMD(ja, 1, 1, do_ja, "do cool things", "invoke the 'ja' application");
