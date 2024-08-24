#ifndef _USB_PROCESS_H_
#define _USB_PROCESS_H_

#include <hardware/irq.h>
#include <hardware/structs/sio.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <tusb.h>
#include "uart-process.h"

void usbProcessing(void);

#endif


