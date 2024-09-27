#include "usb-process.h"
#include "tusb_config.h"
#include "leds.h"


#ifndef MIN
#define MIN(a, b) ((a > b) ? b : a)
#endif /* MIN */

#define USB_CDC_UART_N 0
#define USB_CDC_LED_N 1

uint8_t usbLedBuffer[BUFFER_SIZE];

// read data from CDC_0 to buffer
void usbReadBytesToUart()
{
	uint32_t len = tud_cdc_n_available(USB_CDC_UART_N);

	if(len == 0){
		// no data
		return;
	}

	if(!mutex_try_enter(&uartDataStor.usbMtx, NULL)){
		// error create mutex
		return;
	}

	// if all the data is not placed in the buffer, then we take only a part
	len = MIN(len, BUFFER_SIZE - uartDataStor.usbPos);
	if (len) {
		uint32_t count = tud_cdc_n_read(USB_CDC_UART_N, &uartDataStor.usbBuffer[uartDataStor.usbPos], len);
		uartDataStor.usbPos += count;
	}

	mutex_exit(&uartDataStor.usbMtx);
}

// sending data from the buffer to the CDC_0
void usbWriteBytesFromUart()
{
	if(uartDataStor.uartPos == 0){
		// no data
		return;
	}
	if(!mutex_try_enter(&uartDataStor.uartMtx, NULL)){
		// error create mutex
		return;
	}
	
	uint32_t count = tud_cdc_n_write(USB_CDC_UART_N, uartDataStor.uartBuffer, uartDataStor.uartPos);
	
	uartDataStor.uartPos -= count;
	// if not all data is sent, then the remainder is transferred to the beginning of the buffer
	if (uartDataStor.uartPos > 0){
		memmove(uartDataStor.uartBuffer, &uartDataStor.uartBuffer[count], uartDataStor.uartPos);
	}

	mutex_exit(&uartDataStor.uartMtx);

	if (count){
		tud_cdc_n_write_flush(USB_CDC_UART_N);
	}
}

void usbCdcToUartProcess()
{
	// get uart gfg
	mutex_enter_blocking(&uartDataStor.lcMtx);
	tud_cdc_n_get_line_coding(USB_CDC_UART_N, &uartDataStor.usbLc);
	mutex_exit(&uartDataStor.lcMtx);

	// move data
	usbReadBytesToUart();
	usbWriteBytesFromUart();
}


// get data from CDC_1 for LED
void usbCdcToLedProcess()
{
	uint32_t len = tud_cdc_n_available(USB_CDC_LED_N);
	if(len == 0){
		// no data
		return;
	}
	if(len > BUFFER_SIZE){
		len = BUFFER_SIZE;
	}
	uint32_t count = tud_cdc_n_read(USB_CDC_LED_N, usbLedBuffer, len);
	ledInsertData(usbLedBuffer, count);
}

void usbProcessing(void)
{
	if (!tusb_init()) {
        // USB initialization error
        return;
    }

	while (1) {
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