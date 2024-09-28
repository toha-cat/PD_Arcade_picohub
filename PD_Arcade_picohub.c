#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <tusb.h>

#include "uart-process.h"
#include "usb-process.h"
#include "leds.h"
#include "usb-gamepad.h"


int main(void)
{
	usbd_serial_init();

	// init UART
	initUartData();
	initUartHardware();

	// init all led gpio
	initLedGpios();
	// init btns gpio
	initGamepadGpios();

	// processing usb-cdc to second core
	multicore_launch_core1(usbProcessing);

	while (1) {
		uartUpdateCfg();
		uartWriteBytes();
	}

	return 0;
}
