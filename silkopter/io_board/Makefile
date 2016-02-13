#FUSES      = -U lfuse:w:0xe2:m -U hfuse:w:0xda:m -U efuse:w:0xff:m

NO_ARDUINO = 1

AVRDUDE_PORT = /dev/ttyACM0
BAUDRATE = 57600
AVRDUDE_PROGRAMMER = avr109
MCU=atmega32u4
F_CPU=16000000L


# This is the name of the file that shall be created. (It is also the name of my primary source file, without the file extension.)
TARGET = io_board

# "EXTRAINCDIRS" is a variable used by the base makefile. The base makefile creates a -I compiler flag for every item in the "EXTRAINCDIRS" list.
EXTRAINCDIRS = src src/stl

# specify *.c source files pertaining to my project
SRC =

# specify *.cpp source files pertaining to my project
PSRC = src/main.cpp src/avr_stdio.cpp src/I2C_slave.cpp

# specify additional (non-core) Arduino libraries to include
#ARDLIBS = SoftwareSerial

# include my base makefile
include Makefile.base
