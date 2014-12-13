//***************************************************************************
//
//  Author(s)...: Vlad
//
//  Target(s)...: Mega
//
//  Compiler....:
//
//  Description.: Драйвер SPI
//
//  Data........: 11.2.14
//
//***************************************************************************
#include "spi.h"

#include "RTOS/EERTOS.h"
#include "RTOS/EERTOSHAL.h"
 #include "D_Tasks/task_list.h"
 /*
 Забирать значение из SPDR можно в прерывании таймера!
 */
#warning заменить на структуру
//передающий буфер
static volatile char Spi0_TX_buf[SIZE_SPI_BUF_TX];
static volatile uint16_t Spi0_txBufTail = 0;
static volatile uint16_t Spi0_txBufHead = 0;

static volatile char Spi1_TX_buf[SIZE_SPI_BUF_TX];
static volatile uint16_t Spi1_txBufTail = 0;
static volatile uint16_t Spi1_txBufHead = 0;
 #warning  Spi0_txCount not used


//приемный буфер
static volatile char Spi0_RX_buf[SIZE_SPI_BUF_RX];
static volatile uint16_t Spi0_rxBufTail = 0;
static volatile uint16_t Spi0_rxBufHead = 0;
static volatile uint16_t Spi0_rxCount = 0;

static volatile char Spi1_RX_buf[SIZE_SPI_BUF_RX];
static volatile uint16_t Spi1_rxBufTail = 0;
static volatile uint16_t Spi1_rxBufHead = 0;
static volatile uint16_t Spi1_rxCount = 0;

bool TX_flag = 0; 
bool RX_flag = 0;


void SpiTxBufOvf_Handler(void){
PORTD.7=0;
}

void SPI_FlushTxBuf(uint8_t sel) //"очищает" передающий буфер
{
  uint8_t saved_state;
__disable_interrupts();

 switch (sel)
 {
   case SPI_0:
Spi_0_flush:
  Spi0_txBufTail = 0;
  Spi0_txBufHead = 0;
   break;
   case SPI_1:

   break;
     default:
 goto Spi_0_flush;
   break;
}
__restore_interrupts();
}


///////////////////////////////////////////////////////////////
////////////////////SOFTWARE SPI///////////////////////////////

#warning наполовину софтовый!
/*инициализация SPI*/
void Soft_SPI_Master_Init(void)
{
  /*настройка портов ввода-вывода
  все выводы, кроме MISO выходы*/
  SPI_DDRX = (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(0<<SPI_MISO);
  SPI_PORTX = (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<SPI_MISO);

  /*разрешение spi,старший бит вперед,мастер, режим 0*/
  SPCR = (1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(1<<SPR1)|(0<<SPR0);
 // SPSR = (0<<SPI2X);
 SPCR = (1<<SPIE); /* Enable SPI, Interrupt */
}

////////////////////SOFTWARE SPI///////////////////////////////
///////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////
////////////////////HARDWARE SPI///////////////////////////////

//---------------MASTER-----------------//

void Hard_SPI_Master_Init_default(void)
{
SPCR = 0; /* Set MOSI and SCK output, all others input */
  DDR_SPI = (1<<_MOSI)|(1<<_SCK)|(1<<_SS)|(0<<_MISO);;
  PORT_SPI = (1<<_MOSI)|(1<<_SCK)|(1<<_SS)|(1<<_MISO);
/* Enable SPI, Master, set clock rate fck/16, Interrupt */

SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
//SPCR = (1<<SPIE)|(1<<SPE); /* Enable SPI, Interrupt */  в прерывание не переходит!
}


void Hard_SPI_Master_Init(bool phase, bool polarity, uint8_t prescaller)
{
SPCR = 0;
/* Set MOSI and SCK output, all others input */
  DDR_SPI = (1<<_MOSI)|(1<<_SCK)|(1<<_SS);  DDR_SPI &=~(1<<_MISO);
  PORT_SPI = (1<<_MOSI)|(1<<_SCK)|(1<<_SS);//|(1<<_MISO);//Лучше с подтяжкой!

  SPCR = (phase<<CPHA) | (polarity<<CPOL);

        switch(prescaller)  //prescaller
        {
          case 2:
           SPSR = (1<<SPI2X);
          break;
          case 4:
           SPCR = (0<<SPR1) | (0<<SPR0);
          break;
          case 8:
           SPSR |= (1<<SPI2X);
           SPCR |= (1<<SPR0);
          break;
          case 16:
            SPCR = (1<<SPR0);
          break;
          case 32:
           SPSR = (1<<SPI2X);
           SPCR = (1<<SPR1);
          break;
          case 64:
           SPCR = (1<<SPR0);
          break;
          case 128:
            SPCR = (1<<SPR0) | (1<<SPR0);
          break;
          default:
            SPCR = (1<<SPR0);
          break;
        }
SPCR = (1<<SPE)|(1<<MSTR);
}

/*
sel - number of spi(0 - hardware)
mode - master/slave
*/

void SPI_init(char sel, bool mode, bool phase, bool polarity, uint8_t prescaller){
 switch (sel)
 {
  case SPI_0:
   SPCR = 0;
   SPSR = 0;
     if(mode == SPI_MASTER)
     {
       Hard_SPI_Master_Init(phase, polarity, prescaller);
     }
     else //SLAVE
     {
       Hard_SPI_Slave_Init();
     }
  break;
  /*
   case SPI_1:  //soft spi
   break;
  */
  default:
  Hard_SPI_Master_Init_default();
  break;
 }
}

#warning can be optimized!
void SPI_RW_Buf(uint8_t num, uint8_t *data_tx, uint8_t *data_rx)   //SPI write-read
{
uint8_t i=0; //char data;
   SPI_PORTX &= ~(1<<SPI_SS);
/*
while(num)
{
      SPDR = data_tx[i];  data_tx[i] = 0;
      while(!(SPSR & (1<<SPIF))){};
      data_rx[i] = SPDR;
      // *data_tx++; *data_rx++;
       --num;
        i++;
}  */


 while(*data_tx)
  {
      SPDR = *data_tx++;  //data_tx[i] = 0;
      while(!(SPSR & (1<<SPIF)));
      if(i<SIZE_SPI_BUF_RX){data_rx[i] = SPDR;}
      i++;
  }
   SPI_PORTX |= (1<<SPI_SS);
  SetTask(Task_SPI_ClrBuf); //починить
}
//---------------END_MASTER-----------------//


//---------------SLAVE------------------------//
void Hard_SPI_Slave_Init(void)
{
SPCR = 0;
DDR_SPI = (1<<_MISO);/* Set MISO output, all others input */
SPCR = (1<<SPE);/* Enable SPI */
}
//---------------END SLAVE----------------------//


////////////////////HARDWARE SPI///////////////////////////////
///////////////////////////////////////////////////////////////



//обработчик прерывания по завершению передачи/приёма
interrupt [SPI_STC] void spi_isr(void)  //разобраться с приёмом/передачей!!!!!
{
char data;
uint8_t Tmp = Spi0_txBufTail; // use local variable instead of volatile
 SPCR = (1<<MSTR);  //Master
  PORTD.7^=1;
////////RX
data =  SPDR;
/*
    if (Spi0_rxCount < SIZE_SPI_BUF_RX) //если в буфере еще есть место
    {
       Spi0_RX_buf[Spi0_rxBufTail] = data;//!    //считать символ из SPDR в буфер
       Spi0_rxBufTail++;                    //увеличить индекс хвоста приемного буфера
      if (Spi0_rxBufTail == SIZE_BUF_RX)
      {
       Spi0_rxBufTail = 0;
      }
      Spi0_rxCount++;                      //увеличить счетчик принятых символов
    }
    */
///////////

//////////TX
/*
 if(Tmp != Spi0_txBufHead) // all transmitted
  {
  // SPDR = Spi0_TX_buf[Tmp & (SIZE_SPI_BUF_TX - 1)];
   ++Tmp;
   Spi0_txBufTail = Tmp;
   SPDR = Spi0_TX_buf[Tmp & (SIZE_SPI_BUF_TX - 1)];
  }
   */
/////////

}





  /*
interrupt [SPI_STC] void spi_isr(void) 
{
char data;
uint8_t Tmp = Spi0_txBufTail; // use local variable instead of volatile
 SPCR = (1<<MSTR);  //Master
////////RX
data =  SPDR;
    if (Spi0_rxCount < SIZE_SPI_BUF_RX) //якщо в буфері є місцк
    {
       Spi0_RX_buf[Spi0_rxBufTail] = data;//!    //зчитати символ з SPDR в буфер
       Spi0_rxBufTail++;                 
      if (Spi0_rxBufTail == SIZE_BUF_RX)
      {
       Spi0_rxBufTail = 0;
      }
      Spi0_rxCount++;                     
    }
///////////
//////////TX
 if(Tmp != Spi0_txBufHead) // all transmitted
  {
   ++Tmp;
   Spi0_txBufTail = Tmp;
   SPDR = Spi0_TX_buf[Tmp & (SIZE_SPI_BUF_TX - 1)];
  }
/////////    
}
  */






/*
unsigned char spi(unsigned char data)
{
_ATXMEGA_SPI_.DATA=data;
while ((_ATXMEGA_SPI_.STATUS & SPI_IF_bm)==0);
return _ATXMEGA_SPI_.DATA;
}

void spi_init(bool master_mode,bool lsb_first,SPI_MODE_t mode,bool clk2x,SPI_PRESCALER_t clock_div, unsigned char ss_pin)
{
if (master_mode)
   {
   // Init SS pin as output with wired AND and pull-up
   _ATXMEGA_SPI_PORT_.DIRSET=ss_pin;
   _ATXMEGA_SPI_PORT_.PIN4CTRL=PORT_OPC_WIREDANDPULL_gc;

   // Set SS output to high
   _ATXMEGA_SPI_PORT_.OUTSET=ss_pin;

   // SPI master mode
   _ATXMEGA_SPI_.CTRL=clock_div |                      // SPI prescaler.
                      (clk2x ? SPI_CLK2X_bm : 0) |     // SPI Clock double.
                      SPI_ENABLE_bm |                  // Enable SPI module.
                      (lsb_first ? SPI_DORD_bm : 0) |  // Data order.
                      SPI_MASTER_bm |                  // SPI master.
                      mode;                            // SPI mode.

   // MOSI and SCK as output
   _ATXMEGA_SPI_PORT_.DIRSET=SPI_MOSI_bm | SPI_SCK_bm;
   }
else
   {
   // SPI slave mode
   _ATXMEGA_SPI_.CTRL=SPI_ENABLE_bm |                 // Enable SPI module.
                      (lsb_first ? SPI_DORD_bm : 0) | // Data order.
	                  mode;                           // SPI mode.

   // MISO as output
   _ATXMEGA_SPI_PORT_.DIRSET=SPI_MISO_bm;
   };
// No interrupts, polled mode
_ATXMEGA_SPI_.INTCTRL=SPI_INTLVL_OFF_gc;
}
*/