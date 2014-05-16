
   
//********************************************************
#include "debugger.c"


#include <stdio.h> // Standard Input/Output functions
#include <delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


//timeout values for each task
#define t1 250  
#define t2 125
#define t3 60  
#define begin {
#define end }
 
//the three task subroutines
void task1(void);      //blink at 2 or 8 Hz
void task2(void);    //blink at 1 Hz
void task3(void);    //detect button and modify task 1 rate 

void initialize(void); //all the usual mcu stuff 
          
unsigned char time1, time2, time3;    //timeout counters 
unsigned char tsk2c;                //task 2 counter to get to 1/2 second
unsigned char tsk3m;                //task 3 message to task 1 
unsigned char led;                    //light states  
char msg[20];                        //task 3 serial input         
//**********************************************************
//timer 0 overflow ISR
interrupt [TIM0_COMPA] void timer0_compare(void)
begin      
  //Decrement the three times if they are not already zero
  if (time1>0)    --time1;
  if (time2>0)    --time2;
  if (time3>0)    --time3;
end  

//**********************************************************       
//Entry point and task scheduler loop

char tmp=0;
void main(void)
begin     
     
  initialize();  
  
  ///*****  
    ///*****    
    DDRC.5=1;     
    DDRC.4=1;  
 ///*******    
    PORTC.5=1;
    delay_ms(100);  
    PORTC.5=0;
///********    
  
  //main task scheduler loop    
  logInit(tmp,8);
  while(1)
  begin      
   // if (time1==0)    task1();
   // if (time2==0)     task2();
  //  if (time3==0)    task3();  
  tmp++;
  
  reportVdec(tmp);
    
      PORTC.5=1;
    delay_ms(500);  
    PORTC.5=0;    
     delay_ms(500);
  //   debug(6);
  end
end  
  
//**********************************************************          
//Task subroutines
//Task 1
void task1(void) 
begin  
  time1=t1;  //reset the task timer
  if (tsk3m != 0) time1 >>= 2;  //check for task 3 message
  
  //toggle the zeros bit
  led = led ^ 0x01;
  PORTB = led;   
        
  
  PORTC.5=1;
end  
 
//******************************* 
//Task 2  
void task2(void) 
begin
  time2=t2;  //reset the task timer
  if (--tsk2c == 0)  //have we waited 1/2 second?
  begin  
      tsk2c = 4;        //reload the 1/2 sec counter
     //toggle the ones bit
      led = led ^ 0x02;
      PORTB = led;   
      
       PORTC.5=0;
  end
end  
 
//*******************************   
//Task 3  
void task3(void)
begin
  time3=t3;     //reset the task timer
  tsk3m = ~PINC & 0x01;  //generate the message for task 1
  if (PINC.1==0)
  {
      #asm
          ldi r16, 0x55
          mov r0, r16
      #endasm         
      
        PORTC.5=1;
      debug(3); 
  }
  if (PINC.2==0)
  { printf("Enter a number:");
      scanf("%s", msg);
       printf("\n\r%s\n\r",msg);
  }

end 
  
//********************************************************** 
//Set it all up
void initialize(void)
begin

    //variables to test local variable position on the data stack
    char tt1, tt2, tt3, tt4, tt5, tt6, tt7, tt8;
    //first 6 vars are in r16-21
      tt1=0x55;      //r16
      tt2=0xaa;      //r17
      tt3=3;      //r16
      tt4=4;      //r17
      tt5=5;      //r16
      tt6=6;      //r17
      //rest of the vars are on the stack
      //dstk+7 and dstk+6
      tt7=0x55;
      tt8=0xaa;
      //at this debug point,
      //issuing the command r16 should return 55
      debug(1);
      
    //set up the ports
  DDRC=0x00;    // PORT C is an input      
  DDRB=0xff;    // PORT B is an output  
  PORTB=0;     
  
       
  //set up timer 0  
  TIMSK0=2;        //turn on timer 0 cmp match ISR 
  OCR0A = 250;      //set the compare re to 250 time ticks
      //prescalar to 64 and turn on clear-on-match
  TCCR0A=0b00001011;    
    
  //init the LED status (all off)
  led=0xff; 
  
  //init the task timers
  time1=t1;
  time2=t2;
  time3=t3;    
  
  //init the task 2 state variable 
  //for four ticks
  tsk2c=4;
  
  //init the task 3 message
  //for no message
  tsk3m=0;
     
  //crank up the ISRs
  #asm
      sei
  #endasm 
end  


