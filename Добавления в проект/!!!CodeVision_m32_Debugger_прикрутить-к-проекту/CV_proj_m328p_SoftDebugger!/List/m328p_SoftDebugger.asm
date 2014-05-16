
;CodeVisionAVR C Compiler V2.05.0 Professional
;(C) Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type                : ATmega328P
;Program type             : Application
;Clock frequency          : 8,000000 MHz
;Memory model             : Small
;Optimize for             : Size
;(s)printf features       : int, width
;(s)scanf features        : int, width
;External RAM size        : 0
;Data Stack size          : 512 byte(s)
;Heap size                : 0 byte(s)
;Promote 'char' to 'int'  : Yes
;'char' is unsigned       : Yes
;8 bit enums              : Yes
;global 'const' stored in FLASH: No
;Enhanced core instructions    : On
;Smart register allocation     : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATmega328P
	#pragma AVRPART MEMORY PROG_FLASH 32768
	#pragma AVRPART MEMORY EEPROM 1024
	#pragma AVRPART MEMORY INT_SRAM SIZE 2303
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x100

	#define CALL_SUPPORTED 1

	.LISTMAC
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU EECR=0x1F
	.EQU EEDR=0x20
	.EQU EEARL=0x21
	.EQU EEARH=0x22
	.EQU SPSR=0x2D
	.EQU SPDR=0x2E
	.EQU SMCR=0x33
	.EQU MCUSR=0x34
	.EQU MCUCR=0x35
	.EQU WDTCSR=0x60
	.EQU UCSR0A=0xC0
	.EQU UDR0=0xC6
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F
	.EQU GPIOR0=0x1E

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0100
	.EQU __SRAM_END=0x08FF
	.EQU __DSTACK_SIZE=0x0200
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __GETD1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X+
	LD   R22,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _time1=R4
	.DEF _time2=R3
	.DEF _time3=R6
	.DEF _tsk2c=R5
	.DEF _tsk3m=R8
	.DEF _led=R7
	.DEF _tmp=R10

;GPIOR0 INITIALIZATION VALUE
	.EQU __GPIOR0_INIT=0x00

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	JMP  __RESET
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  _timer0_compare
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00

_tbl10_G100:
	.DB  0x10,0x27,0xE8,0x3,0x64,0x0,0xA,0x0
	.DB  0x1,0x0
_tbl16_G100:
	.DB  0x0,0x10,0x0,0x1,0x10,0x0,0x1,0x0

_0x63:
	.DB  0x0
_0x0:
	.DB  0x64,0x62,0x20,0x49,0x44,0x3A,0x25,0x64
	.DB  0xD,0xA,0x0,0x64,0x62,0x3E,0x0,0x25
	.DB  0x63,0x25,0x78,0x0,0x69,0x6F,0x70,0x6F
	.DB  0x72,0x74,0x20,0x25,0x78,0x3D,0x20,0x25
	.DB  0x78,0xA,0xD,0x0,0x25,0x63,0x25,0x78
	.DB  0x25,0x78,0x0,0x69,0x6F,0x70,0x6F,0x72
	.DB  0x74,0x20,0x25,0x78,0x2D,0x3E,0x20,0x25
	.DB  0x78,0xA,0xD,0x0,0x25,0x63,0x25,0x64
	.DB  0x0,0x64,0x61,0x74,0x61,0x52,0x65,0x67
	.DB  0x20,0x25,0x64,0x3D,0x20,0x25,0x78,0xA
	.DB  0xD,0x0,0x25,0x63,0x25,0x64,0x25,0x78
	.DB  0x0,0x64,0x61,0x74,0x61,0x52,0x65,0x67
	.DB  0x20,0x25,0x64,0x2D,0x3E,0x20,0x25,0x78
	.DB  0xA,0xD,0x0,0x72,0x65,0x67,0x69,0x73
	.DB  0x74,0x65,0x72,0x20,0x23,0x20,0x6D,0x75
	.DB  0x73,0x74,0x20,0x62,0x65,0x20,0x3C,0x20
	.DB  0x32,0x32,0xD,0xA,0x0,0x6D,0x65,0x6D
	.DB  0x6F,0x72,0x79,0x20,0x25,0x78,0x3D,0x20
	.DB  0x25,0x78,0xA,0xD,0x0,0x6D,0x65,0x6D
	.DB  0x6F,0x72,0x79,0x20,0x25,0x78,0x2D,0x3E
	.DB  0x20,0x25,0x78,0xA,0xD,0x0,0x64,0x61
	.DB  0x74,0x61,0x20,0x73,0x74,0x6B,0x20,0x61
	.DB  0x64,0x64,0x72,0x3D,0x25,0x78,0xA,0xD
	.DB  0x0,0x68,0x77,0x20,0x73,0x74,0x6B,0x20
	.DB  0x61,0x64,0x64,0x72,0x3D,0x25,0x78,0xA
	.DB  0xD,0x0,0x53,0x52,0x45,0x47,0x3D,0x25
	.DB  0x78,0xA,0xD,0x0,0x2D,0x2D,0x25,0x73
	.DB  0x2D,0x2D,0xA,0xD,0x0,0x25,0x78,0xD
	.DB  0xA,0x0,0x67,0x20,0x2D,0x2D,0x20,0x67
	.DB  0x6F,0x20,0x72,0x75,0x6E,0x20,0x74,0x61
	.DB  0x72,0x67,0x65,0x74,0x20,0x63,0x6F,0x64
	.DB  0x65,0x20,0xD,0xA,0x0,0x78,0x20,0x2D
	.DB  0x2D,0x20,0x72,0x65,0x62,0x6F,0x6F,0x74
	.DB  0x20,0x6D,0x63,0x75,0x20,0xD,0xA,0x0
	.DB  0x69,0x20,0x69,0x6F,0x72,0x65,0x67,0x20
	.DB  0x2D,0x2D,0x20,0x72,0x65,0x61,0x64,0x20
	.DB  0x69,0x2F,0x6F,0x20,0x72,0x65,0x67,0x69
	.DB  0x73,0x74,0x65,0x72,0x3B,0x20,0x69,0x6F
	.DB  0x72,0x65,0x67,0x20,0x69,0x6E,0x20,0x68
	.DB  0x65,0x78,0xD,0xA,0x0,0x49,0x20,0x69
	.DB  0x6F,0x72,0x65,0x67,0x20,0x64,0x61,0x74
	.DB  0x61,0x20,0x2D,0x2D,0x20,0x77,0x72,0x69
	.DB  0x74,0x65,0x20,0x69,0x2F,0x6F,0x20,0x72
	.DB  0x65,0x67,0x69,0x73,0x74,0x65,0x72,0x20
	.DB  0x2D,0x20,0x68,0x65,0x78,0xD,0xA,0x0
	.DB  0x72,0x20,0x64,0x61,0x74,0x61,0x72,0x65
	.DB  0x67,0x20,0x2D,0x2D,0x20,0x72,0x65,0x61
	.DB  0x64,0x3B,0x20,0x64,0x61,0x74,0x61,0x72
	.DB  0x65,0x67,0x20,0x69,0x6E,0x20,0x64,0x65
	.DB  0x63,0x20,0xD,0xA,0x0,0x52,0x20,0x64
	.DB  0x61,0x74,0x61,0x72,0x65,0x67,0x20,0x64
	.DB  0x61,0x74,0x61,0x20,0x2D,0x2D,0x20,0x77
	.DB  0x72,0x69,0x74,0x65,0x3B,0x20,0x64,0x61
	.DB  0x74,0x61,0x20,0x69,0x6E,0x20,0x68,0x65
	.DB  0x78,0x20,0xD,0xA,0x0,0x6D,0x20,0x61
	.DB  0x64,0x64,0x72,0x20,0x2D,0x2D,0x20,0x72
	.DB  0x65,0x61,0x64,0x20,0x6D,0x65,0x6D,0x6F
	.DB  0x72,0x79,0x3B,0x20,0x68,0x65,0x78,0xD
	.DB  0xA,0x0,0x4D,0x20,0x61,0x64,0x64,0x72
	.DB  0x20,0x64,0x61,0x74,0x20,0x2D,0x2D,0x20
	.DB  0x77,0x72,0x69,0x74,0x65,0x20,0x6D,0x65
	.DB  0x6D,0x6F,0x72,0x79,0x3B,0x20,0x68,0x65
	.DB  0x78,0xD,0xA,0x0,0x64,0x20,0x2D,0x2D
	.DB  0x20,0x74,0x6F,0x70,0x20,0x6F,0x66,0x20
	.DB  0x64,0x61,0x74,0x61,0x20,0x73,0x74,0x61
	.DB  0x63,0x6B,0x3B,0x20,0x68,0x65,0x78,0xD
	.DB  0xA,0x0,0x77,0x20,0x2D,0x2D,0x20,0x74
	.DB  0x6F,0x70,0x20,0x6F,0x66,0x20,0x68,0x61
	.DB  0x72,0x64,0x77,0x61,0x72,0x65,0x20,0x73
	.DB  0x74,0x61,0x63,0x6B,0x3B,0x20,0x68,0x65
	.DB  0x78,0xD,0xA,0x0,0x73,0x20,0x2D,0x2D
	.DB  0x20,0x6D,0x63,0x75,0x20,0x73,0x74,0x61
	.DB  0x74,0x75,0x73,0x20,0x72,0x65,0x67,0x69
	.DB  0x73,0x74,0x65,0x72,0x3B,0x20,0x68,0x65
	.DB  0x78,0xD,0xA,0x0,0x6C,0x2C,0x20,0x6C
	.DB  0x63,0x2C,0x20,0x6C,0x64,0x20,0x2D,0x2D
	.DB  0x20,0x6C,0x6F,0x67,0x67,0x65,0x64,0x20
	.DB  0x76,0x61,0x72,0x69,0x61,0x62,0x6C,0x65
	.DB  0x73,0xD,0xA,0x0,0x54,0x61,0x72,0x67
	.DB  0x65,0x74,0x20,0x63,0x6F,0x64,0x65,0x20
	.DB  0x63,0x6F,0x6D,0x6D,0x61,0x6E,0x64,0x73
	.DB  0x3A,0xD,0xA,0x0,0x20,0x20,0x64,0x65
	.DB  0x62,0x75,0x67,0x28,0x44,0x65,0x62,0x75
	.DB  0x67,0x49,0x44,0x29,0x20,0x2D,0x2D,0x20
	.DB  0x65,0x6E,0x74,0x65,0x72,0x20,0x64,0x65
	.DB  0x62,0x75,0x67,0x67,0x65,0x72,0xD,0xA
	.DB  0x0,0x20,0x20,0x72,0x65,0x70,0x6F,0x72
	.DB  0x74,0x56,0x64,0x65,0x63,0x2C,0x20,0x72
	.DB  0x65,0x70,0x6F,0x72,0x74,0x56,0x68,0x65
	.DB  0x78,0x20,0x2D,0x2D,0x20,0x70,0x72,0x69
	.DB  0x6E,0x74,0x20,0x76,0x61,0x72,0x69,0x61
	.DB  0x62,0x6C,0x65,0x73,0xD,0xA,0x0,0x20
	.DB  0x20,0x6C,0x6F,0x67,0x69,0x6E,0x69,0x74
	.DB  0x2C,0x20,0x6C,0x6F,0x67,0x56,0x20,0x2D
	.DB  0x2D,0x20,0x73,0x61,0x76,0x65,0x20,0x76
	.DB  0x61,0x72,0x69,0x61,0x62,0x6C,0x65,0x73
	.DB  0xD,0xA,0x0,0xD,0xD,0xA,0x20,0x44
	.DB  0x42,0x20,0x32,0x2E,0x30,0xD,0xA,0x0
	.DB  0x54,0x61,0x72,0x67,0x65,0x74,0x20,0x46
	.DB  0x61,0x69,0x6C,0x65,0x64,0x0,0x25,0x70
	.DB  0x0,0x74,0x6D,0x70,0x0,0x3D,0x25,0x64
	.DB  0xA,0xD,0x0,0x45,0x6E,0x74,0x65,0x72
	.DB  0x20,0x61,0x20,0x6E,0x75,0x6D,0x62,0x65
	.DB  0x72,0x3A,0x0,0x25,0x73,0x0,0xA,0xD
	.DB  0x25,0x73,0xA,0xD,0x0
_0x2040060:
	.DB  0x1
_0x2040000:
	.DB  0x2D,0x4E,0x41,0x4E,0x0,0x49,0x4E,0x46
	.DB  0x0

__GLOBAL_INI_TBL:
	.DW  0x01
	.DW  0x0A
	.DW  _0x63*2

	.DW  0x01
	.DW  __seed_G102
	.DW  _0x2040060*2

_0xFFFFFFFF:
	.DW  0

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  MCUCR,R31
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	WDR
	IN   R26,MCUSR
	CBR  R26,8
	OUT  MCUSR,R26
	STS  WDTCSR,R31
	STS  WDTCSR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,LOW(__SRAM_START)
	LDI  R27,HIGH(__SRAM_START)
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;GPIOR0 INITIALIZATION
	LDI  R30,__GPIOR0_INIT
	OUT  GPIOR0,R30

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x300

	.CSEG
;
;
;//********************************************************
;#include "debugger.c"
;/*************************************************************************
;A simple target debug program meant to be used interactivly.
;
;The target is a program written in Codevision C running on a Mega32.
;
;Bruce Land -- BRL4@cornell.edu
;VERSION 10feb2007
;**************************************************************************/
;//================
;// eliminate the debugger compleletly if defined
;//#define nullify_debugger
;
;
;//================
;
;//#include <mega32.h>
;#include <mega328p.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif
;#include <delay.h>
;
;#ifndef nullify_debugger
;//Uncomment this define if you want to activate the receive interrupt
;//The rxc ISR allows you to enter debugger with a <control-c>
;//#define use_rxc_isr
;//With it commented, the debugger does not use any interrupts,
;//nor turn on the global I bit.
;//Uncommented, it uses the UART rxc ISR and enables the I bit.
;
;//Comment this define if you want to disable verbose help
;#define verbose
;//Commenting this saves about 300 bytes of flash
;
;//Uncomment this define to enable inline reporting
;//of registers and memory
;#define use_reporting
;
;//Uncomment this define to enable logging of a variable to RAM on the fly
;#define use_logging
;
;/**************************************************************/
;// UART communication code
;//written by BRL4
;
;#pragma regalloc-
;//variable set by break and <cntl-c>
;unsigned char db_break_id;
;unsigned char db_t0temp, db_t1temp, db_t2temp;
;unsigned int db_swstk, db_hwstk;
;unsigned char db_regs[32], db_sreg;
;unsigned char db_cmd_running;
;#ifdef use_logging
;unsigned int db_logcount;
;unsigned char db_logMax, db_logname[16];
;#endif
;#pragma regalloc+
;
;//define entries
;void cmd(void);
;void mymain(void);
;
;/**********************************************************/
;//macro to force entry into cmd shell
;//DEBUG jumps to cmd function, and freezes clocks.
;//It is defined here because of the tight relationship with getchar
;
;
;void saveitall(void)
; 0000 0004 {

	.CSEG
_saveitall:
;        #asm
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
;} //end saveitall
	RET
;
;void loaddatareg(void)
;{
_loaddatareg:
;        #asm
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
;} //end loaddatareg
	RET
;
;
;#define debug(id) \
;	do{      	\
;	db_t0temp = TCCR0A;  \
;	db_t1temp = TCCR1B; \
;	db_t2temp = TCCR2A;  \
;	TCCR0A = 0;  \
;	TCCR1B = 0; \
;	TCCR2A = 0;  \
;	saveitall(); \
;	db_break_id = id;  \
;	cmd();     \
;	loaddatareg(); \
;	TCCR0A = db_t0temp;	\
;	TCCR1B = db_t1temp;\
;	TCCR2A = db_t2temp;	\
;	}while(0)
;
;#ifdef use_reporting
;//read a reg on the fly
;void peekreg(void)
;{
;#asm
;}
;
;#define reportR(regnum) \
;    do{ \
;        db_temp1 = regnum;  \
;        peekreg(); \
;        printf("R%d=%x\n\r",db_temp1,db_temp) ;  \
;    }while(0)
;
;//read a I/Oreg on the fly
;void peekio(void)
;{
;#asm
;}
;
;#define reportI(regnum) \
;    do{ \
;        db_temp1 = regnum;  \
;        peekio(); \
;        printf("I%x=%x\n\r",db_temp1,db_temp) ;  \
;    }while(0)
;
;//read memory in the fly
;void peekmem(void)
;{
;#asm
;}
;#define reportM(addr) \
;    do{ \
;        db_temp3 = addr;  \
;        peekmem(); \
;        printf("M%x=%x\n\r",db_temp3,db_temp) ;  \
;    }while(0)
;
;#define reportVhex(varname) \
;    do{ \
;        printf(#varname);   \
;        printf("=%x\n\r",varname) ;  \
;    }while(0)
;
;#define reportVdec(varname) \
;    do{ \
;        printf(#varname);   \
;        printf("=%d\n\r",varname) ;  \
;    }while(0)
;#endif
;
;#ifdef use_logging
;    #include <mem.h>
;    #define logInit(varname,logsize) \
;    do{ \
;        db_logMax = logsize;  \
;        db_logcount=0;   \
;        sprintf(db_logname,"%p", #varname); \
;    }while(0)
;
;    #define logV(varname) \
;    do{ \
;        if (db_logcount >= db_logMax) debug(254);    \
;        else pokeb(0x60+db_logcount++,varname); \
;    }while(0)
;
;#endif
;
;/**********************************************************/
;//define the structures and turn on the UART
;void db_init_uart(long baud)
;{
_db_init_uart:
;   	//uses the clock freq set in the config dialog box
; 	//UBRRL= _MCU_CLOCK_FREQUENCY_ /(baud*16L) - 1L;
;    UBRR0L= _MCU_CLOCK_FREQUENCY_ /(baud*16L) - 1L;
;	baud -> Y+0
	CALL __GETD1S0
	__GETD2N 0x10
	CALL __MULD12
	__GETD2N 0x7A1200
	CALL __DIVD21
	__SUBD1N 1
	STS  196,R30
; 	UCSR0B=0x18; // activate UART
	LDI  R30,LOW(24)
	STS  193,R30
; 	#ifdef use_rxc_isr
; 		//UCSR0B.7=1;   //turn on ISR
;        UCSR0B=128;   //turn on ISR
; 		#asm("sei")
; 	#endif
;}
	ADIW R28,4
	RET
;
;//redefine getchar if necessary
;#ifdef use_rxc_isr
;    #define _ALTERNATE_GETCHAR_
;
;    #pragma regalloc-
;    char db_temp_char, db_char_flag;
;    #pragma regalloc+
;
;    char getchar(void)
;    {
;    	db_char_flag=0;
;        while (db_char_flag==0); //set by ISR
;    	return db_temp_char;	 //set by ISR
;    }
;
;    interrupt [USART_RXC] void uart_rec(void)
;    {
;    	db_temp_char=UDR0;   	//get a char
;    	//IF cntl-c and NOT currently in the debugger
;    	if (db_temp_char==0x03 && db_cmd_running==0)
;    	{
;    	    #asm("sei")
;    		debug(255);			//then break to debugger
;    	}
;    	else
;    		db_char_flag = 1;
;    } //end ISR
;#endif
;
;#include <stdio.h>
;
;/**********************************************************/
;
;//global variables for debugger
;#pragma regalloc-
;//MUST BE in memory so asmembler can find them
;unsigned char db_temp, db_temp1, db_temp2 ;
;unsigned int db_temp3, db_temp4;
;#pragma regalloc+
;
;//********************************************************
;//the debug function
;void cmd( void )
;{
_cmd:
;	unsigned char cmd1str[32], current_char, in_count;
;    unsigned int iter;
;
;   	//this variable is cleared by the "g" (go) command
;   	db_cmd_running = 1;
	SBIW R28,32
	CALL __SAVELOCR4
;	cmd1str -> Y+4
;	current_char -> R17
;	in_count -> R16
;	iter -> R18,R19
	LDI  R30,LOW(1)
	STS  _db_cmd_running,R30
;
;   	//print break source: 255 is <cntl-c>
;	printf("db ID:%d\r\n",db_break_id);
	__POINTW1FN _0x0,0
	ST   -Y,R31
	ST   -Y,R30
	LDS  R30,_db_break_id
	CALL SUBOPT_0x0
	CALL SUBOPT_0x1
;
;	while(db_cmd_running)
_0x3:
	LDS  R30,_db_cmd_running
	CPI  R30,0
	BRNE PC+3
	JMP _0x5
;	{
;	    cmd1str[0]=0;
	LDI  R30,LOW(0)
	STD  Y+4,R30
;
;	    //get the next command
;    	//handles backspace, <enter>
;            printf("db>");
	__POINTW1FN _0x0,11
	CALL SUBOPT_0x2
;            in_count=0;
	LDI  R16,LOW(0)
;        while ( (current_char=getchar()) != '\r' /*'='*/)  //<enter>
_0x6:
	CALL _getchar
	MOV  R17,R30
	CPI  R30,LOW(0xD)
	BREQ _0x8
;        {
;        	putchar(current_char);
	ST   -Y,R17
	CALL _putchar
;        	if (current_char == 0x08 & in_count>0)	//backspace
	MOV  R26,R17
	LDI  R30,LOW(8)
	CALL __EQB12
	MOV  R0,R30
	MOV  R26,R16
	LDI  R30,LOW(0)
	CALL __GTB12U
	AND  R30,R0
	BREQ _0x9
;        		--in_count;
	SUBI R16,LOW(1)
;        	else
	RJMP _0xA
_0x9:
;        		cmd1str[in_count++]=current_char;
	MOV  R30,R16
	SUBI R16,-1
	LDI  R31,0
	MOVW R26,R28
	ADIW R26,4
	ADD  R30,R26
	ADC  R31,R27
	ST   Z,R17
;        }
_0xA:
	RJMP _0x6
_0x8:
;        cmd1str[in_count] = 0;	//terminate the string
	MOV  R30,R16
	LDI  R31,0
	MOVW R26,R28
	ADIW R26,4
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(0)
	ST   X,R30
;        putchar('\r');  		//emit carriage return
	LDI  R30,LOW(13)
	ST   -Y,R30
	CALL _putchar
;        putchar('\n'); 			//line feed makes output nicer
	LDI  R30,LOW(10)
	ST   -Y,R30
	CALL _putchar
;
;        //execute the shell command
;        //printf("%s\r\n", cmd1str);
;
;        //target go comand
;        //and return to target program
;        if (cmd1str[0]=='g') db_cmd_running = 0;
	LDD  R26,Y+4
	CPI  R26,LOW(0x67)
	BRNE _0xB
	LDI  R30,LOW(0)
	STS  _db_cmd_running,R30
;
;        //reset -- this forces a reboot!!!
;        if (cmd1str[0]=='x')
_0xB:
	LDD  R26,Y+4
	CPI  R26,LOW(0x78)
	BRNE _0xC
;        {
;            while(!UCSR0A&0x0010000); //wait for last char to transmit
_0xD:
	LDS  R30,192
	CALL __LNEGB1
	CLR  R31
	CLR  R22
	CLR  R23
	__ANDD1N 0x10000
	CALL __CPD10
	BRNE _0xD
;            UCSR0B = 0x00;    //turn off uart
	LDI  R30,LOW(0)
	STS  193,R30
;            //Force a watchdog reset
;           // WDTCR=0x08;   //enable watchdog
;           WDTCSR=0x08;   //enable watchdog
	LDI  R30,LOW(8)
	STS  96,R30
;            while(1);     //just wait for watchdog to reset machine
_0x10:
	RJMP _0x10
;        }
;
;        //read an i/o register
;        if (cmd1str[0]=='i')
_0xC:
	LDD  R26,Y+4
	CPI  R26,LOW(0x69)
	BREQ PC+3
	JMP _0x13
;        {
;        	sscanf(cmd1str,"%c%x", &db_temp, &db_temp1);
	CALL SUBOPT_0x3
	CALL SUBOPT_0x4
	CALL SUBOPT_0x5
	CALL SUBOPT_0x6
;
;        	//check to see if ioreg is timer control
;        	//(which were saved when cmd was entered)
;        	if (db_temp1==0x33)
	LDS  R26,_db_temp1
	CPI  R26,LOW(0x33)
	BRNE _0x14
;        		db_temp = db_t0temp;
	LDS  R30,_db_t0temp
	STS  _db_temp,R30
;        	else if (db_temp1==0x2e)
	RJMP _0x15
_0x14:
	LDS  R26,_db_temp1
	CPI  R26,LOW(0x2E)
	BRNE _0x16
;        		db_temp = db_t1temp;
	LDS  R30,_db_t1temp
	STS  _db_temp,R30
;        	else if (db_temp1==0x25)
	RJMP _0x17
_0x16:
	LDS  R26,_db_temp1
	CPI  R26,LOW(0x25)
	BRNE _0x18
;        		db_temp = db_t2temp;
	LDS  R30,_db_t2temp
	STS  _db_temp,R30
;        	else
	RJMP _0x19
_0x18:
;        	{
;        	#asm
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
;        	}
_0x19:
_0x17:
_0x15:
;        	printf("ioport %x= %x\n\r",db_temp1,db_temp) ;
	__POINTW1FN _0x0,20
	CALL SUBOPT_0x7
	CALL SUBOPT_0x8
	CALL SUBOPT_0x9
;        }
;
;        //write an i/o register
;        if (cmd1str[0]=='I')
_0x13:
	LDD  R26,Y+4
	CPI  R26,LOW(0x49)
	BREQ PC+3
	JMP _0x1A
;        {
;        	sscanf(cmd1str,"%c%x%x", &db_temp,&db_temp1, &db_temp2);
	CALL SUBOPT_0x3
	CALL SUBOPT_0xA
	CALL SUBOPT_0x5
	CALL SUBOPT_0xB
	CALL SUBOPT_0xC
;        	//check to see if ioreg is timer control
;        	//(which were saved when cmd was entered)
;        	if (db_temp1==0x33)
	LDS  R26,_db_temp1
	CPI  R26,LOW(0x33)
	BRNE _0x1B
;        		db_t0temp = db_temp2;
	LDS  R30,_db_temp2
	STS  _db_t0temp,R30
;        	else if (db_temp1==0x2e)
	RJMP _0x1C
_0x1B:
	LDS  R26,_db_temp1
	CPI  R26,LOW(0x2E)
	BRNE _0x1D
;        		db_t1temp = db_temp2;
	LDS  R30,_db_temp2
	STS  _db_t1temp,R30
;        	else if (db_temp1==0x25)
	RJMP _0x1E
_0x1D:
	LDS  R26,_db_temp1
	CPI  R26,LOW(0x25)
	BRNE _0x1F
;        		db_t2temp = db_temp2;
	LDS  R30,_db_temp2
	STS  _db_t2temp,R30
;        	else
	RJMP _0x20
_0x1F:
;        	{
;        	#asm
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
;        	}
_0x20:
_0x1E:
_0x1C:
;        	printf("ioport %x-> %x\n\r",db_temp1,db_temp2) ;
	__POINTW1FN _0x0,43
	CALL SUBOPT_0x7
	CALL SUBOPT_0xD
	CALL SUBOPT_0x9
;        }
;
;
;        //read a data register
;        if(cmd1str[0]=='r')
_0x1A:
	LDD  R26,Y+4
	CPI  R26,LOW(0x72)
	BRNE _0x21
;        {
;        	sscanf(cmd1str,"%c%d", &db_temp, &db_temp2);
	CALL SUBOPT_0x3
	__POINTW1FN _0x0,60
	CALL SUBOPT_0xE
	CALL SUBOPT_0x6
;        	if (db_temp2 < 32)
	LDS  R26,_db_temp2
	CPI  R26,LOW(0x20)
	BRSH _0x22
;        	{
;        	        db_temp = db_regs[db_temp2];
	CALL SUBOPT_0xF
	LD   R30,Z
	STS  _db_temp,R30
;        	        printf("dataReg %d= %x\n\r", db_temp2, db_temp);
	__POINTW1FN _0x0,65
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xD
	CALL SUBOPT_0x8
	CALL SUBOPT_0x9
;        	 }
;        }
_0x22:
;
;        //WRITE a data register
;        //don't mess with C registers 22-31
;        if(cmd1str[0]=='R')
_0x21:
	LDD  R26,Y+4
	CPI  R26,LOW(0x52)
	BRNE _0x23
;        {
;        	sscanf(cmd1str,"%c%d%x", &db_temp, &db_temp2, &db_temp4);
	CALL SUBOPT_0x3
	__POINTW1FN _0x0,82
	CALL SUBOPT_0xE
	CALL SUBOPT_0x10
;        	if (db_temp2 < 22)
	LDS  R26,_db_temp2
	CPI  R26,LOW(0x16)
	BRSH _0x24
;        	{
;        	        db_regs[db_temp2] = db_temp4;
	CALL SUBOPT_0xF
	LDS  R26,_db_temp4
	STD  Z+0,R26
;        	        printf("dataReg %d-> %x\n\r", db_temp2, db_temp4);
	__POINTW1FN _0x0,89
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xD
	CALL SUBOPT_0x11
;        	}
;        	else printf("register # must be < 22\r\n");
	RJMP _0x25
_0x24:
	__POINTW1FN _0x0,107
	CALL SUBOPT_0x2
;
;        }
_0x25:
;
;        //read an memory location
;        if (cmd1str[0]=='m')
_0x23:
	LDD  R26,Y+4
	CPI  R26,LOW(0x6D)
	BRNE _0x26
;        {
;        	sscanf(cmd1str,"%c%x", &db_temp, &db_temp3);
	CALL SUBOPT_0x3
	CALL SUBOPT_0x4
	CALL SUBOPT_0x12
	CALL SUBOPT_0x6
;        	#asm
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
;        	printf("memory %x= %x\n\r",db_temp3,db_temp) ;
	__POINTW1FN _0x0,133
	CALL SUBOPT_0x13
	CALL SUBOPT_0x8
	CALL SUBOPT_0x9
;        }
;
;        //WRITE an memory location
;        if (cmd1str[0]=='M')
_0x26:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4D)
	BRNE _0x27
;        {
;        	sscanf(cmd1str,"%c%x%x", &db_temp, &db_temp3, &db_temp4);
	CALL SUBOPT_0x3
	CALL SUBOPT_0xA
	CALL SUBOPT_0x12
	CALL SUBOPT_0x10
;        	#asm
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
;        	db_temp4 = db_temp4 & 0xff;
	__ANDBMNN _db_temp4,1,0
;        	printf("memory %x-> %x\n\r",db_temp3,db_temp4) ;
	__POINTW1FN _0x0,149
	CALL SUBOPT_0x13
	CALL SUBOPT_0x11
;        }
;
;        //get data stack location
;        if (cmd1str[0]=='d')
_0x27:
	LDD  R26,Y+4
	CPI  R26,LOW(0x64)
	BRNE _0x28
;        {
;         	printf("data stk addr=%x\n\r", db_swstk);
	__POINTW1FN _0x0,166
	ST   -Y,R31
	ST   -Y,R30
	LDS  R30,_db_swstk
	LDS  R31,_db_swstk+1
	CALL SUBOPT_0x14
;        }
;
;        //get hw stack location
;        //add two because it is read in a function
;        if (cmd1str[0]=='w')
_0x28:
	LDD  R26,Y+4
	CPI  R26,LOW(0x77)
	BRNE _0x29
;        {
;         	printf("hw stk addr=%x\n\r", db_hwstk+2);
	__POINTW1FN _0x0,185
	ST   -Y,R31
	ST   -Y,R30
	LDS  R30,_db_hwstk
	LDS  R31,_db_hwstk+1
	ADIW R30,2
	CALL SUBOPT_0x14
;        }
;
;        //get status register
;        if (cmd1str[0]=='s')
_0x29:
	LDD  R26,Y+4
	CPI  R26,LOW(0x73)
	BRNE _0x2A
;        {
;         	printf("SREG=%x\n\r", db_sreg);
	__POINTW1FN _0x0,202
	ST   -Y,R31
	ST   -Y,R30
	LDS  R30,_db_sreg
	CALL SUBOPT_0x0
	CALL SUBOPT_0x1
;        }
;
;        #ifdef use_logging
;        //dump log
;        if (cmd1str[0]=='l')
_0x2A:
	LDD  R26,Y+4
	CPI  R26,LOW(0x6C)
	BRNE _0x2B
;        {
;            if (cmd1str[1]=='c') db_logcount=0;
	LDD  R26,Y+5
	CPI  R26,LOW(0x63)
	BRNE _0x2C
	LDI  R30,LOW(0)
	STS  _db_logcount,R30
	STS  _db_logcount+1,R30
;            else if (cmd1str[1]=='d')
	RJMP _0x2D
_0x2C:
	LDD  R26,Y+5
	CPI  R26,LOW(0x64)
	BRNE _0x2E
;            {
;                printf("--%s--\n\r",db_logname);
	CALL SUBOPT_0x15
;                for(iter=0; iter<db_logcount; iter++)
	__GETWRN 18,19,0
_0x30:
	CALL SUBOPT_0x16
	BRSH _0x31
;                    printf("%d\r\n",peekb(0x60+iter));
	__POINTW1FN _0x0,6
	CALL SUBOPT_0x17
	CALL SUBOPT_0x1
	__ADDWRN 18,19,1
	RJMP _0x30
_0x31:
;            else
	RJMP _0x32
_0x2E:
;            {
;                printf("--%s--\n\r",db_logname);
	CALL SUBOPT_0x15
;                for(iter=0; iter<db_logcount; iter++)
	__GETWRN 18,19,0
_0x34:
	CALL SUBOPT_0x16
	BRSH _0x35
;                    printf("%x\r\n",peekb(0x60+iter));
	__POINTW1FN _0x0,221
	CALL SUBOPT_0x17
	CALL SUBOPT_0x1
	__ADDWRN 18,19,1
	RJMP _0x34
_0x35:
_0x32:
_0x2D:
;        }
;        #endif
;
;        //help
;        if  (cmd1str[0]=='h')
_0x2B:
	LDD  R26,Y+4
	CPI  R26,LOW(0x68)
	BREQ PC+3
	JMP _0x36
;        {
;        #ifdef verbose
;            printf("g -- go run target code \r\n");
	__POINTW1FN _0x0,226
	CALL SUBOPT_0x2
;            printf("x -- reboot mcu \r\n");
	__POINTW1FN _0x0,253
	CALL SUBOPT_0x2
;            printf("i ioreg -- read i/o register; ioreg in hex\r\n");
	__POINTW1FN _0x0,272
	CALL SUBOPT_0x2
;            printf("I ioreg data -- write i/o register - hex\r\n");
	__POINTW1FN _0x0,317
	CALL SUBOPT_0x2
;            printf("r datareg -- read; datareg in dec \r\n");
	__POINTW1FN _0x0,360
	CALL SUBOPT_0x2
;            printf("R datareg data -- write; data in hex \r\n");
	__POINTW1FN _0x0,397
	CALL SUBOPT_0x2
;            printf("m addr -- read memory; hex\r\n") ;
	__POINTW1FN _0x0,437
	CALL SUBOPT_0x2
;            printf("M addr dat -- write memory; hex\r\n");
	__POINTW1FN _0x0,466
	CALL SUBOPT_0x2
;            printf("d -- top of data stack; hex\r\n");
	__POINTW1FN _0x0,500
	CALL SUBOPT_0x2
;            printf("w -- top of hardware stack; hex\r\n");
	__POINTW1FN _0x0,530
	CALL SUBOPT_0x2
;            printf("s -- mcu status register; hex\r\n");
	__POINTW1FN _0x0,564
	CALL SUBOPT_0x2
;            #ifdef use_logging
;            printf("l, lc, ld -- logged variables\r\n");
	__POINTW1FN _0x0,596
	CALL SUBOPT_0x2
;            #endif
;            printf("Target code commands:\r\n");
	__POINTW1FN _0x0,628
	CALL SUBOPT_0x2
;            printf("  debug(DebugID) -- enter debugger\r\n");
	__POINTW1FN _0x0,652
	CALL SUBOPT_0x2
;            #ifdef use_rxc_isr
;            printf("  <cntl-c> -- enter debugger with debugID=255\r\n");
;            #endif
;            #ifdef use_reporting
;            printf("  reportVdec, reportVhex -- print variables\r\n");
	__POINTW1FN _0x0,689
	CALL SUBOPT_0x2
;            #endif
;            #ifdef use_logging
;            printf("  loginit, logV -- save variables\r\n");
	__POINTW1FN _0x0,735
	CALL SUBOPT_0x2
;            #endif
;        #else
;            printf("no help \r\n");
;        #endif
;        }
;
;	}  //end while(db_cmd_running)
_0x36:
	JMP  _0x3
_0x5:
;}    //end cmd
	CALL __LOADLOCR4
	ADIW R28,36
	RET
;
;// set up UART and jump to mymain
;void main(void)
;{
_main:
;    //allocate UART structures, turn on UART
;   	//parameter is baud rate given as an integer
;   	//This routine uses the clock value set in the
;   	//Project...Config dialog!!
;
;   	db_init_uart(9600);
	__GETD1N 0x2580
	CALL __PUTPARD1
	CALL _db_init_uart
;   	printf("\r\r\n DB 2.0\r\n");
	__POINTW1FN _0x0,771
	CALL SUBOPT_0x2
;
;	//start out in main program
;   	db_cmd_running = 0;
	LDI  R30,LOW(0)
	STS  _db_cmd_running,R30
;
;    //now jump to user supplied target program
;    mymain() ;
	RCALL _mymain
;
;    //mymain should NEVER return
;    //But if it does...
;    printf("Target Failed") ;
	__POINTW1FN _0x0,784
	CALL SUBOPT_0x2
;    while(1);
_0x37:
	RJMP _0x37
;}
_0x3A:
	RJMP _0x3A
;
;//allows user to insert a program with it's own main routine
;#define main mymain
;
;//end of debugger
;//following stuff is used if debugger is NULLIFIED
;#else
;
;#define debug(id) do{ }while(0)
;#define reportR(regnum)  do{ }while(0)
;#define reportI(regnum)   do{ }while(0)
;#define reportM(addr) do{ }while(0)
;#define reportVhex(varname)  do{ }while(0)
;#define reportVdec(varname)  do{ }while(0)
;#define logInit(varname,logsize)  do{ }while(0)
;#define logV(varname)  do{ }while(0)
;
;#endif
;
;//end of file
;
;
;#include <stdio.h> // Standard Input/Output functions
;#include <delay.h>
;#include <string.h>
;#include <stdlib.h>
;#include <stdint.h>
;
;
;//timeout values for each task
;#define t1 250
;#define t2 125
;#define t3 60
;#define begin {
;#define end }
;
;//the three task subroutines
;void task1(void);      //blink at 2 or 8 Hz
;void task2(void);    //blink at 1 Hz
;void task3(void);    //detect button and modify task 1 rate
;
;void initialize(void); //all the usual mcu stuff
;
;unsigned char time1, time2, time3;    //timeout counters
;unsigned char tsk2c;                //task 2 counter to get to 1/2 second
;unsigned char tsk3m;                //task 3 message to task 1
;unsigned char led;                    //light states
;char msg[20];                        //task 3 serial input
;//**********************************************************
;//timer 0 overflow ISR
;interrupt [TIM0_COMPA] void timer0_compare(void)
; 0000 0024 begin
_timer0_compare:
	ST   -Y,R30
	IN   R30,SREG
	ST   -Y,R30
; 0000 0025   //Decrement the three times if they are not already zero
; 0000 0026   if (time1>0)    --time1;
	LDI  R30,LOW(0)
	CP   R30,R4
	BRSH _0x3B
	DEC  R4
; 0000 0027   if (time2>0)    --time2;
_0x3B:
	LDI  R30,LOW(0)
	CP   R30,R3
	BRSH _0x3C
	DEC  R3
; 0000 0028   if (time3>0)    --time3;
_0x3C:
	LDI  R30,LOW(0)
	CP   R30,R6
	BRSH _0x3D
	DEC  R6
; 0000 0029 end
_0x3D:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R30,Y+
	RETI
;
;//**********************************************************
;//Entry point and task scheduler loop
;
;char tmp=0;
;void main(void)
; 0000 0030 begin
_mymain:
; 0000 0031 
; 0000 0032   initialize();
	RCALL _initialize
; 0000 0033 
; 0000 0034   ///*****
; 0000 0035     ///*****
; 0000 0036     DDRC.5=1;
	SBI  0x7,5
; 0000 0037     DDRC.4=1;
	SBI  0x7,4
; 0000 0038  ///*******
; 0000 0039     PORTC.5=1;
	SBI  0x8,5
; 0000 003A     delay_ms(100);
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	ST   -Y,R31
	ST   -Y,R30
	CALL _delay_ms
; 0000 003B     PORTC.5=0;
	CBI  0x8,5
; 0000 003C ///********
; 0000 003D 
; 0000 003E   //main task scheduler loop
; 0000 003F   logInit(tmp,8);
	LDI  R30,LOW(8)
	STS  _db_logMax,R30
	LDI  R30,LOW(0)
	STS  _db_logcount,R30
	STS  _db_logcount+1,R30
	LDI  R30,LOW(_db_logname)
	LDI  R31,HIGH(_db_logname)
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0x0,798
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0x0,801
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
; 0000 0040   while(1)
_0x49:
; 0000 0041   begin
; 0000 0042    // if (time1==0)    task1();
; 0000 0043    // if (time2==0)     task2();
; 0000 0044   //  if (time3==0)    task3();
; 0000 0045   tmp++;
	INC  R10
; 0000 0046 
; 0000 0047   reportVdec(tmp);
	__POINTW1FN _0x0,801
	CALL SUBOPT_0x2
	__POINTW1FN _0x0,805
	ST   -Y,R31
	ST   -Y,R30
	MOV  R30,R10
	CALL SUBOPT_0x0
	CALL SUBOPT_0x1
; 0000 0048 
; 0000 0049       PORTC.5=1;
	SBI  0x8,5
; 0000 004A     delay_ms(500);
	CALL SUBOPT_0x18
; 0000 004B     PORTC.5=0;
	CBI  0x8,5
; 0000 004C      delay_ms(500);
	CALL SUBOPT_0x18
; 0000 004D   //   debug(6);
; 0000 004E   end
	RJMP _0x49
; 0000 004F end
;
;//**********************************************************
;//Task subroutines
;//Task 1
;void task1(void)
; 0000 0055 begin
; 0000 0056   time1=t1;  //reset the task timer
; 0000 0057   if (tsk3m != 0) time1 >>= 2;  //check for task 3 message
; 0000 0058 
; 0000 0059   //toggle the zeros bit
; 0000 005A   led = led ^ 0x01;
; 0000 005B   PORTB = led;
; 0000 005C 
; 0000 005D 
; 0000 005E   PORTC.5=1;
; 0000 005F end
;
;//*******************************
;//Task 2
;void task2(void)
; 0000 0064 begin
; 0000 0065   time2=t2;  //reset the task timer
; 0000 0066   if (--tsk2c == 0)  //have we waited 1/2 second?
; 0000 0067   begin
; 0000 0068       tsk2c = 4;        //reload the 1/2 sec counter
; 0000 0069      //toggle the ones bit
; 0000 006A       led = led ^ 0x02;
; 0000 006B       PORTB = led;
; 0000 006C 
; 0000 006D        PORTC.5=0;
; 0000 006E   end
; 0000 006F end
;
;//*******************************
;//Task 3
;void task3(void)
; 0000 0074 begin
; 0000 0075   time3=t3;     //reset the task timer
; 0000 0076   tsk3m = ~PINC & 0x01;  //generate the message for task 1
; 0000 0077   if (PINC.1==0)
; 0000 0078   {
; 0000 0079       #asm
; 0000 007A           ldi r16, 0x55
; 0000 007B           mov r0, r16
; 0000 007C       #endasm
; 0000 007D 
; 0000 007E         PORTC.5=1;
; 0000 007F       debug(3);
; 0000 0080   }
; 0000 0081   if (PINC.2==0)
; 0000 0082   { printf("Enter a number:");
; 0000 0083       scanf("%s", msg);
; 0000 0084        printf("\n\r%s\n\r",msg);
; 0000 0085   }
; 0000 0086 
; 0000 0087 end
;
;//**********************************************************
;//Set it all up
;void initialize(void)
; 0000 008C begin
_initialize:
; 0000 008D 
; 0000 008E     //variables to test local variable position on the data stack
; 0000 008F     char tt1, tt2, tt3, tt4, tt5, tt6, tt7, tt8;
; 0000 0090     //first 6 vars are in r16-21
; 0000 0091       tt1=0x55;      //r16
	SBIW R28,2
	CALL __SAVELOCR6
;	tt1 -> R17
;	tt2 -> R16
;	tt3 -> R19
;	tt4 -> R18
;	tt5 -> R21
;	tt6 -> R20
;	tt7 -> Y+7
;	tt8 -> Y+6
	LDI  R17,LOW(85)
; 0000 0092       tt2=0xaa;      //r17
	LDI  R16,LOW(170)
; 0000 0093       tt3=3;      //r16
	LDI  R19,LOW(3)
; 0000 0094       tt4=4;      //r17
	LDI  R18,LOW(4)
; 0000 0095       tt5=5;      //r16
	LDI  R21,LOW(5)
; 0000 0096       tt6=6;      //r17
	LDI  R20,LOW(6)
; 0000 0097       //rest of the vars are on the stack
; 0000 0098       //dstk+7 and dstk+6
; 0000 0099       tt7=0x55;
	LDI  R30,LOW(85)
	STD  Y+7,R30
; 0000 009A       tt8=0xaa;
	LDI  R30,LOW(170)
	STD  Y+6,R30
; 0000 009B       //at this debug point,
; 0000 009C       //issuing the command r16 should return 55
; 0000 009D       debug(1);
	IN   R30,0x24
	STS  _db_t0temp,R30
	LDS  R30,129
	STS  _db_t1temp,R30
	LDS  R30,176
	STS  _db_t2temp,R30
	LDI  R30,LOW(0)
	OUT  0x24,R30
	STS  129,R30
	STS  176,R30
	CALL _saveitall
	LDI  R30,LOW(1)
	STS  _db_break_id,R30
	CALL _cmd
	CALL _loaddatareg
	LDS  R30,_db_t0temp
	OUT  0x24,R30
	LDS  R30,_db_t1temp
	STS  129,R30
	LDS  R30,_db_t2temp
	STS  176,R30
; 0000 009E 
; 0000 009F     //set up the ports
; 0000 00A0   DDRC=0x00;    // PORT C is an input
	LDI  R30,LOW(0)
	OUT  0x7,R30
; 0000 00A1   DDRB=0xff;    // PORT B is an output
	LDI  R30,LOW(255)
	OUT  0x4,R30
; 0000 00A2   PORTB=0;
	LDI  R30,LOW(0)
	OUT  0x5,R30
; 0000 00A3 
; 0000 00A4 
; 0000 00A5   //set up timer 0
; 0000 00A6   TIMSK0=2;        //turn on timer 0 cmp match ISR
	LDI  R30,LOW(2)
	STS  110,R30
; 0000 00A7   OCR0A = 250;      //set the compare re to 250 time ticks
	LDI  R30,LOW(250)
	OUT  0x27,R30
; 0000 00A8       //prescalar to 64 and turn on clear-on-match
; 0000 00A9   TCCR0A=0b00001011;
	LDI  R30,LOW(11)
	OUT  0x24,R30
; 0000 00AA 
; 0000 00AB   //init the LED status (all off)
; 0000 00AC   led=0xff;
	LDI  R30,LOW(255)
	MOV  R7,R30
; 0000 00AD 
; 0000 00AE   //init the task timers
; 0000 00AF   time1=t1;
	LDI  R30,LOW(250)
	MOV  R4,R30
; 0000 00B0   time2=t2;
	LDI  R30,LOW(125)
	MOV  R3,R30
; 0000 00B1   time3=t3;
	LDI  R30,LOW(60)
	MOV  R6,R30
; 0000 00B2 
; 0000 00B3   //init the task 2 state variable
; 0000 00B4   //for four ticks
; 0000 00B5   tsk2c=4;
	LDI  R30,LOW(4)
	MOV  R5,R30
; 0000 00B6 
; 0000 00B7   //init the task 3 message
; 0000 00B8   //for no message
; 0000 00B9   tsk3m=0;
	CLR  R8
; 0000 00BA 
; 0000 00BB   //crank up the ISRs
; 0000 00BC   #asm
; 0000 00BD       sei
      sei
; 0000 00BE   #endasm
; 0000 00BF end
	CALL __LOADLOCR6
	ADIW R28,8
	RET
;
;
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif

	.CSEG
_getchar:
_0x2000003:
	LDS  R30,192
	ANDI R30,LOW(0x80)
	BREQ _0x2000003
	LDS  R30,198
	RET
_putchar:
_0x2000006:
	LDS  R30,192
	ANDI R30,LOW(0x20)
	BREQ _0x2000006
	LD   R30,Y
	STS  198,R30
	ADIW R28,1
	RET
_put_usart_G100:
	LDD  R30,Y+2
	ST   -Y,R30
	RCALL _putchar
	LD   R26,Y
	LDD  R27,Y+1
	CALL SUBOPT_0x19
	ADIW R28,3
	RET
_put_buff_G100:
	ST   -Y,R17
	ST   -Y,R16
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	ADIW R26,2
	CALL __GETW1P
	SBIW R30,0
	BREQ _0x2000016
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	ADIW R26,4
	CALL __GETW1P
	MOVW R16,R30
	SBIW R30,0
	BREQ _0x2000018
	__CPWRN 16,17,2
	BRLO _0x2000019
	MOVW R30,R16
	SBIW R30,1
	MOVW R16,R30
	__PUTW1SNS 2,4
_0x2000018:
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	ADIW R26,2
	CALL SUBOPT_0x19
	SBIW R30,1
	LDD  R26,Y+4
	STD  Z+0,R26
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	CALL __GETW1P
	TST  R31
	BRMI _0x200001A
	CALL SUBOPT_0x19
_0x200001A:
_0x2000019:
	RJMP _0x200001B
_0x2000016:
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	ST   X+,R30
	ST   X,R31
_0x200001B:
	LDD  R17,Y+1
	LDD  R16,Y+0
	RJMP _0x20A0004
__print_G100:
	SBIW R28,6
	CALL __SAVELOCR6
	LDI  R17,0
	LDD  R26,Y+12
	LDD  R27,Y+12+1
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   X+,R30
	ST   X,R31
_0x200001C:
	LDD  R30,Y+18
	LDD  R31,Y+18+1
	ADIW R30,1
	STD  Y+18,R30
	STD  Y+18+1,R31
	SBIW R30,1
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+3
	JMP _0x200001E
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x2000022
	CPI  R18,37
	BRNE _0x2000023
	LDI  R17,LOW(1)
	RJMP _0x2000024
_0x2000023:
	CALL SUBOPT_0x1A
_0x2000024:
	RJMP _0x2000021
_0x2000022:
	CPI  R30,LOW(0x1)
	BRNE _0x2000025
	CPI  R18,37
	BRNE _0x2000026
	CALL SUBOPT_0x1A
	RJMP _0x20000CF
_0x2000026:
	LDI  R17,LOW(2)
	LDI  R20,LOW(0)
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x2000027
	LDI  R16,LOW(1)
	RJMP _0x2000021
_0x2000027:
	CPI  R18,43
	BRNE _0x2000028
	LDI  R20,LOW(43)
	RJMP _0x2000021
_0x2000028:
	CPI  R18,32
	BRNE _0x2000029
	LDI  R20,LOW(32)
	RJMP _0x2000021
_0x2000029:
	RJMP _0x200002A
_0x2000025:
	CPI  R30,LOW(0x2)
	BRNE _0x200002B
_0x200002A:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x200002C
	ORI  R16,LOW(128)
	RJMP _0x2000021
_0x200002C:
	RJMP _0x200002D
_0x200002B:
	CPI  R30,LOW(0x3)
	BREQ PC+3
	JMP _0x2000021
_0x200002D:
	CPI  R18,48
	BRLO _0x2000030
	CPI  R18,58
	BRLO _0x2000031
_0x2000030:
	RJMP _0x200002F
_0x2000031:
	LDI  R26,LOW(10)
	MUL  R21,R26
	MOV  R21,R0
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x2000021
_0x200002F:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0x2000035
	CALL SUBOPT_0x1B
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	LDD  R26,Z+4
	ST   -Y,R26
	CALL SUBOPT_0x1C
	RJMP _0x2000036
_0x2000035:
	CPI  R30,LOW(0x73)
	BRNE _0x2000038
	CALL SUBOPT_0x1B
	CALL SUBOPT_0x1D
	CALL _strlen
	MOV  R17,R30
	RJMP _0x2000039
_0x2000038:
	CPI  R30,LOW(0x70)
	BRNE _0x200003B
	CALL SUBOPT_0x1B
	CALL SUBOPT_0x1D
	CALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0x2000039:
	ORI  R16,LOW(2)
	ANDI R16,LOW(127)
	LDI  R19,LOW(0)
	RJMP _0x200003C
_0x200003B:
	CPI  R30,LOW(0x64)
	BREQ _0x200003F
	CPI  R30,LOW(0x69)
	BRNE _0x2000040
_0x200003F:
	ORI  R16,LOW(4)
	RJMP _0x2000041
_0x2000040:
	CPI  R30,LOW(0x75)
	BRNE _0x2000042
_0x2000041:
	LDI  R30,LOW(_tbl10_G100*2)
	LDI  R31,HIGH(_tbl10_G100*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(5)
	RJMP _0x2000043
_0x2000042:
	CPI  R30,LOW(0x58)
	BRNE _0x2000045
	ORI  R16,LOW(8)
	RJMP _0x2000046
_0x2000045:
	CPI  R30,LOW(0x78)
	BREQ PC+3
	JMP _0x2000077
_0x2000046:
	LDI  R30,LOW(_tbl16_G100*2)
	LDI  R31,HIGH(_tbl16_G100*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(4)
_0x2000043:
	SBRS R16,2
	RJMP _0x2000048
	CALL SUBOPT_0x1B
	CALL SUBOPT_0x1E
	LDD  R26,Y+11
	TST  R26
	BRPL _0x2000049
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	CALL __ANEGW1
	STD  Y+10,R30
	STD  Y+10+1,R31
	LDI  R20,LOW(45)
_0x2000049:
	CPI  R20,0
	BREQ _0x200004A
	SUBI R17,-LOW(1)
	RJMP _0x200004B
_0x200004A:
	ANDI R16,LOW(251)
_0x200004B:
	RJMP _0x200004C
_0x2000048:
	CALL SUBOPT_0x1B
	CALL SUBOPT_0x1E
_0x200004C:
_0x200003C:
	SBRC R16,0
	RJMP _0x200004D
_0x200004E:
	CP   R17,R21
	BRSH _0x2000050
	SBRS R16,7
	RJMP _0x2000051
	SBRS R16,2
	RJMP _0x2000052
	ANDI R16,LOW(251)
	MOV  R18,R20
	SUBI R17,LOW(1)
	RJMP _0x2000053
_0x2000052:
	LDI  R18,LOW(48)
_0x2000053:
	RJMP _0x2000054
_0x2000051:
	LDI  R18,LOW(32)
_0x2000054:
	CALL SUBOPT_0x1A
	SUBI R21,LOW(1)
	RJMP _0x200004E
_0x2000050:
_0x200004D:
	MOV  R19,R17
	SBRS R16,1
	RJMP _0x2000055
_0x2000056:
	CPI  R19,0
	BREQ _0x2000058
	SBRS R16,3
	RJMP _0x2000059
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	LPM  R18,Z+
	STD  Y+6,R30
	STD  Y+6+1,R31
	RJMP _0x200005A
_0x2000059:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R18,X+
	STD  Y+6,R26
	STD  Y+6+1,R27
_0x200005A:
	CALL SUBOPT_0x1A
	CPI  R21,0
	BREQ _0x200005B
	SUBI R21,LOW(1)
_0x200005B:
	SUBI R19,LOW(1)
	RJMP _0x2000056
_0x2000058:
	RJMP _0x200005C
_0x2000055:
_0x200005E:
	LDI  R18,LOW(48)
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	CALL __GETW1PF
	STD  Y+8,R30
	STD  Y+8+1,R31
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	ADIW R30,2
	STD  Y+6,R30
	STD  Y+6+1,R31
_0x2000060:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	CP   R26,R30
	CPC  R27,R31
	BRLO _0x2000062
	SUBI R18,-LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	SUB  R30,R26
	SBC  R31,R27
	STD  Y+10,R30
	STD  Y+10+1,R31
	RJMP _0x2000060
_0x2000062:
	CPI  R18,58
	BRLO _0x2000063
	SBRS R16,3
	RJMP _0x2000064
	SUBI R18,-LOW(7)
	RJMP _0x2000065
_0x2000064:
	SUBI R18,-LOW(39)
_0x2000065:
_0x2000063:
	SBRC R16,4
	RJMP _0x2000067
	CPI  R18,49
	BRSH _0x2000069
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,1
	BRNE _0x2000068
_0x2000069:
	RJMP _0x20000D0
_0x2000068:
	CP   R21,R19
	BRLO _0x200006D
	SBRS R16,0
	RJMP _0x200006E
_0x200006D:
	RJMP _0x200006C
_0x200006E:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0x200006F
	LDI  R18,LOW(48)
_0x20000D0:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x2000070
	ANDI R16,LOW(251)
	ST   -Y,R20
	CALL SUBOPT_0x1C
	CPI  R21,0
	BREQ _0x2000071
	SUBI R21,LOW(1)
_0x2000071:
_0x2000070:
_0x200006F:
_0x2000067:
	CALL SUBOPT_0x1A
	CPI  R21,0
	BREQ _0x2000072
	SUBI R21,LOW(1)
_0x2000072:
_0x200006C:
	SUBI R19,LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,2
	BRLO _0x200005F
	RJMP _0x200005E
_0x200005F:
_0x200005C:
	SBRS R16,0
	RJMP _0x2000073
_0x2000074:
	CPI  R21,0
	BREQ _0x2000076
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	ST   -Y,R30
	CALL SUBOPT_0x1C
	RJMP _0x2000074
_0x2000076:
_0x2000073:
_0x2000077:
_0x2000036:
_0x20000CF:
	LDI  R17,LOW(0)
_0x2000021:
	RJMP _0x200001C
_0x200001E:
	LDD  R26,Y+12
	LDD  R27,Y+12+1
	CALL __GETW1P
	CALL __LOADLOCR6
	ADIW R28,20
	RET
_sprintf:
	PUSH R15
	MOV  R15,R24
	SBIW R28,6
	CALL __SAVELOCR4
	CALL SUBOPT_0x1F
	SBIW R30,0
	BRNE _0x2000078
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0005
_0x2000078:
	MOVW R26,R28
	ADIW R26,6
	CALL __ADDW2R15
	MOVW R16,R26
	CALL SUBOPT_0x1F
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R30,LOW(0)
	STD  Y+8,R30
	STD  Y+8+1,R30
	MOVW R26,R28
	ADIW R26,10
	CALL SUBOPT_0x20
	LDI  R30,LOW(_put_buff_G100)
	LDI  R31,HIGH(_put_buff_G100)
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,10
	ST   -Y,R31
	ST   -Y,R30
	RCALL __print_G100
	MOVW R18,R30
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
	MOVW R30,R18
_0x20A0005:
	CALL __LOADLOCR4
	ADIW R28,10
	POP  R15
	RET
_printf:
	PUSH R15
	MOV  R15,R24
	SBIW R28,6
	ST   -Y,R17
	ST   -Y,R16
	MOVW R26,R28
	ADIW R26,4
	CALL __ADDW2R15
	MOVW R16,R26
	LDI  R30,LOW(0)
	STD  Y+4,R30
	STD  Y+4+1,R30
	STD  Y+6,R30
	STD  Y+6+1,R30
	MOVW R26,R28
	ADIW R26,8
	CALL SUBOPT_0x20
	LDI  R30,LOW(_put_usart_G100)
	LDI  R31,HIGH(_put_usart_G100)
	CALL SUBOPT_0x21
	RCALL __print_G100
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,8
	POP  R15
	RET
_get_buff_G100:
	ST   -Y,R17
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LDI  R30,LOW(0)
	ST   X,R30
	LDD  R26,Y+3
	LDD  R27,Y+3+1
	LD   R30,X
	MOV  R17,R30
	CPI  R30,0
	BREQ _0x2000080
	LDI  R30,LOW(0)
	ST   X,R30
	RJMP _0x2000081
_0x2000080:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	ADIW R26,1
	CALL __GETW1P
	SBIW R30,0
	BREQ _0x2000082
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	LDD  R26,Z+1
	LDD  R27,Z+2
	LD   R30,X
	MOV  R17,R30
	CPI  R30,0
	BREQ _0x2000083
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	ADIW R26,1
	CALL SUBOPT_0x19
_0x2000083:
	RJMP _0x2000084
_0x2000082:
	LDI  R17,LOW(0)
_0x2000084:
_0x2000081:
	MOV  R30,R17
	LDD  R17,Y+0
_0x20A0004:
	ADIW R28,5
	RET
__scanf_G100:
	SBIW R28,5
	CALL __SAVELOCR6
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	STD  Y+8,R30
	STD  Y+8+1,R31
	MOV  R20,R30
_0x2000085:
	LDD  R30,Y+17
	LDD  R31,Y+17+1
	ADIW R30,1
	STD  Y+17,R30
	STD  Y+17+1,R31
	SBIW R30,1
	LPM  R30,Z
	MOV  R19,R30
	CPI  R30,0
	BRNE PC+3
	JMP _0x2000087
	CALL SUBOPT_0x22
	BREQ _0x2000088
_0x2000089:
	IN   R30,SPL
	IN   R31,SPH
	ST   -Y,R31
	ST   -Y,R30
	PUSH R20
	CALL SUBOPT_0x1C
	POP  R20
	MOV  R19,R30
	CPI  R30,0
	BREQ _0x200008C
	CALL SUBOPT_0x22
	BRNE _0x200008D
_0x200008C:
	RJMP _0x200008B
_0x200008D:
	CALL SUBOPT_0x23
	BRGE _0x200008E
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0002
_0x200008E:
	RJMP _0x2000089
_0x200008B:
	MOV  R20,R19
	RJMP _0x200008F
_0x2000088:
	CPI  R19,37
	BREQ PC+3
	JMP _0x2000090
	LDI  R21,LOW(0)
_0x2000091:
	LDD  R30,Y+17
	LDD  R31,Y+17+1
	LPM  R19,Z+
	STD  Y+17,R30
	STD  Y+17+1,R31
	CPI  R19,48
	BRLO _0x2000095
	CPI  R19,58
	BRLO _0x2000094
_0x2000095:
	RJMP _0x2000093
_0x2000094:
	LDI  R26,LOW(10)
	MUL  R21,R26
	MOV  R21,R0
	MOV  R30,R19
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x2000091
_0x2000093:
	CPI  R19,0
	BRNE _0x2000097
	RJMP _0x2000087
_0x2000097:
_0x2000098:
	IN   R30,SPL
	IN   R31,SPH
	ST   -Y,R31
	ST   -Y,R30
	PUSH R20
	CALL SUBOPT_0x1C
	POP  R20
	MOV  R18,R30
	ST   -Y,R30
	CALL _isspace
	CPI  R30,0
	BREQ _0x200009A
	CALL SUBOPT_0x23
	BRGE _0x200009B
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0002
_0x200009B:
	RJMP _0x2000098
_0x200009A:
	CPI  R18,0
	BRNE _0x200009C
	RJMP _0x200009D
_0x200009C:
	MOV  R20,R18
	CPI  R21,0
	BRNE _0x200009E
	LDI  R21,LOW(255)
_0x200009E:
	MOV  R30,R19
	CPI  R30,LOW(0x63)
	BRNE _0x20000A2
	CALL SUBOPT_0x24
	IN   R30,SPL
	IN   R31,SPH
	ST   -Y,R31
	ST   -Y,R30
	PUSH R20
	CALL SUBOPT_0x1C
	POP  R20
	MOVW R26,R16
	ST   X,R30
	CALL SUBOPT_0x23
	BRGE _0x20000A3
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0002
_0x20000A3:
	RJMP _0x20000A1
_0x20000A2:
	CPI  R30,LOW(0x73)
	BRNE _0x20000AC
	CALL SUBOPT_0x24
_0x20000A5:
	MOV  R30,R21
	SUBI R21,1
	CPI  R30,0
	BREQ _0x20000A7
	IN   R30,SPL
	IN   R31,SPH
	ST   -Y,R31
	ST   -Y,R30
	PUSH R20
	CALL SUBOPT_0x1C
	POP  R20
	MOV  R19,R30
	CPI  R30,0
	BREQ _0x20000A9
	CALL SUBOPT_0x22
	BREQ _0x20000A8
_0x20000A9:
	CALL SUBOPT_0x23
	BRGE _0x20000AB
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0002
_0x20000AB:
	RJMP _0x20000A7
_0x20000A8:
	PUSH R17
	PUSH R16
	__ADDWRN 16,17,1
	MOV  R30,R19
	POP  R26
	POP  R27
	ST   X,R30
	RJMP _0x20000A5
_0x20000A7:
	MOVW R26,R16
	LDI  R30,LOW(0)
	ST   X,R30
	RJMP _0x20000A1
_0x20000AC:
	LDI  R30,LOW(1)
	STD  Y+10,R30
	MOV  R30,R19
	CPI  R30,LOW(0x64)
	BREQ _0x20000B1
	CPI  R30,LOW(0x69)
	BRNE _0x20000B2
_0x20000B1:
	LDI  R30,LOW(0)
	STD  Y+10,R30
	RJMP _0x20000B3
_0x20000B2:
	CPI  R30,LOW(0x75)
	BRNE _0x20000B4
_0x20000B3:
	LDI  R18,LOW(10)
	RJMP _0x20000AF
_0x20000B4:
	CPI  R30,LOW(0x78)
	BRNE _0x20000B5
	LDI  R18,LOW(16)
	RJMP _0x20000AF
_0x20000B5:
	CPI  R30,LOW(0x25)
	BRNE _0x20000B8
	RJMP _0x20000B7
_0x20000B8:
	RJMP _0x20A0003
_0x20000AF:
	LDI  R30,LOW(0)
	STD  Y+6,R30
	STD  Y+6+1,R30
_0x20000B9:
	MOV  R30,R21
	SUBI R21,1
	CPI  R30,0
	BRNE PC+3
	JMP _0x20000BB
	IN   R30,SPL
	IN   R31,SPH
	ST   -Y,R31
	ST   -Y,R30
	PUSH R20
	CALL SUBOPT_0x1C
	POP  R20
	MOV  R19,R30
	CPI  R30,LOW(0x21)
	BRSH _0x20000BC
	CALL SUBOPT_0x23
	BRGE _0x20000BD
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0002
_0x20000BD:
	RJMP _0x20000BE
_0x20000BC:
	LDD  R30,Y+10
	CPI  R30,0
	BRNE _0x20000BF
	CPI  R19,45
	BRNE _0x20000C0
	LDI  R30,LOW(255)
	STD  Y+10,R30
	RJMP _0x20000B9
_0x20000C0:
	LDI  R30,LOW(1)
	STD  Y+10,R30
_0x20000BF:
	CPI  R18,16
	BRNE _0x20000C2
	ST   -Y,R19
	CALL _isxdigit
	CPI  R30,0
	BREQ _0x20000BE
	RJMP _0x20000C4
_0x20000C2:
	ST   -Y,R19
	CALL _isdigit
	CPI  R30,0
	BRNE _0x20000C5
_0x20000BE:
	MOV  R20,R19
	RJMP _0x20000BB
_0x20000C5:
_0x20000C4:
	CPI  R19,97
	BRLO _0x20000C6
	SUBI R19,LOW(87)
	RJMP _0x20000C7
_0x20000C6:
	CPI  R19,65
	BRLO _0x20000C8
	SUBI R19,LOW(55)
	RJMP _0x20000C9
_0x20000C8:
	SUBI R19,LOW(48)
_0x20000C9:
_0x20000C7:
	MOV  R30,R18
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R31,0
	CALL __MULW12U
	MOVW R26,R30
	MOV  R30,R19
	LDI  R31,0
	ADD  R30,R26
	ADC  R31,R27
	STD  Y+6,R30
	STD  Y+6+1,R31
	RJMP _0x20000B9
_0x20000BB:
	CALL SUBOPT_0x24
	LDD  R30,Y+10
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R31,0
	SBRC R30,7
	SER  R31
	CALL __MULW12U
	MOVW R26,R16
	ST   X+,R30
	ST   X,R31
_0x20000A1:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	ADIW R30,1
	STD  Y+8,R30
	STD  Y+8+1,R31
	RJMP _0x20000CA
_0x2000090:
_0x20000B7:
	IN   R30,SPL
	IN   R31,SPH
	ST   -Y,R31
	ST   -Y,R30
	PUSH R20
	CALL SUBOPT_0x1C
	POP  R20
	CP   R30,R19
	BREQ _0x20000CB
	CALL SUBOPT_0x23
	BRGE _0x20000CC
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0002
_0x20000CC:
_0x200009D:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	SBIW R30,0
	BRNE _0x20000CD
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0002
_0x20000CD:
	RJMP _0x2000087
_0x20000CB:
_0x20000CA:
_0x200008F:
	RJMP _0x2000085
_0x2000087:
_0x20A0003:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
_0x20A0002:
	CALL __LOADLOCR6
	ADIW R28,19
	RET
_sscanf:
	PUSH R15
	MOV  R15,R24
	SBIW R28,3
	ST   -Y,R17
	ST   -Y,R16
	CALL SUBOPT_0x25
	SBIW R30,0
	BRNE _0x20000CE
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	RJMP _0x20A0001
_0x20000CE:
	MOVW R26,R28
	ADIW R26,1
	CALL __ADDW2R15
	MOVW R16,R26
	CALL SUBOPT_0x25
	STD  Y+3,R30
	STD  Y+3+1,R31
	MOVW R26,R28
	ADIW R26,5
	CALL SUBOPT_0x20
	LDI  R30,LOW(_get_buff_G100)
	LDI  R31,HIGH(_get_buff_G100)
	CALL SUBOPT_0x21
	RCALL __scanf_G100
_0x20A0001:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,5
	POP  R15
	RET

	.CSEG
_strlen:
    ld   r26,y+
    ld   r27,y+
    clr  r30
    clr  r31
strlen0:
    ld   r22,x+
    tst  r22
    breq strlen1
    adiw r30,1
    rjmp strlen0
strlen1:
    ret
_strlenf:
    clr  r26
    clr  r27
    ld   r30,y+
    ld   r31,y+
strlenf0:
	lpm  r0,z+
    tst  r0
    breq strlenf1
    adiw r26,1
    rjmp strlenf0
strlenf1:
    movw r30,r26
    ret

	.CSEG

	.DSEG

	.CSEG

	.CSEG
_isdigit:
    ldi  r30,1
    ld   r31,y+
    cpi  r31,'0'
    brlo isdigit0
    cpi  r31,'9'+1
    brlo isdigit1
isdigit0:
    clr  r30
isdigit1:
    ret
_isspace:
    ldi  r30,1
    ld   r31,y+
    cpi  r31,' '
    breq isspace1
    cpi  r31,9
    brlo isspace0
    cpi  r31,13+1
    brlo isspace1
isspace0:
    clr  r30
isspace1:
    ret
_isxdigit:
    ldi  r30,1
    ld   r31,y+
    subi r31,0x30
    brcs isxdigit0
    cpi  r31,10
    brcs isxdigit1
    andi r31,0x5f
    subi r31,7
    cpi  r31,10
    brcs isxdigit0
    cpi  r31,16
    brcs isxdigit1
isxdigit0:
    clr  r30
isxdigit1:
    ret

	.CSEG

	.DSEG
_db_break_id:
	.BYTE 0x1
_db_t0temp:
	.BYTE 0x1
_db_t1temp:
	.BYTE 0x1
_db_t2temp:
	.BYTE 0x1
_db_swstk:
	.BYTE 0x2
_db_hwstk:
	.BYTE 0x2
_db_regs:
	.BYTE 0x20
_db_sreg:
	.BYTE 0x1
_db_cmd_running:
	.BYTE 0x1
_db_logcount:
	.BYTE 0x2
_db_logMax:
	.BYTE 0x1
_db_logname:
	.BYTE 0x10
_db_temp:
	.BYTE 0x1
_db_temp1:
	.BYTE 0x1
_db_temp2:
	.BYTE 0x1
_db_temp3:
	.BYTE 0x2
_db_temp4:
	.BYTE 0x2
_msg:
	.BYTE 0x14
__seed_G102:
	.BYTE 0x4

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x0:
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x1:
	LDI  R24,4
	CALL _printf
	ADIW R28,6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 21 TIMES, CODE SIZE REDUCTION:77 WORDS
SUBOPT_0x2:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R24,0
	CALL _printf
	ADIW R28,2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x3:
	MOVW R30,R28
	ADIW R30,4
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x4:
	__POINTW1FN _0x0,15
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(_db_temp)
	LDI  R31,HIGH(_db_temp)
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5:
	LDI  R30,LOW(_db_temp1)
	LDI  R31,HIGH(_db_temp1)
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x6:
	LDI  R24,8
	CALL _sscanf
	ADIW R28,12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x7:
	ST   -Y,R31
	ST   -Y,R30
	LDS  R30,_db_temp1
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x8:
	LDS  R30,_db_temp
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x9:
	LDI  R24,8
	CALL _printf
	ADIW R28,10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xA:
	__POINTW1FN _0x0,36
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(_db_temp)
	LDI  R31,HIGH(_db_temp)
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xB:
	LDI  R30,LOW(_db_temp2)
	LDI  R31,HIGH(_db_temp2)
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC:
	LDI  R24,12
	CALL _sscanf
	ADIW R28,16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD:
	LDS  R30,_db_temp2
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xE:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(_db_temp)
	LDI  R31,HIGH(_db_temp)
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RJMP SUBOPT_0xB

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xF:
	LDS  R30,_db_temp2
	LDI  R31,0
	SUBI R30,LOW(-_db_regs)
	SBCI R31,HIGH(-_db_regs)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x10:
	LDI  R30,LOW(_db_temp4)
	LDI  R31,HIGH(_db_temp4)
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RJMP SUBOPT_0xC

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x11:
	LDS  R30,_db_temp4
	LDS  R31,_db_temp4+1
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RJMP SUBOPT_0x9

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x12:
	LDI  R30,LOW(_db_temp3)
	LDI  R31,HIGH(_db_temp3)
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x13:
	ST   -Y,R31
	ST   -Y,R30
	LDS  R30,_db_temp3
	LDS  R31,_db_temp3+1
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x14:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RJMP SUBOPT_0x1

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x15:
	__POINTW1FN _0x0,212
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(_db_logname)
	LDI  R31,HIGH(_db_logname)
	RJMP SUBOPT_0x14

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x16:
	LDS  R30,_db_logcount
	LDS  R31,_db_logcount+1
	CP   R18,R30
	CPC  R19,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x17:
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R18
	SUBI R30,LOW(-96)
	SBCI R31,HIGH(-96)
	LD   R30,Z
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x18:
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x19:
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x1A:
	ST   -Y,R18
	LDD  R30,Y+13
	LDD  R31,Y+13+1
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+17
	LDD  R31,Y+17+1
	ICALL
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x1B:
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,4
	STD  Y+16,R30
	STD  Y+16+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:37 WORDS
SUBOPT_0x1C:
	LDD  R30,Y+13
	LDD  R31,Y+13+1
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+17
	LDD  R31,Y+17+1
	ICALL
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x1D:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	ADIW R26,4
	CALL __GETW1P
	STD  Y+6,R30
	STD  Y+6+1,R31
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x1E:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	ADIW R26,4
	CALL __GETW1P
	STD  Y+10,R30
	STD  Y+10+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1F:
	MOVW R26,R28
	ADIW R26,12
	CALL __ADDW2R15
	CALL __GETW1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x20:
	CALL __ADDW2R15
	CALL __GETW1P
	ST   -Y,R31
	ST   -Y,R30
	ST   -Y,R17
	ST   -Y,R16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x21:
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,8
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x22:
	ST   -Y,R19
	CALL _isspace
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x23:
	LDD  R26,Y+11
	LDD  R27,Y+11+1
	LD   R26,X
	CPI  R26,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x24:
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	SBIW R30,4
	STD  Y+15,R30
	STD  Y+15+1,R31
	LDD  R26,Y+15
	LDD  R27,Y+15+1
	ADIW R26,4
	LD   R16,X+
	LD   R17,X
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x25:
	MOVW R26,R28
	ADIW R26,7
	CALL __ADDW2R15
	CALL __GETW1P
	RET


	.CSEG
_delay_ms:
	ld   r30,y+
	ld   r31,y+
	adiw r30,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0x7D0
	wdr
	sbiw r30,1
	brne __delay_ms0
__delay_ms1:
	ret

__ADDW2R15:
	CLR  R0
	ADD  R26,R15
	ADC  R27,R0
	RET

__ANEGW1:
	NEG  R31
	NEG  R30
	SBCI R31,0
	RET

__ANEGD1:
	COM  R31
	COM  R22
	COM  R23
	NEG  R30
	SBCI R31,-1
	SBCI R22,-1
	SBCI R23,-1
	RET

__EQB12:
	CP   R30,R26
	LDI  R30,1
	BREQ __EQB12T
	CLR  R30
__EQB12T:
	RET

__GTB12U:
	CP   R30,R26
	LDI  R30,1
	BRLO __GTB12U1
	CLR  R30
__GTB12U1:
	RET

__LNEGB1:
	TST  R30
	LDI  R30,1
	BREQ __LNEGB1F
	CLR  R30
__LNEGB1F:
	RET

__MULW12U:
	MUL  R31,R26
	MOV  R31,R0
	MUL  R30,R27
	ADD  R31,R0
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	RET

__MULD12U:
	MUL  R23,R26
	MOV  R23,R0
	MUL  R22,R27
	ADD  R23,R0
	MUL  R31,R24
	ADD  R23,R0
	MUL  R30,R25
	ADD  R23,R0
	MUL  R22,R26
	MOV  R22,R0
	ADD  R23,R1
	MUL  R31,R27
	ADD  R22,R0
	ADC  R23,R1
	MUL  R30,R24
	ADD  R22,R0
	ADC  R23,R1
	CLR  R24
	MUL  R31,R26
	MOV  R31,R0
	ADD  R22,R1
	ADC  R23,R24
	MUL  R30,R27
	ADD  R31,R0
	ADC  R22,R1
	ADC  R23,R24
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	ADC  R22,R24
	ADC  R23,R24
	RET

__MULD12:
	RCALL __CHKSIGND
	RCALL __MULD12U
	BRTC __MULD121
	RCALL __ANEGD1
__MULD121:
	RET

__DIVD21U:
	PUSH R19
	PUSH R20
	PUSH R21
	CLR  R0
	CLR  R1
	CLR  R20
	CLR  R21
	LDI  R19,32
__DIVD21U1:
	LSL  R26
	ROL  R27
	ROL  R24
	ROL  R25
	ROL  R0
	ROL  R1
	ROL  R20
	ROL  R21
	SUB  R0,R30
	SBC  R1,R31
	SBC  R20,R22
	SBC  R21,R23
	BRCC __DIVD21U2
	ADD  R0,R30
	ADC  R1,R31
	ADC  R20,R22
	ADC  R21,R23
	RJMP __DIVD21U3
__DIVD21U2:
	SBR  R26,1
__DIVD21U3:
	DEC  R19
	BRNE __DIVD21U1
	MOVW R30,R26
	MOVW R22,R24
	MOVW R26,R0
	MOVW R24,R20
	POP  R21
	POP  R20
	POP  R19
	RET

__DIVD21:
	RCALL __CHKSIGND
	RCALL __DIVD21U
	BRTC __DIVD211
	RCALL __ANEGD1
__DIVD211:
	RET

__CHKSIGND:
	CLT
	SBRS R23,7
	RJMP __CHKSD1
	RCALL __ANEGD1
	SET
__CHKSD1:
	SBRS R25,7
	RJMP __CHKSD2
	CLR  R0
	COM  R26
	COM  R27
	COM  R24
	COM  R25
	ADIW R26,1
	ADC  R24,R0
	ADC  R25,R0
	BLD  R0,0
	INC  R0
	BST  R0,0
__CHKSD2:
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__GETW1PF:
	LPM  R0,Z+
	LPM  R31,Z
	MOV  R30,R0
	RET

__GETD1S0:
	LD   R30,Y
	LDD  R31,Y+1
	LDD  R22,Y+2
	LDD  R23,Y+3
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__CPD10:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	RET

__SAVELOCR6:
	ST   -Y,R21
__SAVELOCR5:
	ST   -Y,R20
__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR6:
	LDD  R21,Y+5
__LOADLOCR5:
	LDD  R20,Y+4
__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
