# power_supply

Simple build for a Breadboard power supply.

## Major hardware
* STM32F401 MCU.
* VIM-878-336640 LCD.
* BU91797MUF-ME2 LCd Driver.
* tps63070 Voltage regulator.

## Details 
The power supply is meant to provide voltage in the 1V - 8V DC range to a user breadboard.
It uses either a USB type B connection or a power jack connector not to exceed 16V.  The 
tps63070 is rated to supply up to 2 amps of current depending on the input and output 
voltages (see Docs->ICs for datasheet). 

## Documents 
Attached is the basic code written for the STM32Ide version 1.11 using the HAL library.
The driver for the LCD display, along with the schematics for the board (for KiCad) and relevant datasheets. 
