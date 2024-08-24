#include "usb-process.h"
#include "tusb_config.h"
#include "leds.h"


#ifndef MIN
#define MIN(a, b) ((a > b) ? b : a)
#endif /* MIN */

#define USB_CDC_UART_N 0
#define USB_CDC_LED_N 1

uint8_t usbLedBuffer[BUFFER_SIZE];
uint32_t usbLedBuffPos;

void usbReadBytesToUart()
{
	uint32_t len = tud_cdc_n_available(USB_CDC_UART_N);

	if (len && mutex_try_enter(&uartDataStor.usbMtx, NULL)) {

		len = MIN(len, BUFFER_SIZE - uartDataStor.usbPos);
		if (len) {
			uint32_t count;

			count = tud_cdc_n_read(USB_CDC_UART_N, &uartDataStor.usbBuffer[uartDataStor.usbPos], len);
			uartDataStor.usbPos += count;
		}

		mutex_exit(&uartDataStor.usbMtx);
	}
}

void usbWriteBytesFromUart()
{
	if (uartDataStor.uartPos && mutex_try_enter(&uartDataStor.uartMtx, NULL)) {
		uint32_t count;

		count = tud_cdc_n_write(USB_CDC_UART_N, uartDataStor.uartBuffer, uartDataStor.uartPos);
		if (count < uartDataStor.uartPos){
			memmove(uartDataStor.uartBuffer, &uartDataStor.uartBuffer[count], uartDataStor.uartPos - count);
		}
		uartDataStor.uartPos -= count;

		mutex_exit(&uartDataStor.uartMtx);

		if (count){
			tud_cdc_n_write_flush(USB_CDC_UART_N);
		}
	}
}

void usbCdcToUartProcess()
{
	mutex_enter_blocking(&uartDataStor.lcMtx);
	tud_cdc_n_get_line_coding(USB_CDC_UART_N, &uartDataStor.usbLc);
	mutex_exit(&uartDataStor.lcMtx);

	usbReadBytesToUart();
	usbWriteBytesFromUart();
}



void usbCdcToLedProcess()
{
	uint32_t len = tud_cdc_n_available(USB_CDC_LED_N);

	if (len) {
		len = MIN(len, BUFFER_SIZE - usbLedBuffPos);
		if (len) {
			uint32_t count = tud_cdc_n_read(USB_CDC_UART_N, usbLedBuffer, len);
			usbLedBuffPos += count;
		}
		ledInsertData(usbLedBuffer, len);
	}

}

void usbProcessing(void)
{
	tusb_init();

	while (1) {
		int itf;
		int con = 0;

		tud_task();

		// USB-CDC to UART
		if (tud_cdc_n_connected(USB_CDC_UART_N)) {
			con = 1;
			usbCdcToUartProcess();
		}

		// USB-CDC to LED
		if (tud_cdc_n_connected(USB_CDC_LED_N)) {
			con = 1;
			usbCdcToLedProcess();
		}

		ledSetStatus(con);
	}
}