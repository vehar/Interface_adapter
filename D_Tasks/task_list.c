//����������� ���� ����� ��� RTOS

#include "task_list.h"
//============================================================================
//������� �����
//============================================================================


DECLARE_TASK(Task_1ms)
{
//LED_PORT.LED2^=1;
LED_PORT |= (1<<LED2);
SetTimerTask(Task_1ms,1); //������ �������-�������������
LED_PORT  &= ~(1<<LED2);
}  

DECLARE_TASK(Task2_1ms)
{
//LED_PORT.LED2^=1;
LED_PORT |= (1<<LED3);
SetTimerTask(Task2_1ms,1); //������ �������-�������������
LED_PORT  &= ~(1<<LED3);
}  
DECLARE_TASK(Task3_1ms)
{
SetTimerTask(Task3_1ms,1); //������ �������-�������������
}  
DECLARE_TASK(Task4_1ms)
{
SetTimerTask(Task4_1ms,1); //������ �������-�������������
}  
DECLARE_TASK(Task5_1ms)
{
SetTimerTask(Task5_1ms,1); //������ �������-�������������
}  
DECLARE_TASK(Task6_1ms)
{
SetTimerTask(Task6_1ms,1); //������ �������-�������������
}  

 
DECLARE_TASK(Task_LoadTest)
{
 int Tick =0;
 Tick = v_u32_SYS_TICK;
  //LED_PORT  &= ~(1<<LED2);
 //delay_ms(10);
 // LED_PORT |= (1<<LED2);
// delay_ms(1000);

Put_In_Log("Put_In_Log");

  //LcdClear();      
_LCD_SHOWVAL(Tick); // = sprintf (lcd_buf, "Tick=%i",v_u32_SYS_TICK);  // 500us
 LcdString(1,2);    
_LCD_STRINBUF("Ololo=)");
 LcdString(1,4);
 LcdUpdate();
//#asm("sei");
SetTimerTask(Task_LoadTest,1000);; //������ ��������� ����� ��� �������� �������� ��
}

DECLARE_TASK(Task_Start)
{
SetTimerTask(Task_LedOff,100); //������ �������-�������������
}

/*
void Task_Start (void)
{
SetTimerTask(Task_LedOff,100); //������ �������-�������������
}
  */
DECLARE_TASK (Task_LedOff)
{
SetTimerTask(Task_LedOn,900);
LED_PORT  &= ~(1<<LED2);
}

DECLARE_TASK (Task_LedOn)
{
SetTimerTask(Task_LedOff,100);
LED_PORT  |= (1<<LED2);
}



DECLARE_TASK (Task_ADC_test) //Upd-6     //��� �������� ����������� ���������
 {
 #warning no Lcd_out// sprintf (lcd_buf, "DummyADC=%d ",volt);      // ����� �� ����� ����������
 LcdString(1,1);   LcdUpdate();
 SetTimerTask(Task_ADC_test,5000);
 }
void Task_LcdGreetImage (void) //Greeting image on start    //Upd-4
{
//SetTask(LcdClear);
//SetTask(Task_LcdLines);
 //LcdImage(rad1Image);  SetTimerTask(LcdClear,3000);

 //sprintf (lcd_buf, "  Interface   ");
 strncpy (lcd_buf, "  Interface   ", 15);
 LcdStringBig(1,1);
  strncpy(lcd_buf, " Monitor v2.1 ", 15);
 LcdString(1,3);
 strncpy (lcd_buf, " �����������  ", 15);
 LcdString(1,4);
 strncpy (lcd_buf, "  ���������   ", 15);
 LcdString(1,5);
 strncpy (lcd_buf, "  �� - 104    ", 15);
 LcdString(1,6);
LcdUpdate();
//SetTimerTask(LcdClear,7000);

// sprintf (lcd_buf, "Wait comand...");      //�� ��������
// LcdStringBig(1,3);
// SetTimerTask(LcdUpdate,8000);
}

void Task_LcdLines (void)      //Upd-4       //������ ������!
{
    	for (i=0; i<84; i++)
        {
		LcdLine ( 0, 47, i, 0, 1);
        LcdLine ( 84, 47, 84-i, 0, 1);
		LcdUpdate();
		}
}

void Task_AdcOnLcd (void)
{
//SetTask(LcdClear);
 /*sprintf (lcd_buf, "vref=%d ",vref);      // ����� �� ����� ����������
 LcdString(1,1);
  sprintf (lcd_buf, "d=%d ",d);      // ����� �� ����� ����������
 LcdString(1,2);
  sprintf (lcd_buf, "delta=%d ",delta);      // ����� �� ����� ����������
 LcdString(1,3);
  sprintf (lcd_buf, "volt=%d ",volt);      // ����� �� ����� ����������
 LcdString(1,4);  */
 LcdClear();
 ADC_use();
#warning disabled sprintf (lcd_buf, "���������� ��");
 LcdString(1,2);
#warning disabled sprintf (lcd_buf, "  ������ 0");
 LcdString(1,3);
#warning disabled sprintf (lcd_buf, "= %d ��",adc_result);
 LcdString(1,4);

 LcdUpdate();
}

//#error ���������!
void Task_pars_cmd (void)         //300us? empty buff
{
char scan_interval = 100;
  if (USART_Get_rxCount(SYSTEM_USART) > 0) //���� � ������� ������ ���-�� ����
       {
        symbol = USART_Get_Char(SYSTEM_USART);
        PARS_Parser(symbol);
        //SetTask(Task_pars_cmd);  //���������!
        scan_interval = 5;
       }
 else{scan_interval = 155;};
SetTimerTask(Task_pars_cmd, scan_interval); //25   //���������!
}


void Task_LogOut (void)           //500us ?
{
SetTimerTask(Task_LogOut,50);
if(LogIndex){LogOut();} //���� ���-�� ���� � ��� ������ - �������
}


void Task_Flush_WorkLog(void) //������� ��� �������
{
uint16_t i = 0;
//LED_PORT |= (1<<LED2);
while(i<512){WorkLog[i] = 0; i++;};
//LED_PORT &=~(1<<LED2);
}

/*
void Task_Uart_ByfSend(void){ //������� ��� �������

}*/

void Task_SPI_ClrBuf (void){ //������� rx/tx �������� SPI
uint8_t i;
for(i=0;i<64;i++)
 {
Spi0_RX_buf[i] = 0;
Spi0_TX_buf[i] = 0;
  //if(i<=SIZE_SPI_BUF_TX){Spi0_TX_buf[i] = 0;}
 }
}

 void Task_BuffOut  (void)
 {
  SetTimerTask(Task_BuffOut,50);
  RingBuff_TX();
 }

//=================================================
//////////////////////////I2C//////////////////////

//============================================================================
//������� �����
//============================================================================

// ���������� � ������ ����.
void EEP_StartWrite(void)
{
if (!i2c_eep_WriteByte(0xA0,0x00FF,/*(char)Usart0_RX_buf[15]*/ 9,&EEP_Writed))    // ���� ���� �����������
    {
    SetTimerTask(EEP_StartWrite,50);                        // ��������� ������� ����� 50��
    }
}

// ����� ������ �� �������� �� ������ � ������
void EEP_Writed(void)
{
i2c_Do &= i2c_Free;                                            // ����������� ����

if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))                        // ���� ������ �� �������
    {
    SetTimerTask(EEP_StartWrite,20);                        // ��������� �������
    }
else
    {
    SetTask(IIC_Send_Addr_ToSlave);        		// ���� ��� ��, �� ���� �� ���������
	}											// ����� ������� - �������� ������ ������ 2
}

// ��������� � SLAVE �����������
void IIC_Send_Addr_ToSlave(void)
{
if (i2c_Do & i2c_Busy)						// ���� ���������� �����
		{
		SetTimerTask(IIC_Send_Addr_ToSlave,100);	// �� ��������� ����� 100��
		}

i2c_index = 0;								// ����� �������
i2c_ByteCount = 2;							// ���� ��� �����

i2c_SlaveAddress = 0xB0;					// ����� ����������� 0xB0

i2c_Buffer[0] = 0x00;						// �� ����� ��� �����, ��� �� ���� ������������
i2c_Buffer[1] = 0xFF;

i2c_Do = i2c_sawp;							// ����� = ������� ������, �����+��� ����� ������

MasterOutFunc = &IIC_SendeD_Addr_ToSlave;			// ����� ������ �� �������� ���� ��� ������
ErrorOutFunc = &IIC_SendeD_Addr_ToSlave;			// � ���� ��� �����.

TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;		// �������!
i2c_Do |= i2c_Busy;												// ���� ������!
}


// ����� �� �������� IIC
void IIC_SendeD_Addr_ToSlave(void)
{
i2c_Do &= i2c_Free;							// ����������� ����

if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))		// ���� ������� ��� �� ������� ��� ��� ���� �� �����
	{
	SetTimerTask(IIC_Send_Addr_ToSlave,20);		// ��������� �������
	}
}


// ���� ������� ���� ����� � ������� ������
void SlaveControl(void)
{
i2c_Do &= i2c_Free;				// ����������� ����
UDR0 = i2c_InBuff[0];			// ��������� �������� ����
}

//==============================================================================
