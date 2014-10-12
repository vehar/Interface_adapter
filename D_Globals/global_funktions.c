#include <adapter.h>

#include "RTOS/EERTOS.h"
#include "RTOS/EERTOSHAL.h"
 #include "D_Tasks/task_list.h"
/*
Обработчик результата парсера.(команд)
Эта функция вызывается парсером после обработки
последовательности, но при условии приема хотя бы
одного слова.Вызывать эту функцию самому не нужно
*/



/*
void red_blink(void){
char t=4;
    do{
    LED_RED_ON;
    delay_ms(100);
    LED_RED_OFF;
    delay_ms(200);
    }while(--t);
}  */


uint8_t check_after_pow_on(void)     /*need optimisation*/
{
//uint8_t state = 0;

/*#1 check periferie*/
//if (PINA!=0){printf("P_A=%d\r",PINA);}
//if (PINB!=0){printf("P_B=%d\r",PINB);}
//if (PINC!=0){printf("P_C=%d\r",PINC);}
//if (PIND!=0){printf("P_D=%d\r\n",PIND);}


/*#2 check reset source*/
/*The MCU Control and Status Register provides
information on which reset source caused an MCU Reset*/
if (MCUCSR & (1<<PORF))// Power-on Reset
   {
    printf("porf\r\n");
   }
else if (MCUCSR & (1<<EXTRF))// External Reset
   {
    printf("extrf\r\n");
   }
else if (MCUCSR & (1<<BORF))// Brown-Out Reset
   {
    printf("borf\r\n");
   }
else if (MCUCSR & (1<<WDRF))// Watchdog Reset
   {
    printf("wdrf\r\n");
   }
else if (MCUCSR & (1<<JTRF))// JTAG Reset
   {
    printf("JTRF\r\n");
   }

MCUCSR&=~((1<<JTRF) | (1<<WDRF) | (1<<BORF) | (1<<EXTRF) | (1<<PORF));//clear register
return 0;
}


void TIM2_ON(void){
//TCCR2 |= (1<<CS21)|(1<<CS20);
TCCR2 |= (1<<CS22)|(1<<CS21)|(1<<CS20);

}

void TIM2_OFF(void){
//TCCR2 &= ~((1<<CS21)|(1<<CS20));
TCCR2 &= ~((1<<CS22)|(1<<CS21)|(1<<CS20));
}

void Sys_timer_init(void){ //USED BY RTOS (DONT TOUCH!)
//Settings for Timer2
OCR2 = 125; //125000 /125 = 1000 compare interruptes per second
TCCR2 |= (1<<CS21)|(1<<CS20);//START timer (8Mhz div 64 = 125000)   //Upd-5 теперь 16МГц
TIMSK |= (1<<OCIE2); //compare interrupt EN
}


void print_help(void){
StopRTOS();
USART_Send_StrFl(SYSTEM_USART, help_mess_0);
USART_Send_StrFl(SYSTEM_USART, help_mess_1);
USART_Send_StrFl(SYSTEM_USART, help_mess_2);
USART_Send_StrFl(SYSTEM_USART, help_mess_3);
USART_Send_StrFl(SYSTEM_USART, help_mess_4);

USART_Send_StrFl(SYSTEM_USART,help_Uart_0);
USART_Send_StrFl(SYSTEM_USART,help_Uart_1);
USART_Send_StrFl(SYSTEM_USART,help_Spi_0);
USART_Send_StrFl(SYSTEM_USART,help_Spi_1);
RunRTOS();
}

void print_settings_ram(void){
uint8_t i = 0;
char str[10];

USART_Send_Str(USART_0,"\r<RAM>");
USART_Send_Str(USART_0,"\rUART_SETTINGS\r");
  for(i=0;i<COUNT_OF_UARTS;i++)
    {
    USART_Send_Str(USART_0,"UART ");
    ltoa(i,str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r Mode ");
    ltoa(RAM_settings.MODE_of_Uart[i],str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r Speed ");
    ltoa(RAM_settings.baud_of_Uart[i],str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r--------\r");
    }

USART_Send_Str(USART_0,"\rSPI_SETTINGS\r");
  for(i=0;i<COUNT_OF_SPI;i++)
    {
    USART_Send_Str(USART_0,"SPI ");
    ltoa(i,str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r Mode ");
    ltoa(RAM_settings.MODE_of_Spi[i],str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r Prescaller ");
    ltoa(RAM_settings.prescaller_of_Spi[i],str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r--------\r");
    }
//USART_FlushTxBuf(USART_0);
}

void print_settings_eeprom(void){
uint8_t i = 0;
char str[10];

USART_Send_Str(USART_0,"\r<EEPROM>");
USART_Send_Str(USART_0,"\rUART_SETTINGS\r");
  for(i=0;i<COUNT_OF_UARTS;i++)
    {
    USART_Send_Str(USART_0,"UART ");
    ltoa(i,str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r Mode ");
    ltoa(EE_settings.MODE_of_Uart[i],str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r Speed ");
    ltoa(EE_settings.baud_of_Uart[i],str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r--------\r");
    }

USART_Send_Str(USART_0,"\rSPI_SETTINGS\r");
  for(i=0;i<COUNT_OF_SPI;i++)
    {
    USART_Send_Str(USART_0,"SPI ");
    ltoa(i,str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r Mode ");
    ltoa(EE_settings.MODE_of_Spi[i],str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r Prescaller ");
    ltoa(EE_settings.prescaller_of_Spi[i],str);
    USART_Send_Str(USART_0,str); //convert dec to str

    USART_Send_Str(USART_0,"\r--------\r");
    }
//USART_FlushTxBuf(USART_0);
}


void print_sys(void)
{
char str[5];
USART_Send_Str(USART_0,"\rButes_RX ");
ltoa(v_u32_RX_CNT,str);
USART_Send_Str(USART_0,str); //convert dec to str

USART_Send_Str(USART_0,"\rButes_TX ");
ltoa(v_u32_TX_CNT,str);
USART_Send_Str(USART_0,str); //convert dec to str
}


void RingBuff_TX(void)
{ 
  UCSR0B |= (1 << UDRIE0); // TX int - on
}

#warning TODO
uint8_t get_curr_cpu_freq (void){ //возвращает значение текущей частоты работы мк
uint8_t freq = 0;

return freq;
}

#warning отладить!
void cust_delay_ms(uint16_t delay){ //умная задержка
uint32_t timecnt = v_u32_SYS_TICK + delay;
while (v_u32_SYS_TICK < timecnt){}
}


/**
void cust_ltoa(long int n, char *str;)
{
unsigned long i;
unsigned char j,p;
i=1000000000L;
p=0;
if (n<0)
   {
   n=-n;
   *str++='-';
   };
do
  {
  j=(unsigned char) (n/i);
  if (j || p || (i==1))
     {
     *str++=j+'0';
     p=1;
     }
  n%=i;
  i/=10L;
  } while (i!=0);
   *str = 0;
}
*/