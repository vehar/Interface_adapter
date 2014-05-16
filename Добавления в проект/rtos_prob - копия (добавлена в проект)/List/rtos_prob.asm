
;CodeVisionAVR C Compiler V2.05.0 Professional
;(C) Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type                : ATmega128
;Program type             : Application
;Clock frequency          : 8,000000 MHz
;Memory model             : Small
;Optimize for             : Size
;(s)printf features       : int, width
;(s)scanf features        : int, width
;External RAM size        : 0
;Data Stack size          : 1024 byte(s)
;Heap size                : 0 byte(s)
;Promote 'char' to 'int'  : No
;'char' is unsigned       : Yes
;8 bit enums              : Yes
;global 'const' stored in FLASH: Yes
;Enhanced core instructions    : On
;Smart register allocation     : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATmega128
	#pragma AVRPART MEMORY PROG_FLASH 131072
	#pragma AVRPART MEMORY EEPROM 4096
	#pragma AVRPART MEMORY INT_SRAM SIZE 4351
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x100

	#define CALL_SUPPORTED 1

	.LISTMAC
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU RAMPZ=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F
	.EQU XMCRA=0x6D
	.EQU XMCRB=0x6C

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
	.EQU __SRAM_END=0x10FF
	.EQU __DSTACK_SIZE=0x0400
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
	JMP  _timer2_comp_isr
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

_tbl10_G102:
	.DB  0x10,0x27,0xE8,0x3,0x64,0x0,0xA,0x0
	.DB  0x1,0x0
_tbl16_G102:
	.DB  0x0,0x10,0x0,0x1,0x10,0x0,0x1,0x0

_0x2020060:
	.DB  0x1
_0x2020000:
	.DB  0x2D,0x4E,0x41,0x4E,0x0,0x49,0x4E,0x46
	.DB  0x0

__GLOBAL_INI_TBL:
	.DW  0x01
	.DW  __seed_G101
	.DW  _0x2020060*2

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
	STS  XMCRB,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

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

	OUT  RAMPZ,R24

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
	.ORG 0x500

	.CSEG
;
;#include <mega128.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x20
	.EQU __sm_mask=0x1C
	.EQU __sm_powerdown=0x10
	.EQU __sm_powersave=0x18
	.EQU __sm_standby=0x14
	.EQU __sm_ext_standby=0x1C
	.EQU __sm_adc_noise_red=0x08
	.SET power_ctrl_reg=mcucr
	#endif
;#include <delay.h>
;#include <string.h>
;#include <stdlib.h>
;#include <stdio.h>
;#include <stdint.h>
;#include <io.h>
;
;#include "RTOS/HAL.h"
;#include "RTOS/EERTOS.h"
;#include "RTOS/EERTOSHAL.h"
;
;//RTOS Interrupt
;// Timer2 overflow interrupt service routine
;interrupt [RTOS_ISR] void timer2_comp_isr(void)
; 0000 0011 {

	.CSEG
_timer2_comp_isr:
	ST   -Y,R0
	ST   -Y,R1
	ST   -Y,R15
	ST   -Y,R22
	ST   -Y,R23
	ST   -Y,R24
	ST   -Y,R25
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 0012  //DDRB.4^=1;
; 0000 0013  TimerService();
	RCALL _TimerService
; 0000 0014 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	LD   R25,Y+
	LD   R24,Y+
	LD   R23,Y+
	LD   R22,Y+
	LD   R15,Y+
	LD   R1,Y+
	LD   R0,Y+
	RETI
;
;// Прототипы задач ============================================================
;void Task1 (void);
;void Task2 (void);
;void Task3 (void);
;void Task4 (void);
;
;void Task5 (void);
;void Task6 (void);
;//============================================================================
;//Область задач
;//============================================================================
;
;void Task1 (void)
; 0000 0023 {
_Task1:
; 0000 0024 SetTimerTask(Task2,500);
	LDI  R30,LOW(_Task2)
	LDI  R31,HIGH(_Task2)
	CALL SUBOPT_0x0
; 0000 0025 LED_PORT  |=1<<LED1;
	SBI  0x12,6
; 0000 0026 }
	RET
;
;void Task2 (void)
; 0000 0029 {
_Task2:
; 0000 002A SetTimerTask(Task1,500);
	LDI  R30,LOW(_Task1)
	LDI  R31,HIGH(_Task1)
	CALL SUBOPT_0x0
; 0000 002B LED_PORT  &= ~(1<<LED1);
	CBI  0x12,6
; 0000 002C }
	RET
;
;void Task3 (void)
; 0000 002F {
_Task3:
; 0000 0030 SetTimerTask(Task4,1000);
	LDI  R30,LOW(_Task4)
	LDI  R31,HIGH(_Task4)
	CALL SUBOPT_0x1
; 0000 0031 LED_PORT  |= (1<<LED2);
	SBI  0x12,7
; 0000 0032 }
	RET
;void Task4 (void)
; 0000 0034 {
_Task4:
; 0000 0035 SetTimerTask(Task3,1000);
	LDI  R30,LOW(_Task3)
	LDI  R31,HIGH(_Task3)
	CALL SUBOPT_0x1
; 0000 0036 LED_PORT  &=~ (1<<LED2);
	CBI  0x12,7
; 0000 0037 }
	RET
;
;
;
;
;
;// Declare your global variables here
;
;void main(void)
; 0000 0040 {
_main:
; 0000 0041 InitAll();			// Инициализируем периферию
	CALL _InitAll
; 0000 0042 InitRTOS();			// Инициализируем ядро
	RCALL _InitRTOS
; 0000 0043 RunRTOS();			// Старт ядра.
	CALL _RunRTOS
; 0000 0044 
; 0000 0045 // Запуск фоновых задач.
; 0000 0046 SetTask(Task1);     //290uS (50/50) and (10/10)   но при 1/1 таск 1 лагает
	LDI  R30,LOW(_Task1)
	LDI  R31,HIGH(_Task1)
	ST   -Y,R31
	ST   -Y,R30
	RCALL _SetTask
; 0000 0047 SetTask(Task3);    //290us
	LDI  R30,LOW(_Task3)
	LDI  R31,HIGH(_Task3)
	ST   -Y,R31
	ST   -Y,R30
	RCALL _SetTask
; 0000 0048 
; 0000 0049   //SetTask(Task5);    //при добавлении задачи 285us высок ур 11мс, а низк - 2.5(!)
; 0000 004A 
; 0000 004B while(1) 		// Главный цикл диспетчера
_0x3:
; 0000 004C {
; 0000 004D //wdt_reset();	// Сброс собачьего таймера
; 0000 004E TaskManager();	// Вызов диспетчера
	RCALL _TaskManager
; 0000 004F }
	RJMP _0x3
; 0000 0050 }
_0x6:
	RJMP _0x6
;//290uS (50/50) and (10/10)   но при 1/1 таск 1 лагает:
;// (1 мс низк 10.1 высок волнистый и инверсия фал таск 1 и 2)
;//при кол-ве задач  6 время задержки выполнения каждой по прежнему ~285uS
;
;#include "RTOS/EERTOS.h"
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x20
	.EQU __sm_mask=0x1C
	.EQU __sm_powerdown=0x10
	.EQU __sm_powersave=0x18
	.EQU __sm_standby=0x14
	.EQU __sm_ext_standby=0x1C
	.EQU __sm_adc_noise_red=0x08
	.SET power_ctrl_reg=mcucr
	#endif
;#include "RTOS/EERTOSHAL.h"
;
;
;/*
;Насколько хорошо это будет работать, если таймер установить на 50-100мкс при 16МГц?
;
;DI HALT:
;9 Апрель 2012 в 21:08
;Плохо будет работать. Там довольно многое можно оптимизировать,
;среднее выполнение блока таймерной службы сейчас около 600 тактов.
;Ну или около того. Столько же в среднем занимает перебор очереди,
;установка таймера порядка 700 тактов, установка задачи около 400 тактов.
;Реально тайминг снизить до 500мкс ,но делать очень коротки задачи, иначе таймер лажать будет.
;*/
;
;
;/*
;  UPDATE - Избавился от очереди задач TaskQueue, вместо этого в диспетчере задач
;  выполняются задачи из очереди MainTimer, которые уже "выщелкали",
;  соответственно количество кода очень уменьшилось.
;  MEMORY -58 WORDS
;*/
;
;
;
;// Очереди задач, таймеров.
;// Тип данных - указатель на функцию
;//volatile static TPTR	TaskQueue[TaskQueueSize+1];			// очередь указателей
;//update
;volatile static struct
;						{
;						    TPTR GoToTask; 						// Указатель перехода
;						    uint16_t Time;					// Выдержка в мс
;						}
;						MainTimer[MainTimerQueueSize+1];	// Очередь таймеров
;
;
;// RTOS Подготовка. Очистка очередей
;  void InitRTOS(void)
; 0001 0029 {

	.CSEG
_InitRTOS:
; 0001 002A uint8_t	index;
; 0001 002B 
; 0001 002C /*   //UPDATE
; 0001 002D     for(index=0;index!=TaskQueueSize+1;index++)	// Во все позиции записываем Idle
; 0001 002E 	    {
; 0001 002F 	    TaskQueue[index] = Idle;
; 0001 0030 	 }
; 0001 0031 */
; 0001 0032       for(index=0;index!=MainTimerQueueSize+1;index++) // Обнуляем все таймеры.
	ST   -Y,R17
;	index -> R17
	LDI  R17,LOW(0)
_0x20004:
	CPI  R17,16
	BREQ _0x20005
; 0001 0033     	{
; 0001 0034 	    MainTimer[index].GoToTask = Idle;
	CALL SUBOPT_0x2
	CALL SUBOPT_0x3
; 0001 0035 	    MainTimer[index].Time = 0;
	CALL SUBOPT_0x2
	ADD  R26,R30
	ADC  R27,R31
	ADIW R26,2
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   X+,R30
	ST   X,R31
; 0001 0036 	 }
	SUBI R17,-1
	RJMP _0x20004
_0x20005:
; 0001 0037 }
	RJMP _0x20A0001
;
;
;//Пустая процедура - простой ядра.
;  void  Idle(void)
; 0001 003C {
_Idle:
; 0001 003D 
; 0001 003E }
	RET
;
;
;
; //UPDATE
; void SetTask(TPTR TS){  // Поставить задачу в очередь для немедленного выполнения
; 0001 0043 void SetTask(TPTR TS){
_SetTask:
; 0001 0044  SetTimerTask(TS,0);
;	*TS -> Y+0
	LD   R30,Y
	LDD  R31,Y+1
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   -Y,R31
	ST   -Y,R30
	RCALL _SetTimerTask
; 0001 0045 }
	ADIW R28,2
	RET
;
;/*  //UPDATE
;// Функция установки задачи в очередь. Передаваемый параметр - указатель на функцию
;// Отдаваемое значение - код ошибки.
;void SetTask(TPTR TS)      //1 task ~8words
;{
;char		index = 0;
;char		nointerrupted = 0;
;
;if (STATUS_REG & (1<<Interrupt_Flag))  // Если прерывания разрешены, то запрещаем их.
;	{
;	_disable_interrupts()
;	nointerrupted = 1;					// И ставим флаг, что мы не в прерывании.
;	}
;
;while(TaskQueue[index]!=Idle) 			// Прочесываем очередь задач на предмет свободной ячейки
;	{									// с значением Idle - конец очереди.
;	index++;
;	if (index==TaskQueueSize+1) 		// Если очередь переполнена то выходим не солоно хлебавши
;		{
;		if (nointerrupted)	_enable_interrupts() 	// Если мы не в прерывании, то разрешаем прерывания
;		return;									// Раньше функция возвращала код ошибки - очередь переполнена. Пока убрал.
;		}
;	}
;												// Если нашли свободное место, то
;TaskQueue[index] = TS;							// Записываем в очередь задачу
;if (nointerrupted) _enable_interrupts()				// И включаем прерывания если не в обработчике прерывания.
;}
;*/
;
;//Функция установки задачи по таймеру. Передаваемые параметры - указатель на функцию,
;// Время выдержки в тиках системного таймера. Возвращет код ошибки.
;
;void SetTimerTask(TPTR TS, unsigned int NewTime)    //1 task ~12words
; 0001 0068 {
_SetTimerTask:
; 0001 0069 uint8_t		index=0;
; 0001 006A uint8_t		nointerrupted = 0;
; 0001 006B 
; 0001 006C if (STATUS_REG & (1<<Interrupt_Flag)) 			// Проверка запрета прерывания, аналогично функции выше
	ST   -Y,R17
	ST   -Y,R16
;	*TS -> Y+4
;	NewTime -> Y+2
;	index -> R17
;	nointerrupted -> R16
	LDI  R17,0
	LDI  R16,0
	IN   R30,0x3F
	SBRS R30,7
	RJMP _0x20006
; 0001 006D 	{
; 0001 006E 	_disable_interrupts()
	cli
; 0001 006F 	nointerrupted = 1;
	LDI  R16,LOW(1)
; 0001 0070 	}
; 0001 0071 //====================================================================
; 0001 0072 // My UPDATE - not optimized
; 0001 0073   for(index=0;index!=MainTimerQueueSize+1;++index)	//Прочесываем очередь таймеров
_0x20006:
	LDI  R17,LOW(0)
_0x20008:
	CPI  R17,16
	BREQ _0x20009
; 0001 0074 	{
; 0001 0075 	if(MainTimer[index].GoToTask == TS)				// Если уже есть запись с таким адресом
	CALL SUBOPT_0x2
	CALL SUBOPT_0x4
	MOVW R26,R30
	LDD  R30,Y+4
	LDD  R31,Y+4+1
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2000A
; 0001 0076 		{
; 0001 0077 		MainTimer[index].Time = NewTime;			// Перезаписываем ей выдержку
	CALL SUBOPT_0x2
	CALL SUBOPT_0x5
; 0001 0078 		if (nointerrupted) 	_enable_interrupts()		// Разрешаем прерывания если не были запрещены.
	BREQ _0x2000B
	sei
; 0001 0079 		return;										// Выходим. Раньше был код успешной операции. Пока убрал
_0x2000B:
	RJMP _0x20A0003
; 0001 007A 		}
; 0001 007B 	}
_0x2000A:
	SUBI R17,-LOW(1)
	RJMP _0x20008
_0x20009:
; 0001 007C   for(index=0;index!=MainTimerQueueSize+1;++index)	// Если не находим похожий таймер, то ищем любой пустой
	LDI  R17,LOW(0)
_0x2000D:
	CPI  R17,16
	BREQ _0x2000E
; 0001 007D 	{
; 0001 007E 	if (MainTimer[index].GoToTask == Idle)
	CALL SUBOPT_0x2
	CALL SUBOPT_0x4
	CALL SUBOPT_0x6
	BRNE _0x2000F
; 0001 007F 		{
; 0001 0080 		MainTimer[index].GoToTask = TS;			// Заполняем поле перехода задачи
	CALL SUBOPT_0x2
	ADD  R30,R26
	ADC  R31,R27
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	STD  Z+0,R26
	STD  Z+1,R27
; 0001 0081 		MainTimer[index].Time = NewTime;		// И поле выдержки времени
	CALL SUBOPT_0x2
	CALL SUBOPT_0x5
; 0001 0082 		if (nointerrupted) 	_enable_interrupts()	// Разрешаем прерывания
	BREQ _0x20010
	sei
; 0001 0083 		return;									// Выход.
_0x20010:
	RJMP _0x20A0003
; 0001 0084 		}
; 0001 0085 
; 0001 0086 	}
_0x2000F:
	SUBI R17,-LOW(1)
	RJMP _0x2000D
_0x2000E:
; 0001 0087 //====================================================================
; 0001 0088 /*
; 0001 0089   for(index=0;index!=MainTimerQueueSize+1;++index)	//Прочесываем очередь таймеров
; 0001 008A 	{
; 0001 008B 	if(MainTimer[index].GoToTask == TS)				// Если уже есть запись с таким адресом
; 0001 008C 		{
; 0001 008D 		MainTimer[index].Time = NewTime;			// Перезаписываем ей выдержку
; 0001 008E 		if (nointerrupted) 	_enable_interrupts()		// Разрешаем прерывания если не были запрещены.
; 0001 008F 		return;										// Выходим. Раньше был код успешной операции. Пока убрал
; 0001 0090 		}
; 0001 0091 	}
; 0001 0092   for(index=0;index!=MainTimerQueueSize+1;++index)	// Если не находим похожий таймер, то ищем любой пустой
; 0001 0093 	{
; 0001 0094 	if (MainTimer[index].GoToTask == Idle)
; 0001 0095 		{
; 0001 0096 		MainTimer[index].GoToTask = TS;			// Заполняем поле перехода задачи
; 0001 0097 		MainTimer[index].Time = NewTime;		// И поле выдержки времени
; 0001 0098 		if (nointerrupted) 	_enable_interrupts()	// Разрешаем прерывания
; 0001 0099 		return;									// Выход.
; 0001 009A 		}
; 0001 009B 
; 0001 009C 	}	*/								// тут можно сделать return c кодом ошибки - нет свободных таймеров
; 0001 009D }
_0x20A0003:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,6
	RET
;
;
;
;
;/*=================================================================================
;Диспетчер задач ОС. Выбирает из очереди задачи и отправляет на выполнение.
;*/
;
;inline void TaskManager(void)
; 0001 00A7 {
_TaskManager:
; 0001 00A8 uint8_t		index=0;
; 0001 00A9 
; 0001 00AA //UPDATE
; 0001 00AB TPTR task;
; 0001 00AC //TPTR	GoToTask = Idle;		// Инициализируем переменные
; 0001 00AD 
; 0001 00AE _disable_interrupts()				// Запрещаем прерывания!!!
	CALL __SAVELOCR4
;	index -> R17
;	*task -> R18,R19
	LDI  R17,0
	cli
; 0001 00AF //UPDATE
; 0001 00B0 //================================================================================================
; 0001 00B1   for(index=0;index!=MainTimerQueueSize+1;++index) {  // Прочесываем очередь в поисках нужной задачи
	LDI  R17,LOW(0)
_0x20012:
	CPI  R17,16
	BREQ _0x20013
; 0001 00B2 		if ((MainTimer[index].GoToTask != Idle)&&(MainTimer[index].Time==0)) { // пропускаем пустые задачи и те, время которых еще не подошло
	CALL SUBOPT_0x2
	CALL SUBOPT_0x4
	CALL SUBOPT_0x6
	BREQ _0x20015
	CALL SUBOPT_0x2
	ADD  R26,R30
	ADC  R27,R31
	ADIW R26,2
	CALL __GETW1P
	SBIW R30,0
	BREQ _0x20016
_0x20015:
	RJMP _0x20014
_0x20016:
; 0001 00B3 		    task=MainTimer[index].GoToTask;             // запомним задачу
	CALL SUBOPT_0x2
	ADD  R26,R30
	ADC  R27,R31
	LD   R18,X+
	LD   R19,X
; 0001 00B4 		    MainTimer[index].GoToTask = Idle;           // ставим затычку
	CALL SUBOPT_0x2
	CALL SUBOPT_0x3
; 0001 00B5             _enable_interrupts()							// Разрешаем прерывания
	sei
; 0001 00B6             (task)();								    // Переходим к задаче
	MOVW R30,R18
	ICALL
; 0001 00B7             return;                                     // выход до следующего цикла
	RJMP _0x20A0002
; 0001 00B8 		}
; 0001 00B9 	}
_0x20014:
	SUBI R17,-LOW(1)
	RJMP _0x20012
_0x20013:
; 0001 00BA     _enable_interrupts()							// Разрешаем прерывания
	sei
; 0001 00BB 	Idle();                                     // обошли задачи, нужных нет - простой
	RCALL _Idle
; 0001 00BC //====================================================================================================
; 0001 00BD /* //UPDATE
; 0001 00BE GoToTask = TaskQueue[0];		// Хватаем первое значение из очереди
; 0001 00BF if (GoToTask==Idle) 			// Если там пусто
; 0001 00C0 	{_enable_interrupts()			// Разрешаем прерывания
; 0001 00C1 	(Idle)(); 					// Переходим на обработку пустого цикла
; 0001 00C2 	}
; 0001 00C3 else
; 0001 00C4 	{ for(index=0;index!=TaskQueueSize;index++)	// В противном случае сдвигаем всю очередь
; 0001 00C5 		{
; 0001 00C6 		TaskQueue[index]=TaskQueue[index+1];
; 0001 00C7 		}
; 0001 00C8 	TaskQueue[TaskQueueSize]= Idle;				// В последнюю запись пихаем затычку
; 0001 00C9 _enable_interrupts()							// Разрешаем прерывания
; 0001 00CA 	(GoToTask)();								// Переходим к задаче
; 0001 00CB 	}
; 0001 00CC  */
; 0001 00CD }
_0x20A0002:
	CALL __LOADLOCR4
	ADIW R28,4
	RET
;
;/*
;Служба таймеров ядра. Должна вызываться из прерывания раз в 1мс. Хотя время можно варьировать в зависимости от задачи
;
;To DO: Привести к возможности загружать произвольную очередь таймеров. Тогда можно будет создавать их целую прорву.
;А также использовать эту функцию произвольным образом.
;В этом случае не забыть добавить проверку прерывания.
;*/
;inline void TimerService(void)
; 0001 00D7 {
_TimerService:
; 0001 00D8 uint8_t index;
; 0001 00D9 
; 0001 00DA for(index=0;index!=MainTimerQueueSize+1;index++)		// Прочесываем очередь таймеров
	ST   -Y,R17
;	index -> R17
	LDI  R17,LOW(0)
_0x20018:
	CPI  R17,16
	BREQ _0x20019
; 0001 00DB 	{
; 0001 00DC //==========================================================================
; 0001 00DD //UPDATE
; 0001 00DE          if((MainTimer[index].GoToTask != Idle) && 		    // Если не пустышка и
; 0001 00DF            (MainTimer[index].Time > 0)) {					// таймер не выщелкал, то
	CALL SUBOPT_0x2
	CALL SUBOPT_0x4
	CALL SUBOPT_0x6
	BREQ _0x2001B
	CALL SUBOPT_0x2
	ADD  R26,R30
	ADC  R27,R31
	ADIW R26,2
	CALL __GETW1P
	MOVW R26,R30
	CALL __CPW02
	BRLO _0x2001C
_0x2001B:
	RJMP _0x2001A
_0x2001C:
; 0001 00E0             MainTimer[index].Time--;						// щелкаем еще раз.
	CALL SUBOPT_0x2
	ADD  R26,R30
	ADC  R27,R31
	ADIW R26,2
	LD   R30,X+
	LD   R31,X+
	SBIW R30,1
	ST   -X,R31
	ST   -X,R30
; 0001 00E1 		};
_0x2001A:
; 0001 00E2 //================================================================================
; 0001 00E3      /*  //update
; 0001 00E4 	if(MainTimer[index].GoToTask == Idle) continue;		// Если нашли пустышку - щелкаем следующую итерацию
; 0001 00E5 	if(MainTimer[index].Time !=1)						// Если таймер не выщелкал, то щелкаем еще раз.
; 0001 00E6 		{												// To Do: Вычислить по тактам, что лучше !=1 или !=0.
; 0001 00E7 		MainTimer[index].Time --;						// Уменьшаем число в ячейке если не конец.
; 0001 00E8 		}
; 0001 00E9 	else
; 0001 00EA 		{
; 0001 00EB 		SetTask(MainTimer[index].GoToTask);				// Дощелкали до нуля? Пихаем в очередь задачу
; 0001 00EC 		MainTimer[index].GoToTask = Idle;				// А в ячейку пишем затычку
; 0001 00ED 		}
; 0001 00EE         */
; 0001 00EF 	}
	SUBI R17,-1
	RJMP _0x20018
_0x20019:
; 0001 00F0 }
_0x20A0001:
	LD   R17,Y+
	RET
;
;
;
;//добавление от sniuk 7.1.14
;void ClearTimerTask(TPTR TS)  //обнуление таймера
; 0001 00F6 {
; 0001 00F7 uint8_t	 index=0;
; 0001 00F8 uint8_t nointerrupted = 0;
; 0001 00F9 if (STATUS_REG & (1<<Interrupt_Flag))
;	*TS -> Y+2
;	index -> R17
;	nointerrupted -> R16
; 0001 00FA {
; 0001 00FB _disable_interrupts();
; 0001 00FC nointerrupted = 1;
; 0001 00FD }
; 0001 00FE     for(index=0; index!=MainTimerQueueSize+1; ++index)
; 0001 00FF     {
; 0001 0100         if(MainTimer[index].GoToTask == TS)
; 0001 0101         {
; 0001 0102             MainTimer[index].GoToTask = Idle;
; 0001 0103             MainTimer[index].Time = 0; // Обнуляем время
; 0001 0104             if (nointerrupted) _enable_interrupts();
; 0001 0105             return;
; 0001 0106         }
; 0001 0107     }
; 0001 0108 }
;
;
;    #warning обновил на файл ниже!
;/*
;
;Автор www.google.com-accounts-o8-id-id-AItOawmi18Y12U8R4bYF3i0GRgR
;Язык: C++
;Опубликовано 8 ноября 2011 года в 00:05
;Просмотров 1067
;Вставлю и свои 5 копеек :) Избавился от очереди задач TaskQueue, вместо этого в диспетчере задач выполняются задачи из очереди MainTimer, которые уже "выщелкали", соответственно количество кода очень уменьшилось. Изменен только код eertos.c
;#include "eertos.h"
;
;// Очередь задач.
;volatile static struct	{
;	TPTR GoToTask; 						// Указатель перехода
;	uint16_t Time;							// Выдержка в мс
;} MainTimer[MainTimerQueueSize+1];	// Очередь таймеров
;
;// RTOS Подготовка. Очистка очередей
;inline void InitRTOS(void)
;{
;    uint8_t	index;
;
;    for(index=0;index!=MainTimerQueueSize+1;index++) { // Обнуляем все таймеры.
;		MainTimer[index].GoToTask = Idle;
;        MainTimer[index].Time = 0;
;	}
;}
;
;//Пустая процедура - простой ядра.
;void  Idle(void) {
;
;}
;
;//Функция установки задачи по таймеру. Передаваемые параметры - указатель на функцию,
;// Время выдержки в тиках системного таймера. Возвращет код ошибки.
;void SetTimerTask(TPTR TS, uint16_t NewTime) {
;
;    uint8_t		index=0;
;    uint8_t		nointerrupted = 0;
;
;    if (STATUS_REG & (_BV(Interrupt_Flag))) { 			// Проверка запрета прерывания, аналогично функции выше
;        Disable_Interrupt
;        nointerrupted = 1;
;	}
;
;    for(index=0;index!=MainTimerQueueSize+1;++index) {	//Прочесываем очередь таймеров
;        if(MainTimer[index].GoToTask == TS) {			// Если уже есть запись с таким адресом
;            MainTimer[index].Time = NewTime;			// Перезаписываем ей выдержку
;            if (nointerrupted) 	Enable_Interrupt		// Разрешаем прерывания если не были запрещены.
;            return;										// Выходим. Раньше был код успешной операции. Пока убрал
;		}
;	}
;
;    for(index=0;index!=MainTimerQueueSize+1;++index) {	// Если не находим похожий таймер, то ищем любой пустой
;		if (MainTimer[index].GoToTask == Idle) {
;			MainTimer[index].GoToTask = TS;			// Заполняем поле перехода задачи
;            MainTimer[index].Time = NewTime;		// И поле выдержки времени
;            if (nointerrupted) 	Enable_Interrupt	// Разрешаем прерывания
;            return;									// Выход.
;		}
;	}												// тут можно сделать return c кодом ошибки - нет свободных таймеров
;}
;
;void SetTask(TPTR TS) {                             // Поставить задачу в очередь для немедленного выполнения
;    SetTimerTask(TS,0);
;}
;
;//=================================================================================
;Диспетчер задач ОС. Выбирает из очереди задачи и отправляет на выполнение.
;
;
;inline void TaskManager(void) {
;
;uint8_t		index=0;
;TPTR task;
;
;    Disable_Interrupt				// Запрещаем прерывания!!!
;    for(index=0;index!=MainTimerQueueSize+1;++index) {  // Прочесываем очередь в поисках нужной задачи
;		if ((MainTimer[index].GoToTask != Idle)&&(MainTimer[index].Time==0)) { // пропускаем пустые задачи и те, время которых еще не подошло
;		    task=MainTimer[index].GoToTask;             // запомним задачу
;		    MainTimer[index].GoToTask = Idle;           // ставим затычку
;            Enable_Interrupt							// Разрешаем прерывания
;            (task)();								    // Переходим к задаче
;            return;                                     // выход до следующего цикла
;		}
;	}
;    Enable_Interrupt							// Разрешаем прерывания
;	Idle();                                     // обошли задачи, нужных нет - простой
;}
;
;
;//Служба таймеров ядра. Должна вызываться из прерывания раз в 1мс. Хотя время можно варьировать в зависимости от задачи
;
;inline void TimerService(void) {
;
;uint8_t index;
;
;    for(index=0;index!=MainTimerQueueSize+1;index++) {		// Прочесываем очередь таймеров
;        if((MainTimer[index].GoToTask != Idle) && 		    // Если не пустышка и
;           (MainTimer[index].Time > 0)) {					// таймер не выщелкал, то
;            MainTimer[index].Time--;						// щелкаем еще раз.
;		};
;	}
;}
;
;*/
;#include "RTOS/EERTOSHAL.h"
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x20
	.EQU __sm_mask=0x1C
	.EQU __sm_powerdown=0x10
	.EQU __sm_powersave=0x18
	.EQU __sm_standby=0x14
	.EQU __sm_ext_standby=0x1C
	.EQU __sm_adc_noise_red=0x08
	.SET power_ctrl_reg=mcucr
	#endif
;
;//RTOS Запуск системного таймера
;  void RunRTOS (void)
; 0002 0005 {

	.CSEG
_RunRTOS:
; 0002 0006 TCCR2 = (1<<WGM21)|(1<<CS21)|(1<<CS20); 				// Freq = CK/64 - Установить режим и предделитель
	LDI  R30,LOW(11)
	OUT  0x25,R30
; 0002 0007 										// Автосброс после достижения регистра сравнения
; 0002 0008 TCNT2 = 0;								// Установить начальное значение счётчиков
	LDI  R30,LOW(0)
	OUT  0x24,R30
; 0002 0009 OCR2  = LO(TimerDivider); 				// Установить значение в регистр сравнения
	LDI  R30,LOW(125)
	OUT  0x23,R30
; 0002 000A TIMSK = (0<<TOIE0)|(1<<OCIE2);	        	// Разрешаем прерывание RTOS - запуск ОС
	LDI  R30,LOW(128)
	OUT  0x37,R30
; 0002 000B 
; 0002 000C #asm("sei");
	sei
; 0002 000D }
	RET
;#include "RTOS/HAL.h"
;#include <mega128.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x20
	.EQU __sm_mask=0x1C
	.EQU __sm_powerdown=0x10
	.EQU __sm_powersave=0x18
	.EQU __sm_standby=0x14
	.EQU __sm_ext_standby=0x1C
	.EQU __sm_adc_noise_red=0x08
	.SET power_ctrl_reg=mcucr
	#endif
;
;  void InitAll(void)
; 0003 0005 {

	.CSEG
_InitAll:
; 0003 0006 
; 0003 0007 
; 0003 0008 //InitPort
; 0003 0009 LED_DDR |= 1<<LED1|1<<LED2|1<<LED3;//|1<<I_L|1<<I_C;
	IN   R30,0x11
	ORI  R30,LOW(0xC8)
	OUT  0x11,R30
; 0003 000A 
; 0003 000B 
; 0003 000C }
	RET
;
;
;

	.CSEG

	.CSEG

	.DSEG

	.CSEG
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x20
	.EQU __sm_mask=0x1C
	.EQU __sm_powerdown=0x10
	.EQU __sm_powersave=0x18
	.EQU __sm_standby=0x14
	.EQU __sm_ext_standby=0x1C
	.EQU __sm_adc_noise_red=0x08
	.SET power_ctrl_reg=mcucr
	#endif

	.CSEG

	.CSEG

	.CSEG

	.DSEG
_MainTimer_G001:
	.BYTE 0x40
__seed_G101:
	.BYTE 0x4

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x0:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _SetTimerTask

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x1:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _SetTimerTask

;OPTIMIZER ADDED SUBROUTINE, CALLED 14 TIMES, CODE SIZE REDUCTION:75 WORDS
SUBOPT_0x2:
	MOV  R30,R17
	LDI  R26,LOW(_MainTimer_G001)
	LDI  R27,HIGH(_MainTimer_G001)
	LDI  R31,0
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x3:
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(_Idle)
	LDI  R31,HIGH(_Idle)
	ST   X+,R30
	ST   X,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x4:
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETW1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x5:
	ADD  R30,R26
	ADC  R31,R27
	ADIW R30,2
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	STD  Z+0,R26
	STD  Z+1,R27
	CPI  R16,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x6:
	MOVW R26,R30
	LDI  R30,LOW(_Idle)
	LDI  R31,HIGH(_Idle)
	CP   R30,R26
	CPC  R31,R27
	RET


	.CSEG
__LSLW2:
	LSL  R30
	ROL  R31
	LSL  R30
	ROL  R31
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__CPW02:
	CLR  R0
	CP   R0,R26
	CPC  R0,R27
	RET

__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

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
