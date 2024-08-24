#ifndef _UART_PROCESS_H_
#define _UART_PROCESS_H_

#include <hardware/irq.h>
#include <hardware/structs/sio.h>
#include <hardware/uart.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <tusb.h>

#define BUFFER_SIZE 2560

typedef struct {
	cdc_line_coding_t usbLc;
	cdc_line_coding_t uartLc;
	mutex_t lcMtx;
	uint8_t uartBuffer[BUFFER_SIZE];
	uint32_t uartPos;
	mutex_t uartMtx;
	uint8_t usbBuffer[BUFFER_SIZE];
	uint32_t usbPos;
	mutex_t usbMtx;
} uart_data_t;

extern uart_data_t uartDataStor;


void uartUpdateCfg();
void uartWriteBytes();
void initUartData();
void initUartHardware();

#endif


