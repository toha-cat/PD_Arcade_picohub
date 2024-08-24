#ifndef _LEDS_H_
#define _LEDS_H_

#include <hardware/structs/sio.h>
#include <pico/stdlib.h>

void ledInsertData(uint8_t *data, int32_t len);
void ledSetStatus(int stat);
void initLedGpios();

#endif