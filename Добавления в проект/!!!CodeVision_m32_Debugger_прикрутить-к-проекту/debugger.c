/*************************************************************************
A simple target debug program meant to be used interactivly.  

The target is a program written in Codevision C running on a Mega32.
 
Bruce Land -- BRL4@cornell.edu
VERSION 10feb2007
**************************************************************************/
//================
// eliminate the debugger compleletly if defined
#ifndef nullify_debugger
//================

#include <mega32.h> 

//Uncomment this define if you want to activate the receive interrupt
//The rxc ISR allows you to enter debugger with a <control-c>
//#define use_rxc_isr       
//With it commented, the debugger does not use any interrupts,
//nor turn on the global I bit.  
//Uncommented, it uses the UART rxc ISR and enables the I bit. 

//Comment this define if you want to disable verbose help
#define verbose
//Commenting this saves about 300 bytes of flash  

//Uncomment this define to enable inline reporting 
//of registers and memory
#define use_reporting  

//Uncomment this define to enable logging of a variable to RAM on the fly
#define use_logging

/**************************************************************/
// UART communication code
//written by BRL4

#pragma regalloc-
//variable set by break and <cntl-c> 
unsigned char db_break_id;
unsigned char db_t0temp, db_t1temp, db_t2temp;  
unsigned int db_swstk, db_hwstk;
unsigned char db_regs[32], db_sreg;
unsigned char db_cmd_running; 
#ifdef use_logging
unsigned int db_logcount; 
unsigned char db_logMax, db_logname[16]; 
#endif
#pragma regalloc+

//define entries
void cmd(void);
void mymain(void);

/**********************************************************/
//macro to force entry into cmd shell 
//DEBUG jumps to cmd function, and freezes clocks. 
//It is defined here because of the tight relationship with getchar 


void saveitall(void)
{
        #asm 
        ;save the two stack pointers for viewing in the debugger  
        sts _db_swstk, r28 
        sts _db_swstk+1, r29
        in r28, 0x3d   ;stack ptr
        in r29, 0x3e
        sts _db_hwstk, r28
        sts _db_hwstk+1, r29
        in  r28, 0x3f ;sreg
        sts _db_sreg, r28
        lds r28, _db_swstk
        lds r29, _db_swstk+1 
        ;save registers for viewing 
        sts _db_regs,r0 
        sts _db_regs+1,r1 
        sts _db_regs+2,r2 
        sts _db_regs+3,r3 
        sts _db_regs+4,r4 
        sts _db_regs+5,r5 
        sts _db_regs+6,r6 
        sts _db_regs+7,r7 
        sts _db_regs+8,r8 
        sts _db_regs+9,r9 
        sts _db_regs+10,r10 
        sts _db_regs+11,r11 
        sts _db_regs+12,r12 
        sts _db_regs+13,r13 
        sts _db_regs+14,r14 
        sts _db_regs+15,r15 
        sts _db_regs+16,r16 
        sts _db_regs+17,r17 
        sts _db_regs+18,r18 
        sts _db_regs+19,r19 
        sts _db_regs+20,r20 
        sts _db_regs+21,r21 
        sts _db_regs+22,r22 
        sts _db_regs+23,r23 
        sts _db_regs+24,r24 
        sts _db_regs+25,r25 
        sts _db_regs+26,r26 
        sts _db_regs+27,r27 
        sts _db_regs+28,r28 
        sts _db_regs+29,r29 
        sts _db_regs+30,r30 
        sts _db_regs+31,r31 
        #endasm  
} //end saveitall

void loaddatareg(void)
{
        #asm    
        lds r0, _db_regs 
        lds r1, _db_regs+1 
        lds r2, _db_regs+2 
        lds r3, _db_regs+3 
        lds r4, _db_regs+4 
        lds r5, _db_regs+5 
        lds r6, _db_regs+6 
        lds r7, _db_regs+7 
        lds r8, _db_regs+8 
        lds r9, _db_regs+9 
        lds r10, _db_regs+10 
        lds r11, _db_regs+11 
        lds r12, _db_regs+12 
        lds r13, _db_regs+13 
        lds r14, _db_regs+14 
        lds r15, _db_regs+15 
        lds r16, _db_regs+16 
        lds r17, _db_regs+17 
        lds r18, _db_regs+18 
        lds r19, _db_regs+19 
        lds r20, _db_regs+20 
        lds r21, _db_regs+21 
        #endasm  
} //end loaddatareg

             		
#define debug(id) \
	do{      	\
	db_t0temp = TCCR0;  \
	db_t1temp = TCCR1B; \
	db_t2temp = TCCR2;  \
	TCCR0 = 0;  \
	TCCR1B = 0; \
	TCCR2 = 0;  \
	saveitall(); \
	db_break_id = id;  \          
	cmd();     \ 
	loaddatareg(); \
	TCCR0 = db_t0temp;	\
	TCCR1B = db_t1temp;\
	TCCR2 = db_t2temp;	\
	}while(0) 

#ifdef use_reporting
//read a reg on the fly
void peekreg(void)
{
#asm
    push r26
    push r27 
    push r16
    lds  r26, _db_temp1;address of register
    ldi r27, 0			;always zero when datareg
    ;adiw r27:r26, 0x20;ioreg 0 position in memory
    ld   r16, x 	    ;get the data reg contents
    sts _db_temp, r16	;and store datareg contents
    pop  r16
    pop  r27
    pop  r26
#endasm
}       	   
	
#define reportR(regnum) \
    do{ \
        db_temp1 = regnum;  \ 
        peekreg(); \
        printf("R%d=%x\n\r",db_temp1,db_temp) ;  \
    }while(0) 
 
//read a I/Oreg on the fly
void peekio(void)
{
#asm
    push r26
    push r27 
    push r16 
    in r16, SREG        ;store this before it is destroyed
    lds  r26, _db_temp1 ;address of ioregister
    ldi r27, 0			;always zero when ioreg
    cpi r26, 0x3f       ;output if user asked for SREG
    breq isSREG
    adiw r27:r26, 0x20  ;ioreg 0 position in memory
    ld   r16, x 	    ;get the data reg contents
    isSREG:
    sts _db_temp, r16	;and store datareg contents
    pop  r16
    pop  r27
    pop  r26
#endasm
}       	   
	
#define reportI(regnum) \
    do{ \
        db_temp1 = regnum;  \ 
        peekio(); \
        printf("I%x=%x\n\r",db_temp1,db_temp) ;  \
    }while(0)   

//read memory in the fly
void peekmem(void)
{
#asm
    push r26
     push r27 
     push r16
     lds  r26, _db_temp3;low address of memory
     lds r27, _db_temp3+1;high address of memory
     ld   r16, x ;get the memory contents
     sts _db_temp, r16		;and save memory contents
     pop  r16
     pop  r27
     pop  r26	
#endasm  
}
#define reportM(addr) \
    do{ \
        db_temp3 = addr;  \ 
        peekmem(); \
        printf("M%x=%x\n\r",db_temp3,db_temp) ;  \
    }while(0) 

#define reportVhex(varname) \
    do{ \
        printf(#varname);   \      
        printf("=%x\n\r",varname) ;  \    
    }while(0) 

#define reportVdec(varname) \
    do{ \
        printf(#varname);   \      
        printf("=%d\n\r",varname) ;  \    
    }while(0)       
#endif

#ifdef use_logging 
    #include <mem.h>
    #define logInit(varname,logsize) \
    do{ \ 
        db_logMax = logsize;  \
        db_logcount=0;   \ 
        sprintf(db_logname,"%p", #varname); \
    }while(0) 
    
    #define logV(varname) \
    do{ \       
        if (db_logcount >= db_logMax) debug(254);    \
        else pokeb(0x60+db_logcount++,varname); \
    }while(0) 
    
#endif

/**********************************************************/ 
//define the structures and turn on the UART		
void db_init_uart(long baud)
{	 	
   	//uses the clock freq set in the config dialog box
 	UBRRL= _MCU_CLOCK_FREQUENCY_ /(baud*16L) - 1L;
 	UCSRB=0x18; // activate UART 
 	#ifdef use_rxc_isr
 		UCSRB.7=1;   //turn on ISR 
 		#asm("sei")
 	#endif
} 

//redefine getchar if necessary
#ifdef use_rxc_isr
    #define _ALTERNATE_GETCHAR_
    
    #pragma regalloc-
    char db_temp_char, db_char_flag;
    #pragma regalloc+
    
    char getchar(void)
    {   
    	db_char_flag=0; 		
        while (db_char_flag==0); //set by ISR
    	return db_temp_char;	 //set by ISR
    } 
    
    interrupt [USART_RXC] void uart_rec(void)
    {
    	db_temp_char=UDR;   	//get a char
    	//IF cntl-c and NOT currently in the debugger
    	if (db_temp_char==0x03 && db_cmd_running==0) 
    	{   
    	    #asm("sei")
    		debug(255);			//then break to debugger
    	}
    	else
    		db_char_flag = 1;
    } //end ISR
#endif 

#include <stdio.h> 

/**********************************************************/ 

//global variables for debugger
#pragma regalloc-
//MUST BE in memory so asmembler can find them
unsigned char db_temp, db_temp1, db_temp2 ; 
unsigned int db_temp3, db_temp4;    
#pragma regalloc+ 

//********************************************************
//the debug function
void cmd( void )
{   
	unsigned char cmd1str[32], current_char, in_count;  
    unsigned int iter;
   	
   	//this variable is cleared by the "g" (go) command
   	db_cmd_running = 1;  
   	
   	//print break source: 255 is <cntl-c>
	printf("db ID:%d\r\n",db_break_id);
	    
	while(db_cmd_running)
	{   
	    cmd1str[0]=0;
	     
	    //get the next command
    	//handles backspace, <enter>
            printf("db>");
            in_count=0;   
        while ( (current_char=getchar()) != '\r' )  //<enter>
        {    
        	putchar(current_char);
        	if (current_char == 0x08 & in_count>0)	//backspace  
        		--in_count;
        	else   
        		cmd1str[in_count++]=current_char; 	
        }    
        cmd1str[in_count] = 0;	//terminate the string
        putchar('\r');  		//emit carriage return
        putchar('\n'); 			//line feed makes output nicer
        
        //execute the shell command
        //printf("%s\r\n", cmd1str);
        
        //target go comand  
        //and return to target program
        if (cmd1str[0]=='g') db_cmd_running = 0;
         	    
        //reset -- this forces a reboot!!!
        if (cmd1str[0]=='x')
        {   
            while(!UCSRA.5); //wait for last char to transmit
            UCSRB = 0x00;    //turn off uart 
            //Force a watchdog reset
            WDTCR=0x08;   //enable watchdog
            while(1);     //just wait for watchdog to reset machine
        }
        
        //read an i/o register
        if (cmd1str[0]=='i') 
        {
        	sscanf(cmd1str,"%c%x", &db_temp, &db_temp1);
        	
        	//check to see if ioreg is timer control
        	//(which were saved when cmd was entered)
        	if (db_temp1==0x33)
        		db_temp = db_t0temp;
        	else if (db_temp1==0x2e)
        		db_temp = db_t1temp;
        	else if (db_temp1==0x25)
        		db_temp = db_t2temp;
        	else
        	{
        	#asm
        	 	push r26
        	 	push r27 
        	 	push r16
        	 	lds  r26, _db_temp1	;address of ioregister
        	 	ldi	 r27, 0			;always zero when ioreg
        	 	adiw r27:r26, 0x20	;ioreg 0 position in memory
        	 	ld   r16, x 		;get the ioreg contents
        	 	sts	 _db_temp, r16		;and store ioreg contents
        	 	pop  r16
        	 	pop  r27
        	 	pop  r26	
        	#endasm
        	}
        	printf("ioport %x= %x\n\r",db_temp1,db_temp) ;
        } 
        
        //write an i/o register
        if (cmd1str[0]=='I') 
        {
        	sscanf(cmd1str,"%c%x%x", &db_temp,&db_temp1, &db_temp2);
        	//check to see if ioreg is timer control
        	//(which were saved when cmd was entered)
        	if (db_temp1==0x33)
        		db_t0temp = db_temp2;
        	else if (db_temp1==0x2e)
        		db_t1temp = db_temp2;
        	else if (db_temp1==0x25)
        		db_t2temp = db_temp2;
        	else
        	{
        	#asm
        	 	push r26
        	 	push r27 
        	 	push r16
        	 	lds  r16, _db_temp2	;data to be put in ioreg
        	 	lds  r26, _db_temp1	;address of ioregister
        	 	ldi	 r27, 0		;always zero when ioreg
        	 	adiw r27:r26, 0x20	;ioreg 0 position in memory
        	 	st   x, r16 		;set the ioreg contents
        	 	pop  r16
        	 	pop  r27
        	 	pop  r26	
        	#endasm
        	}
        	printf("ioport %x-> %x\n\r",db_temp1,db_temp2) ;
        }
        
         
        //read a data register 	    
        if(cmd1str[0]=='r') 
        {
        	sscanf(cmd1str,"%c%d", &db_temp, &db_temp2);
        	if (db_temp2 < 32)
        	{
        	        db_temp = db_regs[db_temp2];
        	        printf("dataReg %d= %x\n\r", db_temp2, db_temp);
        	 }  
        }     
        
        //WRITE a data register 	
        //don't mess with C registers 22-31    
        if(cmd1str[0]=='R') 
        {
        	sscanf(cmd1str,"%c%d%x", &db_temp, &db_temp2, &db_temp4);
        	if (db_temp2 < 22)
        	{
        	        db_regs[db_temp2] = db_temp4;
        	        printf("dataReg %d-> %x\n\r", db_temp2, db_temp4);
        	}
        	else printf("register # must be < 22\r\n");	
        	 
        }
        
        //read an memory location
        if (cmd1str[0]=='m') 
        {
        	sscanf(cmd1str,"%c%x", &db_temp, &db_temp3);
        	#asm
        	 	push r26
        	 	push r27 
        	 	push r16
        	 	lds  r26, _db_temp3	;low address of memory
        	 	lds	 r27, _db_temp3+1	;high address of memory
        	 	ld   r16, x 		;get the memory contents
        	 	sts	 _db_temp, r16		;and save memory contents
        	 	pop  r16
        	 	pop  r27
        	 	pop  r26	
        	#endasm
        	printf("memory %x= %x\n\r",db_temp3,db_temp) ;
        }  
        
        //WRITE an memory location
        if (cmd1str[0]=='M') 
        {
        	sscanf(cmd1str,"%c%x%x", &db_temp, &db_temp3, &db_temp4);   
        	#asm
        	 	push r26
        	 	push r27 
        	 	push r16
        	 	lds  r26, _db_temp3	;low address of memory
        	 	lds	 r27, _db_temp3+1	;high address of memory
        	 	lds	 r16, _db_temp4 	;get the data
        	 	st	 x, r16			;put it in memory
        	 	pop  r16
        	 	pop  r27
        	 	pop  r26	
        	#endasm
        	db_temp4 = db_temp4 & 0xff;
        	printf("memory %x-> %x\n\r",db_temp3,db_temp4) ;
        }
        
        //get data stack location
        if (cmd1str[0]=='d')
        {   
         	printf("data stk addr=%x\n\r", db_swstk);  	     	   	     	
        }
        
        //get hw stack location  
        //add two because it is read in a function
        if (cmd1str[0]=='w')
        {   	
         	printf("hw stk addr=%x\n\r", db_hwstk+2);  	     	   	     	
        }
        
        //get status register  
        if (cmd1str[0]=='s')
        {   	
         	printf("SREG=%x\n\r", db_sreg);  	     	   	     	
        }
        
        #ifdef use_logging
        //dump log
        if (cmd1str[0]=='l')
        {   
            if (cmd1str[1]=='c') db_logcount=0; 
            else if (cmd1str[1]=='d') 
            {
                printf("--%s--\n\r",db_logname);
                for(iter=0; iter<db_logcount; iter++)
                    printf("%d\r\n",peekb(0x60+iter));             
            }
            else 
            {
                printf("--%s--\n\r",db_logname);
                for(iter=0; iter<db_logcount; iter++)
                    printf("%x\r\n",peekb(0x60+iter)); 
            }
        }
        #endif
        
        //help
        if  (cmd1str[0]=='h')
        {
        #ifdef verbose 	    
            printf("g -- go run target code \r\n");
            printf("x -- reboot mcu \r\n");
            printf("i ioreg -- read i/o register; ioreg in hex\r\n");
            printf("I ioreg data -- write i/o register - hex\r\n");
            printf("r datareg -- read; datareg in dec \r\n");
            printf("R datareg data -- write; data in hex \r\n");
            printf("m addr -- read memory; hex\r\n") ;
            printf("M addr dat -- write memory; hex\r\n"); 
            printf("d -- top of data stack; hex\r\n"); 
            printf("w -- top of hardware stack; hex\r\n"); 
            printf("s -- mcu status register; hex\r\n");
            #ifdef use_logging
            printf("l, lc, ld -- logged variables\r\n");
            #endif
            printf("Target code commands:\r\n");
            printf("  debug(DebugID) -- enter debugger\r\n");
            #ifdef use_rxc_isr
            printf("  <cntl-c> -- enter debugger with debugID=255\r\n"); 
            #endif
            #ifdef use_reporting
            printf("  reportVdec, reportVhex -- print variables\r\n"); 
            #endif
            #ifdef use_logging
            printf("  loginit, logV -- save variables\r\n"); 
            #endif
        #else
            printf("no help \r\n");
        #endif
        }
    
	}  //end while(db_cmd_running)
}    //end cmd

// set up UART and jump to mymain 
void main(void)
{   
    //allocate UART structures, turn on UART 
   	//parameter is baud rate given as an integer
   	//This routine uses the clock value set in the 
   	//Project...Config dialog!!
   	db_init_uart(9600);  
   	printf("\r\r\n DB 2.0\r\n");

	//start out in main program
   	db_cmd_running = 0;
   	
    //now jump to user supplied target program
    mymain() ;  
    
    //mymain should NEVER return
    //But if it does...
    printf("Target Failed") ;
    while(1);
}     

//allows user to insert a program with it's own main routine
#define main mymain     

//end of debugger
//following stuff is used if debugger is NULLIFIED
#else

#define debug(id) do{ }while(0)
#define reportR(regnum)  do{ }while(0)
#define reportI(regnum)   do{ }while(0)
#define reportM(addr) do{ }while(0) 
#define reportVhex(varname)  do{ }while(0)
#define reportVdec(varname)  do{ }while(0)
#define logInit(varname,logsize)  do{ }while(0)
#define logV(varname)  do{ }while(0)
    
#endif

//end of file