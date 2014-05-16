
//used as an example in the program organization doc.                   
#include <Mega32.h> 
//timeout values for each task
#define t1 250  
#define t2 125
#define t3 60  
#define begin {
#define end }
 
//the three task subroutines
void task1(void);  	//blink at 2 or 8 Hz
void task2(void);	//blink at 1 Hz
void task3(void);	//detect button and modify task 1 rate 

void initialize(void); //all the usual mcu stuff 
          
unsigned char time1, time2, time3;	//timeout counters 
unsigned char tsk2c;				//task 2 counter to get to 1/2 second
unsigned char tsk3m;				//task 3 message to task 1 
unsigned char led;					//light states  
         
//**********************************************************
//timer 0 compare ISR
interrupt [TIM0_COMP] void timer0_compare(void)
begin      
  //Decrement the three times if they are not already zero
  if (time1>0)	--time1;
  if (time2>0) 	--time2;
  if (time3>0)	--time3;
end  

//**********************************************************       
//Entry point and task scheduler loop
void main(void)
begin  
  initialize();
  
  //main task scheduler loop 
  while(1)
  begin     
    if (time1==0)	task1();
    if (time2==0) 	task2();
    if (time3==0)	task3();
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
end  
 
//******************************* 
//Task 2  
void task2(void) 
begin
  time2=t2;  //reset the task timer
  if (--tsk2c == 0)  //have we waited 1/2 second?
  begin  
  	tsk2c = 4;		//reload the 1/2 sec counter
     //toggle the ones bit
  	led = led ^ 0x02;
  	PORTB = led;
  end
end  
 
//*******************************   
//Task 3  
void task3(void) 
begin
  time3=t3;     //reset the task timer
  tsk3m = ~PIND & 0x01;  //generate the message for task 1
end 
  
//********************************************************** 
//Set it all up
void initialize(void)
begin
//set up the ports
  DDRD=0x00;	// PORT D is an input
  DDRB=0xff;    // PORT B is an ouput  
  PORTB=0; 
           
  //set up timer 0  
  TIMSK=2;		//turn on timer 0 cmp match ISR 
  OCR0 = 250;  	//set the compare re to 250 time ticks
  	//prescalar to 64 and turn on clear-on-match
  TCCR0=0b00001011;	
    
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

   