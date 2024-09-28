PG Arcade picohub
=================================

Firmware for Raspberry Pi Pico

Creates two USB-CDC and one HID devices:

The first creates a bridge with UART.

The second disassembles the commands for controlling the LEDs. Works in conjunction with the [PD-Arcade-Lights plugin](https://github.com/steelpuxnastik/PD-Arcade-Lights)

HID GamePad for 5 buttons

Raspberry Pi Pico Pinout
------------------------

| Raspberry Pi Pico GPIO | Function |
|:----------------------:|:--------:|
| GPIO16 (Pin 21)        | UART0 TX |
| GPIO17 (Pin 22)        | UART0 RX |
|  --------------------  |  ------  |
| GPIO5 (Pin 7)          | BTN1 LED |
| GPIO4 (Pin 6)          | BTN2 LED |
| GPIO3 (Pin 5)          | BTN3 LED |
| GPIO2 (Pin 4)          | BTN4 LED |
| GPIO10 (Pin 14)        | L LINE R |
| GPIO11 (Pin 15)        | L LINE G |
| GPIO9 (Pin 12)         | L LINE B |
| GPIO7 (Pin 10)         | R LINE R |
| GPIO8 (Pin 11)         | R LINE G |
| GPIO6 (Pin 9)          | R LINE B |
|  --------------------  |  ------  |
| GPIO18 (Pin 24)        |  BTN 1   |
| GPIO19 (Pin 25)        |  BTN 2   |
| GPIO20 (Pin 26)        |  BTN 3   |
| GPIO21 (Pin 27)        |  BTN 4   |
| GPIO22 (Pin 29)        |  BTN 5   |

Disclaimer
----------

This software is provided without warranty, according to the MIT License, and should therefore not be used where it may endanger life, financial stakes, or cause discomfort and inconvenience to others.