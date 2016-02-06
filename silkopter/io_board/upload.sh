#! /bin/sh

stty -F /dev/ttyACM0 1200
sleep 1
make program

