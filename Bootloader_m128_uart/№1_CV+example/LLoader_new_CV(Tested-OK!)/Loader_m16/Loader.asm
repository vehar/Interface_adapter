
;CodeVisionAVR C Compiler V2.05.0 Evaluation
;(C) Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type                : ATmega16
;Program type             : Boot Loader
;Clock frequency          : 8,000000 MHz
;Memory model             : Small
;Optimize for             : Size
;(s)printf features       : int
;(s)scanf features        : int, width
;External RAM size        : 0
;Data Stack size          : 256 byte(s)
;Heap size                : 0 byte(s)
;Promote 'char' to 'int'  : No
;'char' is unsigned       : Yes
;8 bit enums              : Yes
;global 'const' stored in FLASH: No
;Enhanced core instructions    : On
;Smart register allocation     : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATmega16
	#pragma AVRPART MEMORY PROG_FLASH 16384
	#pragma AVRPART MEMORY EEPROM 512
	#pragma AVRPART MEMORY INT_SRAM SIZE 1119
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

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
	.EQU GICR=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

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

	.EQU __SRAM_START=0x0060
	.EQU __SRAM_END=0x045F
	.EQU __DSTACK_SIZE=0x0100
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
	.DEF _PageAddress=R4
	.DEF _RealPageAddress=R6

	.CSEG
	.ORG 0x1E00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	JMP  __RESET
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00
	JMP  0x1E00

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF THE BOOT LOADER
	LDI  R31,1
	OUT  GICR,R31
	LDI  R31,2
	OUT  GICR,R31
	OUT  MCUCR,R30

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
	LDI  R26,__SRAM_START
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

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
	.DB  0 ; FIRST EEPROM LOCATION NOT USED, SEE ATMEL ERRATA SHEETS

	.DSEG
	.ORG 0x160

	.CSEG
;//*****************************************************************************
;//* BootLoader 7.4
;//*
;//* Devices supported at this time and report Ok, from users
;//* ATMega8
;//* ATMega16
;//* ATMega32
;//* ATMega64
;//* ATMega128
;//* ATMega162
;//* ATMega169
;//* ATMega8515
;//* ATMega8535
;//* ATMega88
;//* ATMega1280
;//* ATMega2560
;//* ATMCAN128
;//* ATMega164/324/644
;//* ATMega324
;//* ATMega324P
;//* ATMega2561
;//* ATMega164
;//* ATMCAN32
;//* ATMega328
;//*
;//* Everything is AS IS without any warranty of any kind.
;//*
;//* Note:
;//* -----
;//* I can't write bootloader of all the MCU it's not my primary job and I don't
;//* make $$$ with that
;//*
;//* If you make new #define please let me know I will update the bootloader
;//* file it will help other AVR users
;//*
;//* bibi@MicroSyl.com
;//*****************************************************************************
;
;
;//*****************************************************************************
;//*****************************************************************************
;// IF YOU MAKE NEW DEFINE THAT IS WORKING PLEASE LET ME KNOW TO UPDATE MEGALOAD
;// This software is free, so I can't pass all my time writting new bootloader
;// for new MCU. I'm shure that you can help me and help ALL MEGALOAD USERS
;//*****************************************************************************
;//*****************************************************************************
;
;
;//*****************************************************************************
;//
;// To setup the bootloader for your project you must
;// remove the comment below to fit with your hardware
;// recompile it using ICCAVR setup for bootloader
;//
;// Flash, EEprom, Lockbit Programming take a bootloader of 512 word
;//
;// if you chose the SMALL256 you will only be able to program the flash without
;// any communication and flash verification.  You will need a bootloader size
;// of 256 word
;//
;//*****************************************************************************
;// MCU selection
;//
;// *************************************
;// *->Do the same thing in assembly.s<-*
;// *************************************
;//
;//*****************************************************************************
;
;//#define MEGATYPE  Mega8
;#define MEGATYPE Mega16
;//#define MEGATYPE Mega64
;//#define MEGATYPE Mega128
;//#define MEGATYPE Mega32
;//#define MEGATYPE Mega162
;//#define MEGATYPE Mega169
;//#define MEGATYPE Mega8515
;//#define MEGATYPE Mega8535
;//#define MEGATYPE Mega163
;//#define MEGATYPE Mega323
;//#define MEGATYPE Mega48
;//#define MEGATYPE Mega88
;//#define MEGATYPE Mega168
;//#define MEGATYPE Mega165
;//#define MEGATYPE Mega3250
;//#define MEGATYPE Mega6450
;//#define MEGATYPE Mega3290
;//#define MEGATYPE Mega6490
;//#define MEGATYPE Mega406
;//#define MEGATYPE Mega640
;//#define MEGATYPE Mega1280
;//#define MEGATYPE Mega2560
;//#define MEGATYPE MCAN128
;//#define MEGATYPE Mega164
;//#define MEGATYPE Mega328
;//#define MEGATYPE Mega324
;//#define MEGATYPE Mega325
;//#define MEGATYPE Mega644
;//#define MEGATYPE Mega645
;//#define MEGATYPE Mega1281
;//#define MEGATYPE Mega2561
;//#define MEGATYPE Mega404
;//#define MEGATYPE MUSB1286
;//#define MEGATYPE MUSB1287
;//#define MEGATYPE MUSB162
;//#define MEGATYPE MUSB646
;//#define MEGATYPE MUSB647
;//#define MEGATYPE MUSB82
;//#define MEGATYPE MCAN32
;//#define MEGATYPE MCAN64
;//#define MEGATYPE Mega329
;//#define MEGATYPE Mega649
;//#define MEGATYPE Mega256
;
;//*****************************************************************************
;// MCU Frequency
;//*****************************************************************************
;#define XTAL        8000000
;
;//*****************************************************************************
;// Bootload on UART x
;//*****************************************************************************
;#define UART        0
;//#define UART       1
;//#define UART       2
;//#define UART       3
;
;//*****************************************************************************
;// BaudRate
;//*****************************************************************************
;#define BAUDRATE     38400
;
;//*****************************************************************************
;// EEprom programming
;// enable EEprom programing via bootloader
;//*****************************************************************************
;//#define EEPROM
;
;//*****************************************************************************
;// LockBit programming
;// enable LOCKBIT programing via bootloader
;//*****************************************************************************
;//#define LOCKBIT
;
;//*****************************************************************************
;// Small 256 Bootloader without eeprom programming, lockbit programming
;// and no data verification
;//*****************************************************************************
;#define SMALL256
;
;//*****************************************************************************
;// RS485
;// if you use RS485 half duplex for bootloader
;// make the appropriate change for RX/TX transceiver switch
;//*****************************************************************************
;//#define RS485DDR  DDRB
;//#define RS485PORT PORTB
;//#define RS485TXE  0x08
;
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;//                 DO NOT CHANGE ANYTHING BELOW THIS LINE
;//               IF YOU DON'T REALLY KNOW WHAT YOU ARE DOING
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;
;#define WDR() #asm("wdr")
;
;#define Mega8           'A'
;#define Mega16          'B'
;#define Mega64          'C'
;#define Mega128         'D'
;#define Mega32          'E'
;#define Mega162         'F'
;#define Mega169         'G'
;#define Mega8515        'H'
;#define Mega8535        'I'
;#define Mega163         'J'
;#define Mega323         'K'
;#define Mega48          'L'
;#define Mega88          'M'
;#define Mega168         'N'
;
;#define Mega165         0x80
;#define Mega3250        0x81
;#define Mega6450        0x82
;#define Mega3290        0x83
;#define Mega6490        0x84
;#define Mega406         0x85
;#define Mega640         0x86
;#define Mega1280        0x87
;#define Mega2560        0x88
;#define MCAN128         0x89
;#define Mega164					0x8a
;#define Mega328					0x8b
;#define Mega324					0x8c
;#define Mega325					0x8d
;#define Mega644					0x8e
;#define Mega645					0x8f
;#define Mega1281				0x90
;#define Mega2561				0x91
;#define Mega404					0x92
;#define MUSB1286				0x93
;#define MUSB1287				0x94
;#define MUSB162					0x95
;#define MUSB646					0x96
;#define MUSB647					0x97
;#define MUSB82					0x98
;#define MCAN32					0x9a
;#define MCAN64					0x9b
;#define Mega329					0x9c
;#define Mega649					0x9d
;#define Mega256					0x9e
;
;#define Flash1k         'g'
;#define Flash2k         'h'
;#define Flash4k         'i'
;#define Flash8k         'l'
;#define Flash16k        'm'
;#define Flash32k        'n'
;#define Flash64k        'o'
;#define Flash128k       'p'
;#define Flash256k       'q'
;#define Flash40k        'r'
;
;#define EEprom64        '.'
;#define EEprom128       '/'
;#define EEprom256       '0'
;#define EEprom512       '1'
;#define EEprom1024      '2'
;#define EEprom2048      '3'
;#define EEprom4096      '4'
;
;#define Boot128         'a'
;#define Boot256         'b'
;#define Boot512         'c'
;#define Boot1024        'd'
;#define Boot2048        'e'
;#define Boot4096        'f'
;
;#define Page32          'Q'
;#define Page64          'R'
;#define Page128         'S'
;#define Page256         'T'
;#define Page512         'V'
;
;#if !(defined MEGATYPE) && !(defined MCU)
;  #error "Processor Type is Undefined"
;#endif
;
;#ifdef EEPROM
;  #define  BootSize       Boot1024
;#endif
;
;#ifndef EEPROM
;  #define  BootSize       Boot512
;#endif
;
;#if (MEGATYPE == Mega8)
;  #include "iom8v.h"
;  #define  DeviceID       Mega8
;  #define  FlashSize      Flash8k
;  #define  PageSize       Page64
;  #define  EEpromSize     EEprom512
;  #define  PageByte       64
;  #define  NSHIFTPAGE     6
;  #define  INTVECREG      GICR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega16)
; // #include "iom16v.h"
;   #include <mega16.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;  #define  DeviceID       Mega16
;  #define  FlashSize      Flash16k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom512
;  #define  PageByte       128
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      GICR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega64)
;  #include "iom64v.h"
;  #define  DeviceID       Mega64
;  #define  FlashSize      Flash64k
;  #define  PageSize       Page256
;  #define  EEpromSize     EEprom2048
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;  #if (UART == 0)
;    #define PULLUPPORT      PORTE
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 1)
;    #define PULLUPPORT      PORTD
;    #define PULLUPPIN       0x04
;  #endif
;#endif
;
;#if (MEGATYPE == Mega128)
;  #include "iom128v.h"
;  #define  DeviceID       Mega128
;  #define  FlashSize      Flash128k
;  #define  PageSize       Page256
;  #define  EEpromSize     EEprom4096
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;  #define  RAMPZ_FLAG
;  #if (UART == 0)
;    #define PULLUPPORT      PORTE
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 1)
;    #define PULLUPPORT      PORTD
;    #define PULLUPPIN       0x04
;  #endif
;#endif
;
;#if (MEGATYPE == Mega32)
;  #include "iom32v.h"
;  #define  DeviceID       Mega32
;  #define  FlashSize      Flash32k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom1024
;  #define  PageByte       128
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      GICR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega162)
;  #include "iom162v.h"
;  #define  DeviceID       Mega162
;  #define  FlashSize      Flash16k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom512
;  #define  PageByte       128
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      GICR
;  #if (UART == 0)
;    #define PULLUPPORT      PORTD
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 1)
;    #define PULLUPPORT      PORTB
;    #define PULLUPPIN       0x04
;  #endif
;#endif
;
;#if (MEGATYPE == Mega169)
;  #include "iom169v.h"
;  #define  DeviceID       Mega169
;  #define  FlashSize      Flash16k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom512
;  #define  PageByte       128
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      MCUCR
;  #define  PULLUPPORT     PORTE
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega8515)
;  #include "iom8515v.h"
;  #define  DeviceID       Mega8515
;  #define  FlashSize      Flash8k
;  #define  PageSize       Page64
;  #define  EEpromSize     EEprom512
;  #define  PageByte       64
;  #define  NSHIFTPAGE     6
;  #define  INTVECREG      GICR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega8535)
;  #include "iom8515v.h"
;  #define  DeviceID       Mega8535
;  #define  FlashSize      Flash8k
;  #define  PageSize       Page64
;  #define  EEpromSize     EEprom512
;  #define  PageByte       64
;  #define  NSHIFTPAGE     6
;  #define  INTVECREG      GICR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega163)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega323)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega48)
;  #include "iom48v.h"
;  #define  DeviceID       Mega48
;  #define  FlashSize      Flash4k
;  #define  PageSize       Page64
;  #define  EEpromSize     EEprom256
;  #define  PageByte       64
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      MCUCR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega88)
;  #include "iom88v.h"
;  #define  DeviceID       Mega88
;  #define  FlashSize      Flash8k
;  #define  PageSize       Page64
;  #define  EEpromSize     EEprom512
;  #define  PageByte       64
;  #define  NSHIFTPAGE     6
;  #define  INTVECREG      MCUCR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega168)
;  #include "iom168v.h"
;  #define  DeviceID       Mega168
;  #define  FlashSize      Flash16k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom512
;  #define  PageByte       128
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      MCUCR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega165)
;  #include "iom165v.h"
;  #define  DeviceID       Mega165
;  #define  FlashSize      Flash16k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom512
;  #define  PageByte       128
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      MCUCR
;  #define  PULLUPPORT     PORTE
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega3250)
;  #include "iom325v.h"
;  #define  DeviceID       Mega3250
;  #define  FlashSize      Flash32k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom1024
;  #define  PageByte       128
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      MCUCR
;  #define  PULLUPPORT     PORTE
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega6450)
;  #include "iom645v.h"
;  #define  DeviceID       Mega6450
;  #define  FlashSize      Flash64k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom2048
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;  #define  PULLUPPORT     PORTE
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega3290)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega6490)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega406)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega640)
;  #include "iom640v.h"
;  #define  DeviceID       Mega640
;  #define  FlashSize      Flash64k
;  #define  PageSize       Page256
;  #define  EEpromSize     EEprom4096
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;  #if (UART == 0)
;    #define PULLUPPORT      PORTE
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 1)
;    #define PULLUPPORT      PORTD
;    #define PULLUPPIN       0x04
;  #endif
;
;  #if (UART == 2)
;   #define PULLUPPORT      PORTH
;   #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 3)
;    #define PULLUPPORT      PORTJ
;    #define PULLUPPIN       0x01
;  #endif
;#endif
;
;#if (MEGATYPE == Mega1280)
;  #include "iom1280v.h"
;  #define  DeviceID       Mega1280
;  #define  FlashSize      Flash128k
;  #define  PageSize       Page256
;  #define  EEpromSize     EEprom4096
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;
;  #if (UART == 0)
;    #define PULLUPPORT      PORTE
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 1)
;    #define PULLUPPORT      PORTD
;    #define PULLUPPIN       0x04
;  #endif
;
;  #if (UART == 2)
;    #define PULLUPPORT      PORTH
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 3)
;    #define PULLUPPORT      PORTJ
;    #define PULLUPPIN       0x01
;  #endif
;#endif
;
;#if (MEGATYPE == Mega2560)
;  #include "iom256v.h"
;  #define  DeviceID       Mega2560
;  #define  FlashSize      Flash256k
;  #define  PageSize       Page256
;  #define  EEpromSize     EEprom4096
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;  #define  RAMPZ_FLAG
;
;
;  #if (UART == 0)
;    #define PULLUPPORT      PORTE
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 1)
;    #define PULLUPPORT      PORTD
;    #define PULLUPPIN       0x04
;  #endif
;
;  #if (UART == 2)
;    #define PULLUPPORT      PORTH
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 3)
;    #define PULLUPPORT      PORTJ
;    #define PULLUPPIN       0x01
;  #endif
;#endif
;
;#if (MEGATYPE == MCAN128)
;  #include "ioCAN128v.h"
;  #define  DeviceID       MCAN128
;  #define  FlashSize      Flash128k
;  #define  PageSize       Page256
;  #define  EEpromSize     EEprom4096
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;  #define  RAMPZ_FLAG
;
;  #if (UART == 0)
;    #define PULLUPPORT      PORTE
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 1)
;    #define PULLUPPORT      PORTD
;    #define PULLUPPIN       0x04
;  #endif
;#endif
;
;#if (MEGATYPE == Mega164)
;  #include "iom164pv.h"
;  #define  DeviceID       Mega164
;  #define  FlashSize      Flash16k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom512
;  #define  PageByte       128
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      MCUCR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if(MEGATYPE == Mega324)
;#include "iom324v.h"
;#define  DeviceID  					Mega324
;#define  FlashSize      		Flash32k
;#define  PageSize       		Page128
;#define  EEpromSize     		EEprom1024
;#define  PageBye       			128
;#define  NSHIFTPAGE     		7
;#define  INTVECREG      		MCUCR
;#if (UART == 0)
;  #define PULLUPPORT      	PORTD
;  #define PULLUPPIN       	0x01
;#endif
;#endif
;
;#if (MEGATYPE == Mega325)
; #error "This MCU had not been define"
;#endif
;
;#if(MEGATYPE == Mega644)
;	#include "iom644v.h"
;	#define DeviceID          Mega644
;	#define FlashSize         Flash64k
;	#define PageSize          Page256
;	#define EEpromSize        EEprom2048
;	#define PageByte          256
;	#define NSHIFTPAGE      	8
;	#define INTVECREG        	MCUCR
;	#define PULLUPPORT    		PORTD
;	#define PULLUPPIN        	0x01
;#endif
;
;#if (MEGATYPE == Mega328)
;  #include "iom328pv.h"
;  #define  DeviceID       Mega328
;  #define  FlashSize      Flash32k
;  #define  PageSize       Page128
;  #define  EEpromSize     EEprom1024
;  #define  PageByte       128
;  #define  NSHIFTPAGE     7
;  #define  INTVECREG      MCUCR
;  #define  PULLUPPORT     PORTD
;  #define  PULLUPPIN      0x01
;#endif
;
;#if (MEGATYPE == Mega645)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega1281)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega2561)
;  #include "Mega2561.h"
;  #define  DeviceID       Mega2561
;  #define  FlashSize      Flash256k
;  #define  PageSize       Page256
;  #define  EEpromSize     EEprom4096
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;  #define  IVCE           0
;  #define  RAMPZ_FLAG
;
;
;  #if (UART == 0)
;    #define PULLUPPORT      PORTE
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 1)
;    #define PULLUPPORT      PORTD
;    #define PULLUPPIN       0x04
;  #endif
;
;#endif
;
;#if (MEGATYPE == Mega404)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == MUSB1286)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == MUSB1287)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == MUSB162)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == MUSB646)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == MUSB647)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == MUSB82)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == MCAN32)
;  #include "ioCAN32v.h"
;  #define  DeviceID       MCAN32
;  #define  FlashSize      Flash32k
;  #define  PageSize       Page256
;  #define  EEpromSize     EEprom1024
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;  #define  RAMPZ_FLAG
;#endif
;
;#if (MEGATYPE == MCAN64)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega329)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega649)
; #error "This MCU had not been define"
;#endif
;
;#if (MEGATYPE == Mega256)
;  #include "iom256v.h"
;  #define  DeviceID       Mega256
;  #define  FlashSize      Flash256k
;  #define  PageSize       Page256
;  #define  EEpromSize     EEprom4096
;  #define  PageByte       256
;  #define  NSHIFTPAGE     8
;  #define  INTVECREG      MCUCR
;  #define  RAMPZ_FLAG
;
;
;  #if (UART == 0)
;    #define PULLUPPORT      PORTE
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 1)
;    #define PULLUPPORT      PORTD
;    #define PULLUPPIN       0x04
;  #endif
;
;  #if (UART == 2)
;    #define PULLUPPORT      PORTH
;    #define PULLUPPIN       0x01
;  #endif
;
;  #if (UART == 3)
;    #define PULLUPPORT      PORTJ
;    #define PULLUPPIN       0x01
;  #endif
;#endif
;
;
;// Serial Port defenition
;
;#if !(defined MEGATYPE) && !(defined MCU)
;  #error "Processor Type is Undefined"
;#endif
;
;#if (UART == 0) && !(defined UCSR0A)
; #define  _UCSRA          UCSRA
; #define  _UCSRB         	UCSRB
; #define  _UCSRC         	UCSRC
; #define  _UBRRL          UBRRL
; #define  _UBRRH          UBRRH
; #define  _UDR            UDR
; #define  _TXC						TXC
;#endif
;
;#if (UART == 0) && (defined UCSR0A)
; #define  _UCSRA          UCSR0A
; #define  _UCSRB         	UCSR0B
; #define  _UCSRC         	UCSR0C
; #define  _UBRRL          UBRR0L
; #define  _UBRRH          UBRR0H
; #define  _UDR            UDR0
; #define  _TXC						TXC0
;#endif
;
;#if (UART == 1)
; #define  _UCSRA          UCSR1A
; #define  _UCSRB         	UCSR1B
; #define  _UCSRC         	UCSR1C
; #define  _UBRRL          UBRR1L
; #define  _UBRRH          UBRR1H
; #define  _UDR            UDR1
; #define  _TXC						TXC1
;#endif
;
;#if (UART == 2)
; #define  _UCSRA          UCSR2A
; #define  _UCSRB         	UCSR2B
; #define  _UCSRC         	UCSR2C
; #define  _UBRRL          UBRR2L
; #define  _UBRRH          UBRR2H
; #define  _UDR            UDR2
; #define  _TXC						TX2
;#endif
;
;#if (UART == 3)
; #define  _UCSRA          UCSR3A
; #define  _UCSRB         	UCSR3B
; #define  _UCSRC         	UCSR3C
; #define  _UBRRL          UBRR3L
; #define  _UBRRH          UBRR3H
; #define  _UDR            UDR3
; #define  _TXC						TXC3
;#endif
;
;#define  FALSE          0
;#define  TRUE           1
;
;#ifdef SMALL256
; #undef EEPROM
; #undef LOCKBIT
;#endif
;
;
;/*****************************************************************************/
;/*                          I N C L U D E                                    */
;/*****************************************************************************/
;
;/*****************************************************************************/
;/*                        P R O T O T Y P E                                  */
;/*****************************************************************************/
;
;void GetPageNumber(void);
;char WriteFlashPage(void);
;
;unsigned char RxChar(void);
;void TxChar(unsigned char ch);
;
;#ifdef EEPROM
;void EEpromLoad(void);
;void EEPROMwrite(int location, unsigned char byte);
;unsigned char EEPROMread( int location);
;void LockBit(void);
;#endif
;
;void main(void);
;
;/*****************************************************************************/
;/*                G L O B A L    V A R I A B L E S                           */
;/*****************************************************************************/
;//unsigned char PageBuffer[PageByte];
;unsigned int PageAddress;
;unsigned int RealPageAddress;
;
;#if ((MEGATYPE == Mega64) || (MEGATYPE == Mega128))
; #asm(".EQU SPMCR = 0x68")
;#elif((MEGATYPE == Mega324) || (MEGATYPE == Mega644))
; #asm(".EQU SPMCR = 0x37")
;#else
; #asm(".EQU SPMCR = 0x57")
	.EQU SPMCR = 0x57
;#endif
;
;#pragma warn-
;#pragma used+
;void WAIT_SPMEN(void)
; 0000 0380 {

	.CSEG
_WAIT_SPMEN:
; 0000 0381 #asm
; 0000 0382         LDS     R30,SPMCR       ; load SPMCR to R27
        LDS     R30,SPMCR       ; load SPMCR to R27
; 0000 0383         SBRC    R30,0          ; check SPMEN flag
        SBRC    R30,0          ; check SPMEN flag
; 0000 0384         RJMP    _WAIT_SPMEN     ; wait for SPMEN flag cleared
        RJMP    _WAIT_SPMEN     ; wait for SPMEN flag cleared
; 0000 0385 #endasm
; 0000 0386 }
	RET
;void write_page (unsigned int adr, unsigned char function)
; 0000 0388 //; bits 8:15 adr addresses the page...(must setup RAMPZ beforehand!!!)
; 0000 0389 {
_write_page:
; 0000 038A #asm
;	adr -> Y+1
;	function -> Y+0
; 0000 038B         CALL _WAIT_SPMEN
        CALL _WAIT_SPMEN
; 0000 038C         LDD R30,Y+1
        LDD R30,Y+1
; 0000 038D         LDD R31,Y+2         ;move address to z pointer (R31=ZH R30=ZL)
        LDD R31,Y+2         ;move address to z pointer (R31=ZH R30=ZL)
; 0000 038E         LDD R26,Y+0
        LDD R26,Y+0
; 0000 038F         STS SPMCR,R26       ;argument 2 decides function
        STS SPMCR,R26       ;argument 2 decides function
; 0000 0390         SPM                 ;perform pagewrite
        SPM                 ;perform pagewrite
; 0000 0391 #endasm
; 0000 0392 }
	ADIW R28,3
	RET
;void fill_temp_buffer (unsigned int data, unsigned int adr)
; 0000 0394 {
_fill_temp_buffer:
; 0000 0395 //; bits 7:1 in adr addresses the word in the page... (2=first word, 4=second word etc..)
; 0000 0396 #asm
;	data -> Y+2
;	adr -> Y+0
; 0000 0397         CALL _WAIT_SPMEN
        CALL _WAIT_SPMEN
; 0000 0398         LDD R31,Y+1
        LDD R31,Y+1
; 0000 0399         LDD R30,Y+0         ;move adress to z pointer (R31=ZH R30=ZL)
        LDD R30,Y+0         ;move adress to z pointer (R31=ZH R30=ZL)
; 0000 039A         LDD R1,Y+3
        LDD R1,Y+3
; 0000 039B         LDD R0,Y+2          ;move data to reg 0 and 1
        LDD R0,Y+2          ;move data to reg 0 and 1
; 0000 039C         LDI R26,0x01
        LDI R26,0x01
; 0000 039D         STS SPMCR,R26
        STS SPMCR,R26
; 0000 039E         SPM            ;Store program memory
        SPM            ;Store program memory
; 0000 039F #endasm
; 0000 03A0 }
	RJMP _0x2000001
;unsigned int read_program_memory (unsigned int adr ,unsigned char cmd)
; 0000 03A2 {
; 0000 03A3 #asm
;	adr -> Y+1
;	cmd -> Y+0
; 0000 03A4         LDD R31,Y+2         ;R31=ZH R30=ZL
; 0000 03A5         LDD R30,Y+1         ;move adress to z pointer
; 0000 03A6         LDD R26,Y+0
; 0000 03A7         SBRC R26,0          ;read lockbits? (second argument=0x09)
; 0000 03A8         STS SPMCR,R26       ;if so, place second argument in SPMEN register
; 0000 03A9 #endasm
; 0000 03AA 
; 0000 03AB #ifdef RAMPZ_FLAG
; 0000 03AC  #asm
; 0000 03AD         ELPM    r26, Z+         ;read LSB
; 0000 03AE         ELPM    r27, Z          ;read MSB
; 0000 03AF  #endasm
; 0000 03B0 #else
; 0000 03B1  #asm
; 0000 03B2         LPM     r26, Z+
; 0000 03B3         LPM     r27, Z
; 0000 03B4  #endasm
; 0000 03B5 #endif
; 0000 03B6 #asm
; 0000 03B7         MOVW R30,R26
; 0000 03B8 #endasm
; 0000 03B9 }
;#ifdef LOCKBIT
;void write_lock_bits (unsigned char val)
;{
;#asm
;        LD  R0,Y
;        LDI R30,0x09
;        STS SPMCR,R30
;        SPM                ;write lockbits
;#endasm
;}
;#endif
;void enableRWW(void)
; 0000 03C6 {
_enableRWW:
; 0000 03C7 #asm
; 0000 03C8         CALL _WAIT_SPMEN
        CALL _WAIT_SPMEN
; 0000 03C9         LDI R27,0x11
        LDI R27,0x11
; 0000 03CA         STS SPMCR,R27
        STS SPMCR,R27
; 0000 03CB         SPM
        SPM
; 0000 03CC #endasm
; 0000 03CD }
	RET
;#pragma used-
;#pragma warn+
;
;/*****************************************************************************/
;
;void GetPageNumber(void)
; 0000 03D4 {
_GetPageNumber:
; 0000 03D5   unsigned char PageAddressHigh = RxChar();
; 0000 03D6 
; 0000 03D7   RealPageAddress = (((unsigned int)PageAddressHigh << 8) + RxChar());
	ST   -Y,R17
;	PageAddressHigh -> R17
	RCALL _RxChar
	MOV  R17,R30
	MOV  R31,R17
	LDI  R30,LOW(0)
	PUSH R31
	PUSH R30
	RCALL _RxChar
	POP  R26
	POP  R27
	LDI  R31,0
	ADD  R30,R26
	ADC  R31,R27
	MOVW R6,R30
; 0000 03D8   PageAddress = RealPageAddress << NSHIFTPAGE;
	MOVW R26,R6
	LDI  R30,LOW(7)
	CALL __LSLW12
	MOVW R4,R30
; 0000 03D9 
; 0000 03DA   #ifdef RAMPZ_FLAG
; 0000 03DB   RAMPZ = PageAddressHigh;
; 0000 03DC   #endif
; 0000 03DD }
	LD   R17,Y+
	RET
;
;/*****************************************************************************/
;
;char WriteFlashPage(void)
; 0000 03E2 {
_WriteFlashPage:
; 0000 03E3 	#ifdef SMALL256
; 0000 03E4   //-------------
; 0000 03E5   unsigned int i;
; 0000 03E6   unsigned int TempInt;
; 0000 03E7 
; 0000 03E8   for (i=0;i<PageByte;i+=2)
	CALL __SAVELOCR4
;	i -> R16,R17
;	TempInt -> R18,R19
	__GETWRN 16,17,0
_0x4:
	__CPWRN 16,17,128
	BRSH _0x5
; 0000 03E9   {
; 0000 03EA   	TempInt = RxChar();
	RCALL _RxChar
	MOV  R18,R30
	CLR  R19
; 0000 03EB   	TempInt |= ((unsigned int)RxChar()<<8);
	RCALL _RxChar
	MOV  R31,R30
	LDI  R30,0
	__ORWRR 18,19,30,31
; 0000 03EC   	fill_temp_buffer(TempInt,i);    //call asm routine.
	ST   -Y,R19
	ST   -Y,R18
	ST   -Y,R17
	ST   -Y,R16
	CALL _fill_temp_buffer
; 0000 03ED   }
	__ADDWRN 16,17,2
	RJMP _0x4
_0x5:
; 0000 03EE   write_page(PageAddress,0x03);     //Perform page ERASE
	ST   -Y,R5
	ST   -Y,R4
	LDI  R30,LOW(3)
	ST   -Y,R30
	CALL _write_page
; 0000 03EF   write_page(PageAddress,0x05);     //Perform page write
	ST   -Y,R5
	ST   -Y,R4
	LDI  R30,LOW(5)
	ST   -Y,R30
	CALL _write_page
; 0000 03F0   enableRWW();
	RCALL _enableRWW
; 0000 03F1   i = RxChar();
	RCALL _RxChar
	MOV  R16,R30
	CLR  R17
; 0000 03F2   return 1;
	LDI  R30,LOW(1)
	CALL __LOADLOCR4
_0x2000001:
	ADIW R28,4
	RET
; 0000 03F3 
; 0000 03F4   #else //--------------
; 0000 03F5 
; 0000 03F6   unsigned int i;
; 0000 03F7   unsigned int TempInt;
; 0000 03F8   unsigned char FlashCheckSum = 0;
; 0000 03F9   unsigned char CheckSum = 0;
; 0000 03FA   unsigned char Left;
; 0000 03FB   unsigned char Right;
; 0000 03FC 
; 0000 03FD   for (i=0;i<PageByte;i+=2)
; 0000 03FE   {
; 0000 03FF    Right = RxChar();
; 0000 0400    Left = RxChar();
; 0000 0401    TempInt = (unsigned int)Right + ((unsigned int)Left<<8);
; 0000 0402    CheckSum += (Right + Left);
; 0000 0403    fill_temp_buffer(TempInt,i);      //call asm routine.
; 0000 0404   }
; 0000 0405 
; 0000 0406   if (CheckSum != RxChar()) return 0;
; 0000 0407 
; 0000 0408   write_page(PageAddress,0x03);     //Perform page ERASE
; 0000 0409   write_page(PageAddress,0x05);     //Perform page write
; 0000 040A   enableRWW();
; 0000 040B   for (i=0;i<PageByte;i+=2)
; 0000 040C   {
; 0000 040D     TempInt = read_program_memory(PageAddress + i,0x00);
; 0000 040E     FlashCheckSum += (char)(TempInt & 0x00ff) + (char)(TempInt >> 8);
; 0000 040F   }
; 0000 0410   if (CheckSum != FlashCheckSum) return 0;
; 0000 0411 
; 0000 0412   return 1;
; 0000 0413 
; 0000 0414   #endif
; 0000 0415 }
;
;/*****************************************************************************/
;/* EEprom Programing Code                                                    */
;/*****************************************************************************/
;#ifdef EEPROM
;void EEpromLoad()
;{
;  unsigned char ByteAddressHigh;
;  unsigned char ByteAddressLow;
;  unsigned int ByteAddress;
;  unsigned char Data;
;  unsigned char LocalCheckSum;
;  unsigned char CheckSum;
;
;  TxChar(')');
;  TxChar('!');
;  while (1)
;  {
;  	WDR();
;    LocalCheckSum = 0;
;
;    ByteAddressHigh = RxChar();
;    LocalCheckSum += ByteAddressHigh;
;
;    ByteAddressLow = RxChar();
;    LocalCheckSum += ByteAddressLow;
;
;    ByteAddress = (ByteAddressHigh<<8)+ByteAddressLow;
;
;    if (ByteAddress == 0xffff) return;
;
;    Data = RxChar();
;    LocalCheckSum += Data;
;
;    CheckSum = RxChar();
;
;    if (CheckSum == LocalCheckSum)
;    {
;      EEPROMwrite(ByteAddress, Data);
;      if (EEPROMread(ByteAddress) == Data) TxChar('!');
;      else TxChar('@');
;    }
;    else
;    {
;      TxChar('@');
;    }
;  }
;}
;#endif
;
;/*****************************************************************************/
;
;#ifdef EEPROM
;void EEPROMwrite( int location, unsigned char byte)
;{
;  while (EECR & 0x02) WDR();        // Wait until any earlier write is done
;  EEAR = location;
;  EEDR = byte;
;  EECR |= 0x04;                     // Set MASTER WRITE enable
;  EECR |= 0x02;                     // Set WRITE strobe
;}
;#endif
;
;/*****************************************************************************/
;
;#ifdef EEPROM
;unsigned char EEPROMread( int location)
;{
;  while (EECR & 0x02) WDR();
;  EEAR = location;
;  EECR |= 0x01;                     // Set READ strobe
;  return (EEDR);                    // Return byte
;}
;#endif
;
;/*****************************************************************************/
;/* LockBit Code                                                              */
;/*****************************************************************************/
;#ifdef LOCKBIT
;void LockBit(void)
;{
;  unsigned char Byte;
;
;  TxChar('%');
;
;  Byte = RxChar();
;
;  if (Byte == ~RxChar()) write_lock_bits(~Byte);
;}
;#endif
;
;/*****************************************************************************/
;/* Serial Port Code                                                          */
;/*****************************************************************************/
;
;/*****************************************************************************/
;
;unsigned char RxChar(void)
; 0000 0478 {
_RxChar:
; 0000 0479 	unsigned int TimeOut = 0;
; 0000 047A 
; 0000 047B 	while(!(_UCSRA & 0x80))
	ST   -Y,R17
	ST   -Y,R16
;	TimeOut -> R16,R17
	__GETWRN 16,17,0
_0x6:
	SBIC 0xB,7
	RJMP _0x8
; 0000 047C 	{
; 0000 047D 		WDR();
	wdr
; 0000 047E 		TimeOut += 2;
	__ADDWRN 16,17,2
; 0000 047F 		TimeOut -= 1;
	__SUBWRN 16,17,1
; 0000 0480 		if (TimeOut > 65530) break;
	__CPWRN 16,17,-5
	BRLO _0x6
; 0000 0481 	}
_0x8:
; 0000 0482 
; 0000 0483   return _UDR;
	IN   R30,0xC
	LD   R16,Y+
	LD   R17,Y+
	RET
; 0000 0484 }
;
;/*****************************************************************************/
;
;void TxChar(unsigned char ch)
; 0000 0489 {
_TxChar:
; 0000 048A   while(!(_UCSRA & 0x20)) WDR();      // wait for empty transmit buffer
;	ch -> Y+0
_0xA:
	SBIC 0xB,5
	RJMP _0xC
	wdr
; 0000 048B   #ifndef RS485DDR
; 0000 048C   _UDR = ch;                         // write char
	RJMP _0xA
_0xC:
	LD   R30,Y
	OUT  0xC,R30
; 0000 048D   #endif
; 0000 048E 
; 0000 048F   #ifdef RS485DDR
; 0000 0490   RS485PORT |= RS485TXE;            // RS485 in TX mode
; 0000 0491   _UDR = ch;                        // write char
; 0000 0492   while(!(_UCSRA & 0x40)) WDR();    // Wait for char to be cue off
; 0000 0493   _UCSRA |= 0x40;                   // Clear flag
; 0000 0494   RS485PORT &= ~RS485TXE;           // RS485 in RX mode
; 0000 0495   #endif
; 0000 0496 }
	ADIW R28,1
	RET
;
;/*****************************************************************************/
;
;void main(void)
; 0000 049B {
_main:
; 0000 049C   PULLUPPORT = PULLUPPIN;           // Pull up on RX line
	LDI  R30,LOW(1)
	OUT  0x12,R30
; 0000 049D 
; 0000 049E   //_UBRRH = ((XTAL / (16 * BAUDRATE)) - 1)>>8;
; 0000 049F  	_UBRRL = (XTAL / (16 * BAUDRATE)) - 1;      //set baud rate;
	LDI  R30,LOW(12)
	OUT  0x9,R30
; 0000 04A0 	_UCSRB = 0x18;                     // Rx enable Tx Enable
	LDI  R30,LOW(24)
	OUT  0xA,R30
; 0000 04A1 	_UCSRC = 0x86;                     // Asyn,NoParity,1StopBit,8Bit
	LDI  R30,LOW(134)
	OUT  0x20,R30
; 0000 04A2 
; 0000 04A3 	#ifdef RS485DDR
; 0000 04A4 	RS485DDR |= RS485TXE;             // RS485 Tranceiver switch pin as output
; 0000 04A5 	RS485PORT &= ~RS485TXE;           // RS485 in Rx mode
; 0000 04A6 	#endif
; 0000 04A7 
; 0000 04A8 	RxChar();
	RCALL _RxChar
; 0000 04A9 	TxChar('>');
	LDI  R30,LOW(62)
	ST   -Y,R30
	RCALL _TxChar
; 0000 04AA 	if (RxChar() == '<')
	RCALL _RxChar
	CPI  R30,LOW(0x3C)
	BRNE _0xD
; 0000 04AB 	{
; 0000 04AC 	    TxChar(PageSize);
	LDI  R30,LOW(83)
	ST   -Y,R30
	RCALL _TxChar
; 0000 04AD 		TxChar(DeviceID);
	LDI  R30,LOW(66)
	ST   -Y,R30
	RCALL _TxChar
; 0000 04AE     	TxChar(FlashSize);
	LDI  R30,LOW(109)
	ST   -Y,R30
	RCALL _TxChar
; 0000 04AF   	    TxChar(BootSize);
	LDI  R30,LOW(99)
	ST   -Y,R30
	RCALL _TxChar
; 0000 04B0   	    TxChar(EEpromSize);
	LDI  R30,LOW(49)
	ST   -Y,R30
	RCALL _TxChar
; 0000 04B1 
; 0000 04B2   	RxChar();
	RCALL _RxChar
; 0000 04B3   	TxChar('!');
	LDI  R30,LOW(33)
	ST   -Y,R30
	RCALL _TxChar
; 0000 04B4 
; 0000 04B5 		while (1)
_0xE:
; 0000 04B6 		{
; 0000 04B7 
; 0000 04B8 		 WDR();
	wdr
; 0000 04B9 		 GetPageNumber();
	RCALL _GetPageNumber
; 0000 04BA 
; 0000 04BB 		 if (RealPageAddress == 0xffff) break;
	LDI  R30,LOW(65535)
	LDI  R31,HIGH(65535)
	CP   R30,R6
	CPC  R31,R7
	BREQ _0x10
; 0000 04BC 
; 0000 04BD 		 if (WriteFlashPage())
	RCALL _WriteFlashPage
	CPI  R30,0
	BREQ _0x12
; 0000 04BE              {
; 0000 04BF              TxChar('!');
	LDI  R30,LOW(33)
	ST   -Y,R30
	RCALL _TxChar
; 0000 04C0              }
; 0000 04C1 		 else  break; //TxChar('@');
	RJMP _0x13
_0x12:
	RJMP _0x10
; 0000 04C2 
; 0000 04C3 		}
_0x13:
	RJMP _0xE
_0x10:
; 0000 04C4 
; 0000 04C5 		#ifdef EEPROM
; 0000 04C6 		EEpromLoad();
; 0000 04C7 		#endif
; 0000 04C8 		#ifdef LOCKBIT
; 0000 04C9 		LockBit();
; 0000 04CA 		#endif
; 0000 04CB 	}
; 0000 04CC 
; 0000 04CD   #ifdef RAMPZ_FLAG
; 0000 04CE   RAMPZ = 0;
; 0000 04CF   #endif
; 0000 04D0 
; 0000 04D1   #ifdef INTVECREG
; 0000 04D2   //MCUCR = (1<<IVCE);
; 0000 04D3  // MCUCR = 0x00;
; 0000 04D4  //maebe:
; 0000 04D5      //INTVECREG = (1<<0);
; 0000 04D6      //INTVECREG = 0x00;
; 0000 04D7   #endif
; 0000 04D8 
; 0000 04D9   #asm("jmp 0x0000");                // Run application code
_0xD:
	jmp 0x0000
; 0000 04DA }
_0x14:
	RJMP _0x14

	.CSEG

	.CSEG
__LSLW12:
	TST  R30
	MOV  R0,R30
	MOVW R30,R26
	BREQ __LSLW12R
__LSLW12L:
	LSL  R30
	ROL  R31
	DEC  R0
	BRNE __LSLW12L
__LSLW12R:
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
