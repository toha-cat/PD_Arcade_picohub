#ifndef _USB_GAMEPAD_H_
#define _USB_GAMEPAD_H_

#include <hardware/structs/sio.h>
#include <pico/stdlib.h>


void initGamepadGpios();
void gamepadTask();

#endif


