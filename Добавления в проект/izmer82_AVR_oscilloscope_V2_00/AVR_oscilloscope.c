
/* AVR oscilloscope with 128x64 graphic LCD.

   AVR:  ATmega32 at 16MHz (external crystal).
   OpAmp: LM358 is connected on PA0 (ADC0).

   Created by Serasidis Vassilis 
   Home: http://www.serasidis.gr
   e-mail: info@serasidis.gr and avrsite@yahoo.gr

   History:
   
   - V2.00 on 14 Mar 2011 by Vassilis Serasidis. 
     * gLCD library was improved and alphanumeric font was included ('font3x6.h' file).

	 * Information about the measured waveform such as Voltage (Vpp) and frequency (kHz) is appeared on LCD.

	 * Sampling rate has been increased because sample accuracy reduced to 8-bit instead of 10-bit and 
       the ADC prescaler reduced from 16 to 4. That gives a much faster sampling rate.

	 * RAM requirement has been reduced from ~1200 bytes to 335 bytes. That gives you the chance to use some other
	   AVR micro controller with lower RAM than 2kB (firmwares 1.00 and 1.01 need AVRs with 2kB of RAM ).
	    
     * A problem with buttons S3 (PC4 pin) and S7 (PC2 pin) was fixed. 

   - V1.01 on 26 Jun 2008 by Anantha Narayanan.	Solved a delay issue.     
   - V1.00 on 03 Aug 2007 by Vassilis Serasidis. Initial version. Only waveform is appeared on LCD.

   All source code files of "AVR oscilloscope" project are distributed under GPL V3 (General Public License).
*/

#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>
#include "glcd.c"



/* define CPU frequency in MHz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define msUp				1
#define msDwn				4      
#define YposUp				0
#define YposDwn				3
#define freeze				2

#define AC 					0
#define DC 					1
#define SQUARE				2
#define TRUE				0
#define FALSE				1

unsigned int  dataCounter = 0;
unsigned int  timeDiv = 0;  
unsigned char trigger = 0;
unsigned char findZero = 0;
unsigned char upLimit = 0;
unsigned char lowLimit = 255;
unsigned char prevADCvalue = 0;
unsigned char voltageType = AC;
unsigned char complete = TRUE;
unsigned int  voltage;
unsigned char frequency;
unsigned int  ADCvalue;
unsigned char hex2asciiBuffer[4];

signed char Ypos   = 0; 
signed char Ypos2   = 0;
signed char position  = 0;

void ADC_init (void);
void hex2Ascii(unsigned int data, unsigned char Buffer[4]);


int main (void)
{

	unsigned int  i,k;
	unsigned long endOfPeriod=0;
	unsigned char freqComplete=0;
	unsigned int  maxVoltage=0;

	DDRC = 0b00000000; 
	PORTC = 0b11111111;

	DDRA = 0b00000000; 

	glcdInit();
	ADC_init();
	createWelcomeScreen();
	_delay_ms(4000);
	createRaster();
	createWave();

	for(;;)
	{

		if (~PINC & (1<<msUp) && (timeDiv <= 120))
	    	timeDiv += 1;

		if (~PINC & (1<<msDwn) && (timeDiv >= 1))
			timeDiv -= 1;


		if (~PINC & (1<<YposUp) && (Ypos2 <= 60))
	    	Ypos2++;

		if (~PINC & (1<<YposDwn) && (Ypos2 >= -60))
			Ypos2--;

		if (~PINC & (1<<freeze))
			while (~PINC & (1<<freeze)); // It freezes the display to watch the wave.
		

			
//--------v
// Read 100 samples from analog input.
// From these 100 samples we can find the middle of the signal to be used as trigger...
// ... the Volts peak-to-peak and the frequency of the measured signal. 

		findZero = 0;
		upLimit = 0;
		lowLimit = 255;
		endOfPeriod = 0;
		freqComplete = 0;
		complete = FALSE;

		for (i=2; i<15000; i++)
		{
			ADCSRA |= (1 << ADSC); // Enable ADC
			loop_until_bit_is_set(ADCSRA, ADIF); // wait until conversion complete.
			ADCvalue = ADCH;

			//------------------------------------------------------v
			//  Find the end of the first period that is appeared on LCD.
			//  The beginning of the period is always the first pixel on the left of LCD.

            //Find the start of the period of the measured waveform. 
			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 0))
				freqComplete = 1;
			
			//If you have found the start of the period, find the rise of the waveform.
			if((ADCvalue < trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 1))
				freqComplete = 2;

			//The next step is to find the start of the next period...
			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 2))
			{
				freqComplete = 3; //Wow! we found the end of the first period. 
				endOfPeriod = ((23000)/(i/2)); //Calculate the frequency that will be displayed on LCD.	
			}
			//------------------------------------------------------^

			prevADCvalue = ADCvalue; // Get a backup of the current ADC value.

			for(k=timeDiv;k>0;k--)   // Make a delay to see on LCD low-frequency waveforms. Normally, the minimum waveform that can be entirely displayed on LCD is 310 Hz. 
			{
				ADCSRA |= (1 << ADSC);   // Enable ADC
				loop_until_bit_is_set(ADCSRA, ADIF); // wait until conversion complete.
				ADCvalue = ADCH;
			}

			if (upLimit < ADCvalue)  // Find the higher voltage level of the input waveform.
				upLimit = ADCvalue;

			if (lowLimit > ADCvalue) // Find the lower voltage level of the input waveform.
				lowLimit = ADCvalue;


			maxVoltage = (upLimit * 2);  // Maximum voltage on ADC pin is calculated from upLimit register (255*2=510 = 5.10V).

			if (ADCvalue > 0)
			{
				voltage = ((upLimit-lowLimit)*2); //Get the Vpp and store it to "voltage" (Volts Peak-to-peak of inputed waveform).
				ADCvalue += 5;
				ADCvalue /= 5;
				ADCvalue += 2;
			}else
				ADCvalue = 2;
			
			position = ADCvalue + Ypos2 +5; 
			if ((position <= 63) && (position >= 0) && (i<100))  // Adjust Up-Down the wave on LCD.
				fillDataLcdBuffer(i,position);
			else
			if(i<100)
				fillDataLcdBuffer(i,0);

			if((i>100)&&(freqComplete==3))   //If i>100 and freqComplete=3 that means that our waveform is outside of LCD displaying area.
				break;  //Do not wait until i=15000. Terminate this loop ("for (i=0; i<15000; i++)")

		}

		if(upLimit != lowLimit)
			trigger = (((upLimit - lowLimit)/2)+ lowLimit); // Find the middle of the wave to be used as trigger.
		else
			trigger = upLimit;

//--------^

//--------- Print Volts peak-to-peak and frequency on display -------------

		restoreRaster();
		createWave();

		line=3;                    //Show the DC voltage
		column=109;
		gLCDgotoXY(line,column);
		hex2Ascii(maxVoltage,hex2asciiBuffer);
		GLCD_WriteChar(hex2asciiBuffer[2]);
		sendDataOnLCD(0b01000000); //Print one dot character on LCD (only 1 column length).
		sendDataOnLCD(0);
		GLCD_WriteChar(hex2asciiBuffer[1]);
		GLCD_WriteChar(hex2asciiBuffer[0]);
		sendDataOnLCD(0x00);
		GLCD_WriteChar('V');

		line=4;                    //Show the Vpp (Volts peak-to-peak).
		column=109;
		gLCDgotoXY(line,column);
		hex2Ascii(voltage,hex2asciiBuffer);
		GLCD_WriteChar(hex2asciiBuffer[2]);
		sendDataOnLCD(0b01000000); //Print one dot character on LCD (only 1 column length).
		sendDataOnLCD(0);
		GLCD_WriteChar(hex2asciiBuffer[1]);
		GLCD_WriteChar(hex2asciiBuffer[0]);
		sendDataOnLCD(0x00);
		GLCD_WriteChar('V');

		line=6;                   // Go to 6th line on LCD.
		column=122; 
		gLCDgotoXY(line,column);

		if(timeDiv == 0)          // If 'timeDiv' = 0 then the frequency in to the 'endOfPeriod' variable is real.
			GLCD_WriteChar(0x5c); // So, print on LCD the "Play" character. '0x5c' is the '\' character ("Play" symbol).
		else
			GLCD_WriteChar(']');  // If 'timeDiv' is not zero that means we have shrink the waveform. So, print the "Pause" symbol on LCD. 
		
		if(timeDiv == 0)		  // If 'timeDiv' = 0 then the frequency in to the 'endOfPeriod' variable is real.
		{
			line=7;               // Go to 7th line and print on LCD the waveform's frequency
			column=102;
			gLCDgotoXY(line,column);
			if(endOfPeriod <10000) // The maximum frequency that can be displayed on LCD is 9999 Hz.
			{
				gLCDgotoXY(line,column);
				hex2Ascii(endOfPeriod,hex2asciiBuffer);
				GLCD_WriteChar(hex2asciiBuffer[3]);
				GLCD_WriteChar(hex2asciiBuffer[2]);
				GLCD_WriteChar(hex2asciiBuffer[1]);
				GLCD_WriteChar(hex2asciiBuffer[0]);	
			}
			
		}                    // if 'timeDiv' > 0 do not update the frequency. Keep the previous frequency value instead. 

//-------------------------------------------------------

		dataCounter = 0;
		complete = FALSE;
		freqComplete = 0;

		do  //Wait in this loop until you find again the start of the measured waveform.
		{
			prevADCvalue = ADCvalue;
			
			ADCSRA |= (1 << ADSC);    // Enable ADC
			loop_until_bit_is_set(ADCSRA, ADIF);
			ADCvalue = ADCH;

			//Find the start of the period of the measured waveform. 
			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 0))
				freqComplete = 1;
	
			//If you have found the start of the period, find the rise of the waveform.
			if((ADCvalue < trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 1))
				freqComplete = 2;

			//The next step is to find the start of the next period...
			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 2))
			{
				freqComplete = 3; 
				complete = TRUE;
			}
			if(dataCounter > 3000)
				complete = TRUE;
			dataCounter++;

		}while(complete == FALSE);

	}	
}

//===============================================================================
//  Initializing the Analog to Digital Converter (ADC).
//===============================================================================

void ADC_init (void)
{
	ADMUX = 0b01100000;    // PA0 -> ADC0, ADLAR=1 (8-bit)
	ADCSRA |= ((1<<ADEN) | (1<<ADSC) | (1<<ADPS1)); // ADC prescaler at 4
}


//===============================================================================
//  Function that converts an integer into 4 ASCII-character bytes.
//  For example if 'data' contains the number "6543" then 
//    Buffer[3] = '6'  Buffer[2]='5'  Buffer[1]='4'   Buffer[0]='3'. 
//  I know, I could have used the 'itoa' function to do the conversion from integer into ASCII 
//    but my function is easier and more convenient to be handled.
//===============================================================================
void hex2Ascii(unsigned int data, unsigned char Buffer[4])
{
	Buffer[3] = ((data/1000)+0x30);
	data %= 1000;
	Buffer[2]  = ((data/100)+0x30);
	data %= 100;
	Buffer[1]  = ((data/10)+0x30);
	Buffer[0]  = ((data%10)+0x30);
}
