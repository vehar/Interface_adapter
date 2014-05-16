/* As graphic LCD used the DEM128064A model (128x64 pixels).

   Created by Serasidis Vassilis and finished at 03.Aug.2007
   Home: http://www.serasidis.gr
   e-mail: info@serasidis.gr and avrsite@yahoo.gr

   -- glcd V1.01 : (by Anantha Narayanan at 27.Jun.2008) fixed an optimitation problem with the delay routine.
   -- glcd V1.00 : (by Vassilis Serasidis at 03.Aug.2007) initial version created 

   This code is ditributed under GPL v3 (General Public License).
*/

#include "glcd.h"
//#include "font5x7.h"
#include "font3x6.h"
#include <util/delay.h>


void glcdInit (void)
{ 


  data_port_pins = 0;  //DATA Port Low
  data_port_ddr = 0xff; //Make DATA port output

  ctrl_port = 0;   //CONTROL Port Low
  ctrl_port_ddr = 0xff; //Make CONTROL port outputs

  ctrl_port |= (1<<lcdrst);
  ctrl_port |= (1<<lcdcs1);   //Enable the CS1 of the display
  ctrl_port |= (1<<lcdcs2);   //Enable the CS2 of the display
  ctrl_port &= ~(1<<lcdrs);        //Clear RS  \_
  ctrl_port &= ~(1<<lcdrw);        //Clear RW  /  Command mode

   
  _delay_us(42);
  data_port = 0b11000000;   //Display start line = 0 (0-63)
  eStrobe();
 

   
  _delay_us(42);
        data_port = 0b01000000;   //Set address = 0 (0-63)
        eStrobe();

   
  _delay_us(42);
        data_port = 0b10111000;   //Set page = 0 (0-7)
        eStrobe();

  _delay_us(42);
   
        data_port = 0b00111111;   //Display ON
        eStrobe();

        ctrl_port &= ~(1<<lcdcs1);  //Disable the CS1 of display
        ctrl_port &= ~(1<<lcdcs2);  //Disable the CS2 of display
}

//====================================================================
//  create "welcome" screen
//====================================================================
void createWelcomeScreen (void)
{ 
	unsigned int i;
	const char *data = welcomeScreen;
	column = 0;
	line =0;

	gLCDgotoXY(line,column);

	for (i = 0; i<1024; i++)
		sendDataOnLCD(pgm_read_byte(data++));

}
//====================================================================
// create the raster screen (IS USED)
//====================================================================
void createRaster (void)
{ 
	unsigned int i;
	const char *data = LcdRaster;
	column = 0;
	line =0;

	gLCDgotoXY(line,column);

	for (i = 0; i<1024; i++)
		sendDataOnLCD(pgm_read_byte(data++));

}
//====================================================================
// (IS USED)
//====================================================================
void gLCDgotoXY (unsigned char lineData, unsigned char columnData)
{
	if(columnData < 64)   // If column is at address 0-63...
		enable_cs1();     // ...select the first half of LCD
	else
	{                     // ...or else...
		enable_cs2();     // ...select the second half of LCD.
		columnData-=64;
	}

	ctrl_port &= ~(1<<lcdrw);   //Clear RW. Command mode
	ctrl_port &= ~(1<<lcdrs);   //Clear RS. Command mode

	lineData |= 0b10111000;
	data_port = lineData;    //Select page (0-7)
	eStrobe();
		
	columnData |= 0b01000000;
	data_port = columnData;  //Set column address (0-63)
	eStrobe();

	ctrl_port &= ~(1<<lcdrw);   //Clear RW. 
	ctrl_port |= (1<<lcdrs);    //Set RS. write data mode
	
}

//====================================================================
//
//====================================================================

void fillDataLcdBuffer (unsigned char address, unsigned char data)
{
	dataLcdBuffer[address] = data;
}

//====================================================================
// (IS USED)
//====================================================================
unsigned char readDataFromLCD (void)
{
	unsigned char data;

	if (column < 64)
		enable_cs1();
	
	if(column >= 64)
		gLCDgotoXY(line,column);

	data_port_ddr = 0;

	ctrl_port |= ((1<<lcdrw)|(1<<lcdrs)); // "DATA SEND" mode
	_delay_us(5); 
	ctrl_port |= (1<<lcde);
	_delay_us(5); 
	ctrl_port &= ~(1<<lcde);
	_delay_us(5);

	ctrl_port |= (1<<lcde);
	_delay_us(5); 
	data=data_port_pins;
	ctrl_port &= ~(1<<lcde);
	_delay_us(5);
	data_port_ddr = 0xff;

	gLCDgotoXY(line,column);
	
	return data;
}

//====================================================================
// (IS USED)
//====================================================================
void sendDataOnLCD (unsigned char data)
{
	if (column < 64)
		enable_cs1();
	
	if(column >= 64)
		gLCDgotoXY(line,column);

	if(column == 128)
	{
		column = 0;
		line++;

		if (line == 8)
			line = 0;
		gLCDgotoXY(line,column);
	}
	_delay_us(5);
	ctrl_port |= (1<<lcdrs); // "DATA SEND" mode
	ctrl_port &= ~(1<<lcdrw); 
	data_port = data;
	eStrobe();

	column++;     // increase column (maximum 128).
}

//====================================================================
// Write the data for 1 waveform to the buffer dataLcdBuffer.
//====================================================================
void createWave (void)
{
 unsigned char data;
 unsigned char byte;
 unsigned char i;

	for (i=0; i<100; i++)
	{
		line=7;
		column=0;
		byte = 0b10000000;

		for(data=dataLcdBuffer[i]; data>7; data-=8)
			line--;
		
		for(;data>0; data--)
			byte >>= 1; 

		column=i;
		gLCDgotoXY(line,column);
		data=readDataFromLCD();
		dataLcdBackupBuffer[i*2]=line;     //Backup the line position (0-7)
		dataLcdBackupBuffer[(i*2)+1]=data; //Backup the data which are on the current LCD possition.

		gLCDgotoXY(line,column);
		sendDataOnLCD(byte | data);	
	}
}

//====================================================================
// 
//====================================================================
void restoreRaster (void)
{
 unsigned char data, i;

	for (i=0; i<100; i++)
	{	
		column=i;
		line = dataLcdBackupBuffer[i*2];
		data = dataLcdBackupBuffer[(i*2)+1];
		gLCDgotoXY(line,column);		
		sendDataOnLCD(data);
	}
}
//====================================================================
//
//====================================================================
void enable_cs1 (void)
{
	ctrl_port |= (1<<lcdcs1);   //Enable the CS1 of the display
	ctrl_port &= ~(1<<lcdcs2);   //Disable the CS2 of the display
}

//====================================================================
//
//====================================================================
void enable_cs2 (void)
{
	ctrl_port |= (1<<lcdcs2);   //Enable the CS2 of the display
	ctrl_port &= ~(1<<lcdcs1);   //Disable the CS1 of the display
}

//====================================================================
//
//====================================================================
void eStrobe (void)
{
   _delay_us(5);
   ctrl_port |= (1<<lcde);   //lcd 'E' pin high
   _delay_us(5);
        ctrl_port &= ~(1<<lcde);  //lcd 'E' pin low
   _delay_us(5);
}


//====================================================================
//
//====================================================================
void GLCD_WriteChar(char charToWrite)
{
	unsigned char i=0;
	for(i = 0; i < 3; i++) 
		sendDataOnLCD(pgm_read_byte(&font3x6[((charToWrite - 0x20) * 3) + i]));
	sendDataOnLCD(0x00);
}
