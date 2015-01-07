/**********************************************
****************PCD8544 Driver*****************
***********************************************

for original NOKIA 3310 & alternative "chinese" version of display

48x84 dots, 6x14 symbols

**********************************************/

//#define china 1		// если определено - работаем по алгоритмам "китайского" дисплея, иначе - оригинального
#define SOFT_SPI 1	// если определено - используем программный SPI, иначе - аппаратный

#ifndef SOFT_SPI
unsigned char SPCR_val = 0x50;	// режим
unsigned char SPSR_val = 0x01;  // удвоение частоты
#endif

//LCD Port & pinout setup. Примечание: порты с "кривой" адресацией (типа порт G и т.п.) не поддерживаются
#define LCD_DC_PORT  	PORTA	// выбор команда-данные; любой пин любого порта
#define LCD_DC_DDR 	    DDRA
#define LCD_DC_PIN 	    0

#define LCD_CPORT 	    PORTA	// Chip-Select; любой пин любого порта
#define LCD_CDDR   	    DDRA
#define LCD_CPIN    	1

#define LCD_RST_PORT 	PORTA	// сброс экрана; любой пин любого порта
#define LCD_RST_DDR   	DDRA
#define LCD_RST_PIN   	2

#define LCD_MOSI_PORT 	PORTA	// выход данных SPI, должен быть соответствующий пин аппаратного SPI, если он используется
#define LCD_MOSI_DDR	DDRA
#define LCD_MOSI_PIN    3

#define LCD_CLK_PORT	PORTA	// тактирование SPI, должен быть соответствующий пин аппаратного SPI, если он используется
#define LCD_CLK_DDR     DDRA
#define LCD_CLK_PIN     4



#ifndef SOFT_SPI
#define LCD_SS_PORT	    PORTB	// ChipSelect SPI, должен быть соответствующий пин аппаратного SPI и настроен на выход
#define LCD_SS_DDR     	DDRB
#define LCD_SS_PIN     	0
#endif

//***********************************************************
//Настройки контроллера дисплея и переменные для работы с ним
//***********************************************************

#pragma used+

unsigned char lcd_buf[15];		//текстовый буфер для вывода на LCD

#warning - перенести в дефайны
bit power_down = 0;			//power-down control: 0 - chip is active, 1 - chip is in PD-mode
bit addressing = 0;			//направление адресации: 0 - горизонтальная, 1- вертикальная
//bit instuct_set = 0;			//набор инструкций: 0 - стандартный, 1 - расширенный - в текущей версии не используется

#ifdef china
bit x_mirror = 0;			//зеркалирование по X: 0 - выкл., 1 - вкл.
bit y_mirror = 0;			//зеркалирование по Y: 0 - выкл., 1 - вкл.
bit SPI_invert = 0;			//порядок битов в SPI: 0 - MSB first, 1 - LSB first
#endif

//unsigned char set_y;			//адрес по У, 0..5 - в текущей версии не используется
//unsigned char set_x;                 	//адрес по Х, 0..83 - в текущей версии не используется
uint8_t temp_control = 3;  	//температурный коэффициент, 0..3
unsigned char bias = 3;                 //смещение, 0..7
unsigned char Vop = 70;			//рабочее напрядение LCD, 0..127 (определяет контрастность)
unsigned char disp_config = 2;		//режим дисплея: 0 - blank, 1 - all on, 2 - normal, 3 - inverse

#ifdef china
unsigned char shift = 5;		//0..3F - сдвиг экрана вверх, в точках
#endif

#define PIXEL_OFF	0		//режимы отображения пикселя - используются в графических функциях
#define PIXEL_ON	1
#define PIXEL_XOR	2

#define LCD_X_RES               84	//разрешение экрана
#define LCD_Y_RES               48
#define LCD_CACHSIZE          LCD_X_RES*LCD_Y_RES/8

#define Cntr_X_RES              102    	//разрешение контроллера - предполагаемое - но работает))
#define Cntr_Y_RES              64
#define Cntr_buf_size           Cntr_X_RES*Cntr_Y_RES/8

unsigned char  LcdCache [LCD_CACHSIZE];	//Cache buffer in SRAM 84*48 bits or 504 bytes
unsigned int   LcdCacheIdx;              	//Cache index

#define LCD_CMD         0
#define LCD_DATA        1

//***************************************************
//****************Прототипы функций******************
//***************************************************
void LcdSend (unsigned char data, unsigned char cmd);    			//Sends data to display controller
void LcdUpdate (void);   							//Copies the LCD cache into the device RAM
void LcdClear (void);    							//Clears the display
void LcdInit ( void );								//Настройка SPI и дисплея
void LcdContrast (unsigned char contrast); 					//contrast -> Contrast value from 0x00 to 0x7F
void LcdMode (unsigned char mode); 						//режимы дисплея: 0 - blank, 1 - all on, 2 - normal, 3 - inverse
void LcdPwrMode (void);								//инвертирует состояние вкл/выкл дисплея
void LcdImage (flash unsigned char *imageData);					//вывод изображения
void LcdPixel (unsigned char x, unsigned char y, unsigned char mode);     	//Displays a pixel at given absolute (x, y) location, mode -> Off, On or Xor
void LcdLine (int x1, int y1, int x2, int y2, unsigned char mode);  		//Draws a line between two points on the display
void LcdCircle(char x, char y, char radius, unsigned char mode);		//рисуем круг с координатами центра и радиусом
void LcdBatt(int x1, int y1, int x2, int y2, unsigned char persent);		//рисуем батарейку и заполняем ее на %
void LcdGotoXYFont (unsigned char x, unsigned char y);   			//Sets cursor location to xy location. Range: 1,1 .. 14,6
void LcdTextBufClean (void);							//очистка текстового буфера
void LcdChr (int ch);								//Displays a character at current cursor location and increment cursor location
void LcdString (unsigned char x, unsigned char y);				//Displays a string at current cursor location
void LcdChrBold (int ch);							//Печатает символ на текущем месте, большой и жирный)
void LcdStringBold (unsigned char x, unsigned char y);				//Печатает большую и жирную строку
void LcdChrBig (int ch);							//Печатает символ на текущем месте, большой
void LcdStringBig (unsigned char x, unsigned char y);				//Печатает большую строку
//***************************************************
// UPDATE ##1
void LcdBar(int x1, int y1, int x2, int y2, unsigned char persent);		// рисует прогресс-бар и заполняет его на "процент"
void LcdBarLine(unsigned char line, unsigned char persent);			// рисуем прошресс-бар в указанной строке
void LcdStringInv (unsigned char x, unsigned char y);                           // печатает строку в инверсном шрифте (удобно для настроек)

  #warning a lot of nulls -need to compressed!
const char table[0x0500] =
{
0x00, 0x00, 0x00, 0x00, 0x00,// 00
0x00, 0x00, 0x5F, 0x00, 0x00,// 01
0x00, 0x07, 0x00, 0x07, 0x00,// 02
0x14, 0x7F, 0x14, 0x7F, 0x14,// 03
0x24, 0x2A, 0x7F, 0x2A, 0x12,// 04
0x23, 0x13, 0x08, 0x64, 0x62,// 05
0x36, 0x49, 0x55, 0x22, 0x50,// 06
0x00, 0x05, 0x03, 0x00, 0x00,// 07
0x00, 0x1C, 0x22, 0x41, 0x00,// 08
0x00, 0x41, 0x22, 0x1C, 0x00,// 09
0x14, 0x08, 0x3E, 0x08, 0x14,// 0A
0x08, 0x08, 0x3E, 0x08, 0x08,// 0B
0x00, 0x50, 0x30, 0x00, 0x00,// 0C
0x08, 0x08, 0x08, 0x08, 0x08,// 0D
0x00, 0x60, 0x60, 0x00, 0x00,// 0E
0x20, 0x10, 0x08, 0x04, 0x02,// 0F
0x00, 0x00, 0x00, 0x00, 0x00,// 10
0x00, 0x00, 0x5F, 0x00, 0x00,// 11
0x00, 0x07, 0x00, 0x07, 0x00,// 12
0x14, 0x7F, 0x14, 0x7F, 0x14,// 13
0x24, 0x2A, 0x7F, 0x2A, 0x12,// 14
0x23, 0x13, 0x08, 0x64, 0x62,// 15
0x36, 0x49, 0x55, 0x22, 0x50,// 16
0x00, 0x05, 0x03, 0x00, 0x00,// 17
0x00, 0x1C, 0x22, 0x41, 0x00,// 18
0x00, 0x41, 0x22, 0x1C, 0x00,// 19
0x14, 0x08, 0x3E, 0x08, 0x14,// 1A
0x08, 0x08, 0x3E, 0x08, 0x08,// 1B
0x00, 0x50, 0x30, 0x00, 0x00,// 1C
0x08, 0x08, 0x08, 0x08, 0x08,// 1D
0x00, 0x60, 0x60, 0x00, 0x00,// 1E
0x20, 0x10, 0x08, 0x04, 0x02,// 1F
0x00, 0x00, 0x00, 0x00, 0x00,// 20 space
0x00, 0x00, 0x5F, 0x00, 0x00,// 21 !
0x00, 0x07, 0x00, 0x07, 0x00,// 22 "
0x14, 0x7F, 0x14, 0x7F, 0x14,// 23 #
0x24, 0x2A, 0x7F, 0x2A, 0x12,// 24 $
0x23, 0x13, 0x08, 0x64, 0x62,// 25 %
0x36, 0x49, 0x55, 0x22, 0x50,// 26 &
0x00, 0x05, 0x03, 0x00, 0x00,// 27 '
0x00, 0x1C, 0x22, 0x41, 0x00,// 28 (
0x00, 0x41, 0x22, 0x1C, 0x00,// 29 )
0x14, 0x08, 0x3E, 0x08, 0x14,// 2a *
0x08, 0x08, 0x3E, 0x08, 0x08,// 2b +
0x00, 0x50, 0x30, 0x00, 0x00,// 2c ,
0x08, 0x08, 0x08, 0x08, 0x08,// 2d -
0x00, 0x60, 0x60, 0x00, 0x00,// 2e .
0x20, 0x10, 0x08, 0x04, 0x02,// 2f /
0x3E, 0x51, 0x49, 0x45, 0x3E,// 30 0
0x00, 0x42, 0x7F, 0x40, 0x00,// 31 1
0x42, 0x61, 0x51, 0x49, 0x46,// 32 2
0x21, 0x41, 0x45, 0x4B, 0x31,// 33 3
0x18, 0x14, 0x12, 0x7F, 0x10,// 34 4
0x27, 0x45, 0x45, 0x45, 0x39,// 35 5
0x3C, 0x4A, 0x49, 0x49, 0x30,// 36 6
0x01, 0x71, 0x09, 0x05, 0x03,// 37 7
0x36, 0x49, 0x49, 0x49, 0x36,// 38 8
0x06, 0x49, 0x49, 0x29, 0x1E,// 39 9
0x00, 0x36, 0x36, 0x00, 0x00,// 3a :
0x00, 0x56, 0x36, 0x00, 0x00,// 3b ;
0x08, 0x14, 0x22, 0x41, 0x00,// 3c <
0x14, 0x14, 0x14, 0x14, 0x14,// 3d =
0x00, 0x41, 0x22, 0x14, 0x08,// 3e >
0x02, 0x01, 0x51, 0x09, 0x06,// 3f ?
0x32, 0x49, 0x79, 0x41, 0x3E,// 40 @
0x7E, 0x11, 0x11, 0x11, 0x7E,// 41 A
0x7F, 0x49, 0x49, 0x49, 0x36,// 42 B
0x3E, 0x41, 0x41, 0x41, 0x22,// 43 C
0x7F, 0x41, 0x41, 0x22, 0x1C,// 44 D
0x7F, 0x49, 0x49, 0x49, 0x41,// 45 E
0x7F, 0x09, 0x09, 0x09, 0x01,// 46 F
0x3E, 0x41, 0x49, 0x49, 0x7A,// 47 G
0x7F, 0x08, 0x08, 0x08, 0x7F,// 48 H
0x00, 0x41, 0x7F, 0x41, 0x00,// 49 I
0x20, 0x40, 0x41, 0x3F, 0x01,// 4a J
0x7F, 0x08, 0x14, 0x22, 0x41,// 4b K
0x7F, 0x40, 0x40, 0x40, 0x40,// 4c L
0x7F, 0x02, 0x0C, 0x02, 0x7F,// 4d M
0x7F, 0x04, 0x08, 0x10, 0x7F,// 4e N
0x3E, 0x41, 0x41, 0x41, 0x3E,// 4f O
0x7F, 0x09, 0x09, 0x09, 0x06,// 50 P
0x3E, 0x41, 0x51, 0x21, 0x5E,// 51 Q
0x7F, 0x09, 0x19, 0x29, 0x46,// 52 R
0x46, 0x49, 0x49, 0x49, 0x31,// 53 S
0x01, 0x01, 0x7F, 0x01, 0x01,// 54 T
0x3F, 0x40, 0x40, 0x40, 0x3F,// 55 U
0x1F, 0x20, 0x40, 0x20, 0x1F,// 56 V
0x3F, 0x40, 0x38, 0x40, 0x3F,// 57 W
0x63, 0x14, 0x08, 0x14, 0x63,// 58 X
0x07, 0x08, 0x70, 0x08, 0x07,// 59 Y
0x61, 0x51, 0x49, 0x45, 0x43,// 5a Z
0x00, 0x7F, 0x41, 0x41, 0x00,// 5b [
0x02, 0x04, 0x08, 0x10, 0x20,// 5c Yen Currency Sign
0x00, 0x41, 0x41, 0x7F, 0x00,// 5d ]
0x04, 0x02, 0x01, 0x02, 0x04,// 5e ^
0x40, 0x40, 0x40, 0x40, 0x40,// 5f _
0x00, 0x01, 0x02, 0x04, 0x00,// 60 `
0x20, 0x54, 0x54, 0x54, 0x78,// 61 a
0x7F, 0x48, 0x44, 0x44, 0x38,// 62 b
0x38, 0x44, 0x44, 0x44, 0x20,// 63 c
0x38, 0x44, 0x44, 0x48, 0x7F,// 64 d
0x38, 0x54, 0x54, 0x54, 0x18,// 65 e
0x08, 0x7E, 0x09, 0x01, 0x02,// 66 f
0x0C, 0x52, 0x52, 0x52, 0x3E,// 67 g
0x7F, 0x08, 0x04, 0x04, 0x78,// 68 h
0x00, 0x44, 0x7D, 0x40, 0x00,// 69 i
0x20, 0x40, 0x44, 0x3D, 0x00,// 6a j
0x7F, 0x10, 0x28, 0x44, 0x00,// 6b k
0x00, 0x41, 0x7F, 0x40, 0x00,// 6c l
0x7C, 0x04, 0x18, 0x04, 0x78,// 6d m
0x7C, 0x08, 0x04, 0x04, 0x78,// 6e n
0x38, 0x44, 0x44, 0x44, 0x38,// 6f o
0x7C, 0x14, 0x14, 0x14, 0x08,// 70 p
0x08, 0x14, 0x14, 0x18, 0x7C,// 71 q
0x7C, 0x08, 0x04, 0x04, 0x08,// 72 r
0x48, 0x54, 0x54, 0x54, 0x20,// 73 s
0x04, 0x3F, 0x44, 0x40, 0x20,// 74 t
0x3C, 0x40, 0x40, 0x20, 0x7C,// 75 u
0x1C, 0x20, 0x40, 0x20, 0x1C,// 76 v
0x3C, 0x40, 0x30, 0x40, 0x3C,// 77 w
0x44, 0x28, 0x10, 0x28, 0x44,// 78 x
0x0C, 0x50, 0x50, 0x50, 0x3C,// 79 y
0x44, 0x64, 0x54, 0x4C, 0x44,// 7a z
0x00, 0x08, 0x36, 0x41, 0x00,// 7b <
0x00, 0x00, 0x7F, 0x00, 0x00,// 7c |
0x00, 0x41, 0x36, 0x08, 0x00,// 7d >
0x10, 0x08, 0x08, 0x10, 0x08,// 7e Right Arrow ->
0x78, 0x46, 0x41, 0x46, 0x78,// 7f Left Arrow <-
0x00, 0x00, 0x00, 0x00, 0x00,// 80
0x00, 0x00, 0x5F, 0x00, 0x00,// 81
0x00, 0x07, 0x00, 0x07, 0x00,// 82
0x14, 0x7F, 0x14, 0x7F, 0x14,// 83
0x24, 0x2A, 0x7F, 0x2A, 0x12,// 84
0x23, 0x13, 0x08, 0x64, 0x62,// 85
0x36, 0x49, 0x55, 0x22, 0x50,// 86
0x00, 0x05, 0x03, 0x00, 0x00,// 87
0x00, 0x1C, 0x22, 0x41, 0x00,// 88
0x00, 0x41, 0x22, 0x1C, 0x00,// 89
0x14, 0x08, 0x3E, 0x08, 0x14,// 8A
0x08, 0x08, 0x3E, 0x08, 0x08,// 8B
0x00, 0x50, 0x30, 0x00, 0x00,// 8C
0x08, 0x08, 0x08, 0x08, 0x08,// 8D
0x00, 0x60, 0x60, 0x00, 0x00,// 8E
0x20, 0x10, 0x08, 0x04, 0x02,// 8F
0x00, 0x00, 0x00, 0x00, 0x00,// 90
0x00, 0x00, 0x5F, 0x00, 0x00,// 91
0x00, 0x07, 0x00, 0x07, 0x00,// 92
0x14, 0x7F, 0x14, 0x7F, 0x14,// 93
0x24, 0x2A, 0x7F, 0x2A, 0x12,// 94
0x23, 0x13, 0x08, 0x64, 0x62,// 95
0x36, 0x49, 0x55, 0x22, 0x50,// 96
0x00, 0x05, 0x03, 0x00, 0x00,// 97
0x00, 0x1C, 0x22, 0x41, 0x00,// 98
0x00, 0x41, 0x22, 0x1C, 0x00,// 99
0x14, 0x08, 0x3E, 0x08, 0x14,// 9A
0x08, 0x08, 0x3E, 0x08, 0x08,// 9B
0x00, 0x50, 0x30, 0x00, 0x00,// 9C
0x08, 0x08, 0x08, 0x08, 0x08,// 9D
0x00, 0x60, 0x60, 0x00, 0x00,// 9E
0x20, 0x10, 0x08, 0x04, 0x02,// 9F
0x00, 0x00, 0x00, 0x00, 0x00,// A0
0x00, 0x00, 0x5F, 0x00, 0x00,// A1
0x00, 0x07, 0x00, 0x07, 0x00,// A2
0x14, 0x7F, 0x14, 0x7F, 0x14,// A3
0x24, 0x2A, 0x7F, 0x2A, 0x12,// A4
0x23, 0x13, 0x08, 0x64, 0x62,// A5
0x36, 0x49, 0x55, 0x22, 0x50,// A6
0x00, 0x05, 0x03, 0x00, 0x00,// A7
0x00, 0x1C, 0x22, 0x41, 0x00,// A8
0x00, 0x41, 0x22, 0x1C, 0x00,// A9
0x14, 0x08, 0x3E, 0x08, 0x14,// AA
0x08, 0x08, 0x3E, 0x08, 0x08,// AB
0x00, 0x50, 0x30, 0x00, 0x00,// AC
0x08, 0x08, 0x08, 0x08, 0x08,// AD
0x00, 0x60, 0x60, 0x00, 0x00,// AE
0x20, 0x10, 0x08, 0x04, 0x02,// AF
0x3E, 0x51, 0x49, 0x45, 0x3E,// B0
0x00, 0x42, 0x7F, 0x40, 0x00,// B1
0x42, 0x61, 0x51, 0x49, 0x46,// B2
0x21, 0x41, 0x45, 0x4B, 0x31,// B3
0x18, 0x14, 0x12, 0x7F, 0x10,// B4
0x27, 0x45, 0x45, 0x45, 0x39,// B5
0x3C, 0x4A, 0x49, 0x49, 0x30,// B6
0x01, 0x71, 0x09, 0x05, 0x03,// B7
0x36, 0x49, 0x49, 0x49, 0x36,// B8
0x06, 0x49, 0x49, 0x29, 0x1E,// B9
0x00, 0x36, 0x36, 0x00, 0x00,// BA
0x00, 0x56, 0x36, 0x00, 0x00,// BB
0x08, 0x14, 0x22, 0x41, 0x00,// BC
0x14, 0x14, 0x14, 0x14, 0x14,// BD
0x00, 0x41, 0x22, 0x14, 0x08,// BE
0x02, 0x01, 0x51, 0x09, 0x06,// BF
0x7E, 0x11, 0x11, 0x11, 0x7E,// C0 А
0x7F, 0x49, 0x49, 0x49, 0x31,// C1 Б
0x7F, 0x49, 0x49, 0x49, 0x36,// C2 ?
0x7F, 0x01, 0x01, 0x01, 0x03,// C3 ?
0x70, 0x29, 0x27, 0x21, 0x7F,// C4 ?
0x7F, 0x49, 0x49, 0x49, 0x41,// C5 ?
0x77, 0x08, 0x7F, 0x08, 0x77,// C6 ?
0x41, 0x41, 0x41, 0x49, 0x76,// C7 ?
0x7F, 0x10, 0x08, 0x04, 0x7F,// C8 ?
0x7F, 0x10, 0x09, 0x04, 0x7F,// C9 ?
0x7F, 0x08, 0x14, 0x22, 0x41,// CA ?
0x20, 0x41, 0x3F, 0x01, 0x7F,// CB ?
0x7F, 0x02, 0x0C, 0x02, 0x7F,// CC ?
0x7F, 0x08, 0x08, 0x08, 0x7F,// CD ?
0x3E, 0x41, 0x41, 0x41, 0x3E,// CE ?
0x7F, 0x01, 0x01, 0x01, 0x7F,// CF ?
0x7F, 0x09, 0x09, 0x09, 0x06,// D0 ?
0x3E, 0x41, 0x41, 0x41, 0x22,// D1 ?
0x01, 0x01, 0x7F, 0x01, 0x01,// D2 ?
0x47, 0x28, 0x10, 0x08, 0x07,// D3 ?
0x1E, 0x21, 0x7F, 0x21, 0x1E,// D4 ?
0x63, 0x14, 0x08, 0x14, 0x63,// D5 ?
0x3F, 0x20, 0x20, 0x20, 0x5F,// D6 ?
0x07, 0x08, 0x08, 0x08, 0x7F,// D7 ?
0x7F, 0x40, 0x7F, 0x40, 0x7F,// D8 ?
0x3F, 0x20, 0x3F, 0x20, 0x5F,// D9 ?
0x01, 0x7F, 0x48, 0x48, 0x30,// DA ?
0x7F, 0x48, 0x30, 0x00, 0x7F,// DB ?
0x00, 0x7F, 0x48, 0x48, 0x30,// DC ?
0x41, 0x41, 0x41, 0x49, 0x3E,// DD ?
0x7F, 0x08, 0x3E, 0x41, 0x3E,// DE ?
0x46, 0x29, 0x19, 0x09, 0x7F,// DF ?
0x20, 0x54, 0x54, 0x54, 0x78,// E0 ?
0x3C, 0x4A, 0x4A, 0x49, 0x31,// E1 ?
0x7C, 0x54, 0x54, 0x28, 0x00,// E2 ?
0x7C, 0x04, 0x04, 0x04, 0x0C,// E3 ?
0x72, 0x2A, 0x26, 0x22, 0x7E,// E4 ?
0x38, 0x54, 0x54, 0x54, 0x18,// E5 ?
0x6C, 0x10, 0x7C, 0x10, 0x6C,// E6 ?
0x44, 0x44, 0x54, 0x54, 0x38,// E7 ?
0x7C, 0x20, 0x10, 0x08, 0x7C,// E8 ?
0x7C, 0x21, 0x12, 0x09, 0x7C,// E9 ?
0x7C, 0x10, 0x28, 0x44, 0x00,// EA ?
0x20, 0x44, 0x3C, 0x04, 0x7C,// EB ?
0x7C, 0x08, 0x10, 0x08, 0x7C,// EC ?
0x7C, 0x10, 0x10, 0x10, 0x7C,// ED ?
0x38, 0x44, 0x44, 0x44, 0x38,// EE ?
0x7C, 0x04, 0x04, 0x04, 0x7C,// EF ?
0x7C, 0x14, 0x14, 0x14, 0x08,// F0 ?
0x38, 0x44, 0x44, 0x44, 0x20,// F1 ?
0x04, 0x04, 0x7C, 0x04, 0x04,// F2 ?
0x44, 0x28, 0x10, 0x08, 0x04,// F3 ?
0x08, 0x14, 0x7E, 0x14, 0x08,// F4 ?
0x44, 0x28, 0x10, 0x28, 0x44,// F5 ?
0x3C, 0x40, 0x40, 0x7C, 0x40,// F6 ?
0x0C, 0x10, 0x10, 0x10, 0x7C,// F7 ?
0x7C, 0x40, 0x7C, 0x40, 0x7C,// F8 ?
0x3C, 0x20, 0x3C, 0x20, 0x7C,// F9 ?
0x04, 0x7C, 0x50, 0x50, 0x20,// FA ?
0x7C, 0x50, 0x20, 0x00, 0x7C,// FB ?
0x00, 0x7C, 0x50, 0x50, 0x20,// FC ?
0x28, 0x44, 0x44, 0x54, 0x38,// FD ?
0x7C, 0x10, 0x38, 0x44, 0x38,// FE ?
0x48, 0x54, 0x34, 0x14, 0x7C }; // FF

void LcdSend (unsigned char data, unsigned char cmd)    //Sends data to display controller
        {
        #ifdef SOFT_SPI
        unsigned char i, mask = 128;
        #endif

        LCD_CPORT.LCD_CPIN = 0;                //Enable display controller (active low)
         if (cmd) LCD_DC_PORT.LCD_DC_PIN = 1;	//выбираем команда или данные
         else LCD_DC_PORT.LCD_DC_PIN = 0;

        #ifdef SOFT_SPI
        for (i=0;i<8;i++)
          {
            	if ((data&mask)!=0) LCD_MOSI_PORT.LCD_MOSI_PIN = 1;
                else LCD_MOSI_PORT.LCD_MOSI_PIN = 0;
        	mask = mask/2;
        	LCD_CLK_PORT.LCD_CLK_PIN = 1;
            LCD_CLK_PORT.LCD_CLK_PIN = 0;
          }
        #endif

        #ifndef SOFT_SPI
        SPDR = data;                            //Send data to display controller
        while ( (SPSR & 0x80) != 0x80 ){};        //Wait until Tx register empty
        #endif

        LCD_CPORT.LCD_CPIN = 1;                //Disable display controller
        }

#warning need 7.05ms(!) to execute
void LcdUpdate (void)   //Copies the LCD cache into the device RAM
        {
        int i;
	#ifdef china
	char j;
	#endif

        LcdSend(0x80, LCD_CMD);		//команды установки указателя памяти дисплея на 0,0
        LcdSend(0x40, LCD_CMD);

    #ifdef china                    		//если китайский дисплей - грузим пустую строку
		for (j = Cntr_X_RES; j>0; j--) LcdSend(0, LCD_DATA);
	#endif

        for (i = 0; i < LCD_CACHSIZE; i++)		//грузим данные
                {
                LcdSend(LcdCache[i], LCD_DATA);
		#ifdef china				//если дисплей китайский - догружаем каждую строку до размера его буфера
		if (++j == LCD_X_RES)
			{
			for (j = (Cntr_X_RES-LCD_X_RES); j>0; j--) LcdSend(0, LCD_DATA);
			j=0;
			}
		#endif
                }
        }

void LcdClear (void)    //Clears the display
{
        int i;
	for (i = 0; i < LCD_CACHSIZE; i++) LcdCache[i] = 0;	//забиваем всю память 0
	LcdUpdate ();
}

/*     or
void lcd_clear(void) // Clears the display   //Upd - 7
{
	lcdCacheIdx = 0;
	lcd_base_addr(lcdCacheIdx);
    // Set the entire cache to zero and write 0s to lcd
    for(int i=0;i<LCD_CACHE_SIZE;i++) {
		lcd_send(0, LCD_DATA);
    }
}
*/

void LcdInit ( void )	//инициализация SPI и дисплея
        {
        LCD_RST_PORT.LCD_RST_PIN = 1;       //настроили порты ввода/вывода
        LCD_RST_DDR.LCD_RST_PIN = LCD_DC_DDR.LCD_DC_PIN = LCD_CDDR.LCD_CPIN = LCD_MOSI_DDR.LCD_MOSI_PIN = LCD_CLK_DDR.LCD_CLK_PIN = 1;
        LCD_CLK_PORT.LCD_CLK_PIN = 0;
        	#ifndef SOFT_SPI
        LCD_SS_DDR.LCD_SS_PIN = 1;
        LCD_SS_PORT.LCD_SS_PIN = 0;
        	#endif
        delay_ms(1);
        	#ifndef SOFT_SPI
        //SPCR = SPCR_val;
        //SPSR = SPSR_val;
        	#endif
        LCD_RST_PORT.LCD_RST_PIN = 0;       //дернули ресет
        delay_ms(20);
        LCD_RST_PORT.LCD_RST_PIN = 1;

        LCD_CPORT.LCD_CPIN = 1;        //Disable LCD controller

/*
    LCD_PORT |= LCD_SCE_PIN;    // Disable LCD controller
    lcd_send(0x21, LCD_CMD);  // LCD Extended Commands
    lcd_send(0xC8, LCD_CMD);  // Set LCD Vop(Contrast)
    lcd_send(0x06, LCD_CMD);  // Set Temp coefficent
    lcd_send(0x13, LCD_CMD);  // LCD bias mode 1:48
    lcd_send(0x20, LCD_CMD);  // Standard Commands, Horizontal addressing
    lcd_send(0x0C, LCD_CMD);  // LCD in normal mode
*/
        LcdSend( 0b00100001, LCD_CMD ); 				//LCD Extended Commands
        LcdSend( 0b00000100+temp_control, LCD_CMD ); 	//Set Temp coefficent   //! НА 128 - ГЛЮЧИТ
#ifdef china
        LcdSend( 0b00001000|SPI_invert<<3, LCD_CMD ); 	//порядок битов в SPI
#endif
        LcdSend( 0b00010000+bias, LCD_CMD ); 			//LCD bias mode 1:48
#ifdef china
        LcdSend( 0b01000000+shift, LCD_CMD ); 			//первая строка выше экрана, отображаем со второй
#endif
	    LcdSend( 0b10000000+Vop, LCD_CMD ); 			//Set LCD Vop (Contrast)
#ifdef china
	    LcdSend( 0x20|x_mirror<<5|y_mirror<<4|power_down<<3, LCD_CMD );			//LCD Standard Commands
#endif
#ifndef china
        LcdSend( 0x20|power_down<<3|addressing<<2, LCD_CMD );				//LCD Standard Commands
#endif
        LcdSend( 0b00001000|((disp_config<<1|disp_config)&0b00000101), LCD_CMD ); 	//LCD mode
        LcdClear();
        }

void LcdContrast (unsigned char contrast) 	//contrast -> Contrast value from 0x00 to 0x7F
        {
        if (contrast > 0x7F) return;
        LcdSend( 0x21, LCD_CMD );               //LCD Extended Commands
        LcdSend( 0x80 | contrast, LCD_CMD );    //Set LCD Vop (Contrast)
        LcdSend( 0x20, LCD_CMD );               //LCD Standard Commands,Horizontal addressing mode
        }

void LcdMode (unsigned char mode) 		//режим дисплея: 0 - blank, 1 - all on, 2 - normal, 3 - inverse
        {
        if (mode > 3) return;
        LcdSend( 0b00001000|((mode<<1|mode)&0b00000101), LCD_CMD ); 	//LCD mode
        }

 void LcdPwrMode (void) 				//инвертирует состояние вкл/выкл дисплея
        {
        power_down = ~power_down;
        LcdSend( 0x20|power_down<<3, LCD_CMD );
        }
/*
void LcdPwrMode (void) 				//инвертирует состояние вкл/выкл дисплея
        {
        power_down = ~power_down;
        	#ifdef china
	LcdSend( 0x20|x_mirror<<4|y_mirror<<3|power_down<<2|addressing<<1, LCD_CMD );			//LCD Standard Commands
        	#elif
        LcdSend( 0x20|0<<2|addressing<<1, LCD_CMD );
                #endif
        }

void Lcd_off (void) 				//выкл дисплея
        {
        	#ifdef china
	LcdSend( 0x20|x_mirror<<4|y_mirror<<3|1<<2|addressing<<1, LCD_CMD );			//LCD Standard Commands
        	#elif
        LcdSend( 0x20|1<<2|addressing<<1, LCD_CMD );
                #endif
        }

void Lcd_on (void) 				//вкл дисплея
        {
        	#ifdef china
	LcdSend( 0x20|x_mirror<<4|y_mirror<<3|0<<2|addressing<<1, LCD_CMD );			//LCD Standard Commands
        	#elif
        LcdSend( 0x20|0<<2|addressing<<1, LCD_CMD );
                #endif
        }   */



void LcdImage (flash unsigned char *imageData)	//вывод изображения
        {
        unsigned int i;

        LcdSend(0x80, LCD_CMD);		//ставим указатель на 0,0
        LcdSend(0x40, LCD_CMD);
        for (i = 0; i < LCD_CACHSIZE; i++) LcdCache[i] = imageData[i];	//грузим данные
        }        
        
 void LcdImageRam (unsigned char *imageData)	//вывод изображения
        {
        unsigned int i;

        LcdSend(0x80, LCD_CMD);		//ставим указатель на 0,0
        LcdSend(0x40, LCD_CMD);
        for (i = 0; i < LCD_CACHSIZE; i++) LcdCache[i] = imageData[i];	//грузим данные
        }       
        

void LcdPixel (unsigned char x, unsigned char y, unsigned char mode)     //Displays a pixel at given absolute (x, y) location, mode -> Off, On or Xor
        {
        int index;
        unsigned char offset, data;

        if ( x > LCD_X_RES ) return;	//если передали в функцию муть - выходим
        if ( y > LCD_Y_RES ) return;

        index = (((int)(y)/8)*84)+x;    //считаем номер байта в массиве памяти дисплея
        offset  = y-((y/8)*8);          //считаем номер бита в этом байте

        data = LcdCache[index];         //берем байт по найденному индексу

        if ( mode == PIXEL_OFF ) data &= ( ~( 0x01 << offset ) );	//редактируем бит в этом байте
                else if ( mode == PIXEL_ON ) data |= ( 0x01 << offset );
                        else if ( mode  == PIXEL_XOR ) data ^= ( 0x01 << offset );

        LcdCache[index] = data;		//загружаем байт назад
        }

void LcdLine (int x1, int y1, int x2, int y2, unsigned char mode)  	//Draws a line between two points on the display - по Брезенхейму
        {
        signed int dy = 0;
        signed int dx = 0;
        signed int stepx = 0;
        signed int stepy = 0;
        signed int fraction = 0;

        if (x1>LCD_X_RES || x2>LCD_X_RES || y1>LCD_Y_RES || y2>LCD_Y_RES) return;

        dy = y2 - y1;
        dx = x2 - x1;
        if (dy < 0)
                {
                dy = -dy;
                stepy = -1;
                }
                else stepy = 1;
        if (dx < 0)
                {
                dx = -dx;
                stepx = -1;
                }
                else stepx = 1;
        dy <<= 1;
        dx <<= 1;
        LcdPixel(x1,y1,mode);
        if (dx > dy)
                {
                fraction = dy - (dx >> 1);
                while (x1 != x2)
                   {
                        if (fraction >= 0)
                                {
                                y1 += stepy;
                                fraction -= dx;
                                }
                        x1 += stepx;
                        fraction += dy;
                        LcdPixel(x1,y1,mode);
                   }
                }
                else
                        {
                        fraction = dx - (dy >> 1);
                        while (y1 != y2)
                                {
                                if (fraction >= 0)
                                        {
                                        x1 += stepx;
                                        fraction -= dy;
                                        }
                                y1 += stepy;
                                fraction += dx;
                                LcdPixel(x1,y1,mode);
                                }
                        }
        }

#warning Upd-7 not tested!
// Set the base address of the lcd
void lcd_base_addr(unsigned int addr) {   //Upd-7
	LcdSend(0x80 |(addr % LCD_X_RES), LCD_CMD);
	LcdSend(0x40 |(addr / LCD_X_RES), LCD_CMD);
}

/* Clears an area on a line */    //Upd-7
/*
void lcd_clear_area(unsigned char line, unsigned char startX, unsigned char endX)
{
    // Start and end positions of line
    int start = (line-1)*84+(startX-1);
    int end = (line-1)*84+(endX-1);

	lcd_base_addr(start);

    // Clear all data in range from cache
    for(uint16_t i=start;i<end;i++) {
       LcdSend(0, LCD_DATA);
    }
}  */

/*
 * Имя : LcdRect
 * Описание : Рисует незакрашенный прямоугольник
 * Аргумент(ы) : x1 -> абсолютная координата x левого верхнего угла
 * y1 -> абсолютная координата y левого верхнего угла
 * x2 -> абсолютная координата x правого нижнего угла
 * y2 -> абсолютная координата y правого нижнего угла
 * mode -> Off, On или Xor. Смотри enum в n3310.h
 * Возвращаемое значение : смотри возвращаемое значение в n3310lcd.h
 */  
      
 /*
byte LcdRect ( byte x1, byte y1, byte x2, byte y2, LcdPixelMode mode )    //not tested!
{
byte tmpIdx;
 
// Проверка границ
if ( ( x1 >= LCD_X_RES) || ( x2 >= LCD_X_RES) || ( y1 >= LCD_Y_RES) || ( y2 >= LCD_Y_RES) )
return OUT_OF_BORDER;
 
if ( ( x2 > x1 ) && ( y2 > y1 ) )
{
// Рисуем горизонтальные линии
for ( tmpIdx = x1; tmpIdx <= x2; tmpIdx++ )
{
LcdPixel( tmpIdx, y1, mode );
LcdPixel( tmpIdx, y2, mode );
}
 
// Рисуем вертикальные линии
for ( tmpIdx = y1; tmpIdx <= y2; tmpIdx++ )
{
LcdPixel( x1, tmpIdx, mode );
LcdPixel( x2, tmpIdx, mode );
}
 
// Установка флага изменений кэша
UpdateLcd = TRUE;
}
return OK;
}


*/

void LcdCircle(char x, char y, char radius, unsigned char mode)		//рисуем круг по координатам с радиусом - по Брезенхейму
        {
        signed char xc = 0;
        signed char yc = 0;
        signed char p = 0;

        if (x>LCD_X_RES || y>LCD_Y_RES) return;

        yc=radius;
        p = 3 - (radius<<1);
        while (xc <= yc)
                {
                LcdPixel(x + xc, y + yc, mode);
                LcdPixel(x + xc, y - yc, mode);
                LcdPixel(x - xc, y + yc, mode);
                LcdPixel(x - xc, y - yc, mode);
                LcdPixel(x + yc, y + xc, mode);
                LcdPixel(x + yc, y - xc, mode);
                LcdPixel(x - yc, y + xc, mode);
                LcdPixel(x - yc, y - xc, mode);
                if (p < 0) p += (xc++ << 2) + 6;
                        else p += ((xc++ - yc--)<<2) + 10;
                }
        }

void LcdBatt(int x1, int y1, int x2, int y2, unsigned char persent)	//рисуем батарейку с заполнением в %
        {
        unsigned char horizon_line,horizon_line2,i;
        if(persent>100)return;
        LcdLine(x1,y2,x2,y2,1);  //down
        LcdLine(x2,y1,x2,y2,1);  //right
	LcdLine(x1,y1,x1,y2,1);  //left
	LcdLine(x1,y1,x2,y1,1);  //up
	LcdLine(x1+7,y1-1,x2-7,y1-1,1);
	LcdLine(x1+7,y1-2,x2-7,y1-2,1);

        horizon_line=persent*(y2-y1-3)/100;
        for(i=0;i<horizon_line;i++) LcdLine(x1+2,y2-2-i,x2-2,y2-2-i,1);

        horizon_line2=(y2-y1-3);
        for(i=horizon_line2;i>horizon_line;i--) LcdLine(x1+2,y2-2-i,x2-2,y2-2-i,0);
	}

void LcdBar(int x1, int y1, int x2, int y2, unsigned char persent)	//рисуем прогресс-бар
        {
        unsigned char line;
        if(persent>100)return;
        LcdLine(x1+2,y2,x2-2,y2,1);  //down
        LcdLine(x2-2,y1,x2-2,y2,1);  //right
	LcdLine(x1+2,y1,x1+2,y2,1);  //left
	LcdLine(x1+2,y1,x2-2,y1,1);  //up

        LcdLine(x2-1,y1+1,x2-1,y2-1,1);  //right
	LcdLine(x1+1,y1+1,x1+1,y2-1,1);  //left

        LcdLine(x2,y1+2,x2,y2-2,1);  //right
	LcdLine(x1,y1+2,x1,y2-2,1);  //left

        line=persent*(x2-x1-7)/100-1;
        LcdLine(x1+4,y1+2,x2-4,y2-2,0);
        LcdLine(x1+4,y1+2,x1+4+line,y2-2,1);
	}

void LcdBarLine(unsigned char line, unsigned char persent)	//рисуем прошресс-бар
        {
        LcdBar(0, (line-1)*7+1, 83, (line-1)*7+5, persent);
        }

void LcdGotoXYFont (unsigned char x, unsigned char y)   //Sets cursor location to xy location. Range: 1,1 .. 14,6
        {
        if (x <= 14 && y <= 6) LcdCacheIdx = ( (int)(y) - 1 ) * 84 + ( (int)(x) - 1 ) * 6;
        }

void LcdTextBufClean (void)	//очистка текстового буфера
	{
	char i;
	for (i=0; i<14; i++) lcd_buf[i] = 0;
	}

void LcdChr (int ch)	//Displays a character at current cursor location and increment cursor location
 	{
     	unsigned char i;

        for ( i = 0; i < 5; i++ ) LcdCache[LcdCacheIdx++] = table[(ch*5+i)];	//выделяем байт-столбик из символа и грузим в массив - 5 раз
     	LcdCache[LcdCacheIdx++] = 0x00;	//добавляем пробел между символами
 	}

void LcdChrInv (int ch)	//Displays a character at current cursor location and increment cursor location
 	{
     	unsigned char i;

        for ( i = 0; i < 5; i++ ) LcdCache[LcdCacheIdx++] = ~(table[(ch*5+i)]);	//выделяем байт-столбик из символа и грузим в массив - 5 раз
     	LcdCache[LcdCacheIdx++] = 0xFF;	//добавляем пробел между символами
 	}

void LcdString (unsigned char x, unsigned char y)	//Displays a string at current cursor location
	{
	unsigned char i;

	if (x > 14 || y > 6) return;
	LcdGotoXYFont (x, y);
	for ( i = 0; i < 15-x; i++ ) if (lcd_buf[i]) LcdChr (lcd_buf[i]);
	LcdTextBufClean();
	}

void LcdStringInv (unsigned char x, unsigned char y)	//Displays a string at current cursor location
	{
	unsigned char i;

	if (x > 14 || y > 6) return;
	LcdGotoXYFont (x, y);
	for ( i = 0; i < 15-x; i++ ) if (lcd_buf[i]) LcdChrInv (lcd_buf[i]);
	LcdTextBufClean();
	}

void LcdChrBold (int ch)	//Displays a bold character at current cursor location and increment cursor location
 	{
     	unsigned char i;
     	unsigned char a = 0, b = 0, c = 0;

     	for ( i = 0; i < 5; i++ )
     	        {
     	        c = table[(ch*5+i)];		//выделяем столбец из символа

     	        b =  (c & 0x01) * 3;            //"растягиваем" столбец на два байта
              	b |= (c & 0x02) * 6;
              	b |= (c & 0x04) * 12;
              	b |= (c & 0x08) * 24;

              	c >>= 4;
              	a =  (c & 0x01) * 3;
              	a |= (c & 0x02) * 6;
              	a |= (c & 0x04) * 12;
              	a |= (c & 0x08) * 24;

     	        LcdCache[LcdCacheIdx] = b;	//копируем байты в экранный буфер
     	        LcdCache[LcdCacheIdx+1] = b;    //дублируем для получения жирного шрифта
     	        LcdCache[LcdCacheIdx+84] = a;
     	        LcdCache[LcdCacheIdx+85] = a;
     	        LcdCacheIdx = LcdCacheIdx+2;
     	        }

     	LcdCache[LcdCacheIdx++] = 0x00;	//для пробела между символами
     	LcdCache[LcdCacheIdx++] = 0x00;
 	}

void LcdStringBold (unsigned char x, unsigned char y)	//Displays a string at current cursor location
	{
	unsigned char i;

	if (x > 13 || y > 5) return;
	LcdGotoXYFont (x, y);
	for ( i = 0; i < 14-x; i++ ) if (lcd_buf[i]) LcdChrBold (lcd_buf[i]);
	LcdTextBufClean();
	}

void LcdChrBig (int ch)	//Displays a character at current cursor location and increment cursor location
 	{
     	unsigned char i;
     	unsigned char a = 0, b = 0, c = 0;

     	for ( i = 0; i < 5; i++ )
     	        {
     	        c = table[(ch*5+i)];		//выделяем столбец из символа

     	        b =  (c & 0x01) * 3;            //"растягиваем" столбец на два байта
              	b |= (c & 0x02) * 6;
              	b |= (c & 0x04) * 12;
              	b |= (c & 0x08) * 24;

              	c >>= 4;
              	a =  (c & 0x01) * 3;
              	a |= (c & 0x02) * 6;
              	a |= (c & 0x04) * 12;
              	a |= (c & 0x08) * 24;
     	        LcdCache[LcdCacheIdx] = b;
     	        LcdCache[LcdCacheIdx+84] = a;
     	        LcdCacheIdx = LcdCacheIdx+1;
     	        }

     	LcdCache[LcdCacheIdx++] = 0x00;
     	}

void LcdStringBig (unsigned char x, unsigned char y)	//Displays a string at current cursor location
	{
	unsigned char i;

	if (x > 14 || y > 5) return;
	LcdGotoXYFont (x, y);
	for ( i = 0; i < 15-x; i++ ) if (lcd_buf[i]) LcdChrBig (lcd_buf[i]);
	LcdTextBufClean();
	}

#pragma used-