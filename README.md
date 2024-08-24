PG Arcade picohub
=================================

Firmware for Raspberry Pi Pico

Creates two USB-CDC devices:

The first creates a bridge with UART.

The second disassembles the commands for controlling the LEDs. Works in conjunction with the [PD-Arcade-Lights plugin](https://github.com/COM14/PD-Arcade-Lights)

Raspberry Pi Pico Pinout
------------------------

| Raspberry Pi Pico GPIO | Function |
|:----------------------:|:--------:|
| GPIO16 (Pin 21)        | UART0 TX |
| GPIO17 (Pin 22)        | UART0 RX |
|  --------------------  |  ------  |
| GPIO4 (Pin 2)          | BTN1 LED |
| GPIO5 (Pin 3)          | BTN2 LED |
| GPIO5 (Pin 4)          | BTN3 LED |
| GPIO5 (Pin 5)          | BTN4 LED |
| GPIO5 (Pin 6)          | L LINE R |
| GPIO5 (Pin 7)          | L LINE G |
| GPIO5 (Pin 8)          | L LINE B |
| GPIO5 (Pin 9)          | R LINE R |
| GPIO5 (Pin 10)         | R LINE G |
| GPIO5 (Pin 11)         | R LINE B |

Disclaimer
----------

This software is provided without warranty, according to the MIT License, and should therefore not be used where it may endanger life, financial stakes, or cause discomfort and inconvenience to others.