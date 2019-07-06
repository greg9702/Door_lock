# __Door lock__

### __Project overview__
Project of door lock made in C. <br>
Used Arduino Uno - Atmega328P and WiciuBoard (small PCB board with 4x 8 segment display, keyboard and LEDs). <br>
Everything is made using interrupts.

### __How it works.__
User have to input 4-char long password. <br>
The display show '-' symbols subtend to every number entered. <br>
User can _Cancel_ action or _Delete_ last entered character. <br>
After confirmation, the system display response to user _OK_ if password was correct <br>
or _BAD_ if entered password was incorrect. <br>
After couple seconds, system restart itself. <br>
Correct password is set directly in code.

### __Demo:__
Demo is [here](https://i.imgur.com/Q1uOY2U.gifv).

### __Pins__

#### Keyboard:

**PB(0-3)** - columns 4-1

**PC(4-5)** - rows 2-1

**PB(4-5)** - rows 4-3

**PC(0-3)** - anode 1-4

#### Display

**PD(0-7)** - a-g
