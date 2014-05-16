avrdude.exe -p m16 -c avr109 -P COM2 -U flash:w:_GCC-RTOS.hex:a
@echo off 
color 0A
echo ATmega16 Burned ok!
pause