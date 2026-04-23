# Project Description: ESP32-S3 LED Blink

## Overview
This project is a basic ESP32-S3 program used to test that the board is working correctly by blinking an LED.

## Function
The program turns the LED on and off 3 times, then stops.

## Behavior
- LED blinks 3 times
- Each blink consists of:
  - LED ON
  - short delay
  - LED OFF
  - short delay
- After the third blink, the program ends or remains idle

## Purpose
This project is useful for:
- verifying that the ESP32-S3 is programmed correctly
- testing LED control on a GPIO pin
- confirming that the development environment is working

## Example Use Case
A simple hardware test after flashing firmware to an ESP32-S3 board.

## Notes
- The LED can be the built-in onboard LED or an external LED connected to a GPIO pin
- The delay time can be adjusted as needed

