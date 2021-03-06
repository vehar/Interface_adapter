//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru
//
//  Target(s)...: Mega
//
//  Compiler....:
//
//  Description.: ������� SPI
//
//  Data........: 2.10.12
//
//***************************************************************************
#ifndef SPI_H
#define SPI_H

#include <compilers.h>
#include "D_Globals/global_defines.h"


//#define COUNT_OF_SPI 2

//SOFTWARE
#define SPI_PORTX   PORTB
#define SPI_DDRX    DDRB

#define SPI_MISO   3
#define SPI_MOSI   2
#define SPI_SCK    1
#define SPI_SS     0


//HARDWARE
#define DDR_SPI    DDRB
#define PORT_SPI   PORTB

#define _MISO   3
#define _MOSI   2
#define _SCK    1
#define _SS     0

#define MAX_SPI_SPEED _MCU_CLOCK_FREQUENCY_/2
#define MAX_SPI_PRESCALLER 128

#define SPI_0 0
#define SPI_1 1
#define SPI_2 2

#define SPI_SLAVE 0
#define SPI_MASTER 1

//bit masks of spi settings
#define MS_SLV   0b00000001 //master/slave
#define PHASE    0b00000010
#define POLARITY 0b00000100


#define SIZE_SPI_BUF_TX  64
#define SIZE_SPI_BUF_RX  64
/*____________�������____________________*/


void SPI_FlushTxBuf(uint8_t sel); //"�������" ���������� ����� (����� ���� �� ������������)

/*������������� SPI ������*/
void Soft_SPI_Master_Init(void);
void Hard_SPI_Master_Init_default(void);
void Hard_SPI_Master_Init(bool phase, bool polarity, uint8_t prescaller);


void Hard_SPI_Slave_Init(void);
void SPI_init(char sel, bool mode, bool phase, bool polarity, uint8_t prescaller);

#endif //SPI_H


/*
������� ��������/������ ������ � ������� SPI ������,
����������� � ������ Master ������� �� ���������
������������������ ��������:

1. ��������� ������� ����������� ������ �� ����� SS
2. �������� ������ � ������� SPDR
3. �������� ��������� �������� (�������� ����� SPIF)
4. ���������� �������� ������ (������ SPDR), ���� ���������
5. ������� �� 2-�� ���, ���� �������� �� ��� ������
6. ��������� �������� ����������� ������ �� ����� SS
*/

/*

*/