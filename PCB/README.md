PD Arcade picohub
=================================

Firmware for Raspberry Pi Pico for use in Hatsune Miku Project DIVA Arcade Future Tone DIY-arcade controller 

Creates two USB-CDC and one HID devices:

The first creates UART-bridge for connection [LKP](https://github.com/Project-Alpaca/LKP)-Slider with [LKP-Serial](https://github.com/Project-Alpaca/LKP-FW) firmware.

The second disassembles the commands for controlling the LEDs. Works in conjunction with the [PD-Arcade-Lights plugin](https://github.com/steelpuxnastik/PD-Arcade-Lights)

HID-Gamepad for 5 buttons.

Raspberry Pi Pico Pinout
------------------------

| Raspberry Pi Pico GPIO | Function |
|-------------|-------------|
| 			[LKP-Slider](https://github.com/Project-Alpaca/LKP) connection				|
|-------------|-------------|
| GPIO16 (Pin 21)        | UART0 TX |
| GPIO17 (Pin 22)        | UART0 RX |
|-------------|-------------|
| LEDs      |
|-------------|-------------|
| GPIO2 (Pin 4)          | BTN4 ![Circle](/images/circle.png) LED (Circle) |
| GPIO3 (Pin 5)          | BTN3 ![Cross](/images/cross.png) LED (Cross) |
| GPIO4 (Pin 6)          | BTN2 ![Square](/images/square.png) LED (Square) |
| GPIO5 (Pin 7)          | BTN1 ![Triangle](/images/triangle.png) LED (Triangle) |
| GPIO6 (Pin 9)          | R LINE B (RGB LED strip)|
| GPIO7 (Pin 10)         | R LINE R (RGB LED strip)|
| GPIO8 (Pin 11)         | R LINE G (RGB LED strip)|
| GPIO9 (Pin 12)         | L LINE B (RGB LED strip)|
| GPIO10 (Pin 14)        | L LINE R (RGB LED strip)|
| GPIO11 (Pin 15)        | L LINE G (RGB LED strip)|
| GPIO15 (Pin 20)        | LED-strip brightness PWM signal source|
|-------------			|-------------|
| 			Controls	 			|
|-------------			|-------------|
| GPIO18 (Pin 24)        |  BTN 1 ![Triangle](/images/triangle.png) (Triangle) |
| GPIO19 (Pin 25)        |  BTN 2 ![Square](/images/square.png) (Square) |
| GPIO20 (Pin 26)        |  BTN 3 ![Cross](/images/cross.png) (Cross) |
| GPIO21 (Pin 27)        |  BTN 4 ![Circle](/images/circle.png) (Circle) |
| GPIO22 (Pin 29)        |  BTN 5 (Start)  |

Disclaimer
----------

This software is provided without warranty, according to the MIT License, and should therefore not be used where it may endanger life, financial stakes, or cause discomfort and inconvenience to others.
