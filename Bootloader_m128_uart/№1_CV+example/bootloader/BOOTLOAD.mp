NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   7C7C   0276 =    630. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       3E3E  __text_start
       3E3E  __start
       3E64  _exit
       3E65  _write_page
       3E6B  _fill_temp_buffer
       3E73  _read_program_memory
       3E7A  _write_lock_bits
       3E80  _enableRWW
       3E8B  _GetPageNumber
       3EA0  _WriteFlashPage
       3F03  _RxChar
       3F1F  _TxChar
       3F28  _main
       3F63  pop_xgsetF0FC
       3F6E  push_xgsetF0FC
       3F70  push_xgset30FC
       3F72  push_xgset00FC
       3F74  push_xgset003C
       3F79  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   0100   0104 =    260. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0100  _RealPageAddress
       0100  __bss_start
       0102  _PageAddress
       0104  _PageBuffer
       0204  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   7C00   0004 =      4. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\progra~1\iccavr\lib\crtboot.o	[ crtboot.s ]
assembly.o	[ assembly.s ]
main.o	[ main.c ]
<library>	[ gpop5.s, gpush.s ]

User Global Definitions

ram_end = 0x8ff
hwstk_size = 0x10

User Base Address Definitions

vector = 0x7c00
func_lit = 0x7c7c
data = 0x100
eeprom:0.1024

