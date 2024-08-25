#include "uart-process.h"

#define DEF_BIT_RATE 115200
#define DEF_STOP_BITS 1
#define DEF_PARITY 0
#define DEF_DATA_BITS 8

#define UART_IRQ UART0_IRQ
#define UART_TX_PIN 16
#define UART_RX_PIN 17


uart_inst_t *const uartInst = uart0;

uart_data_t uartDataStor;

static inline uint databitsFilter(uint8_t dataBits)
{
	switch (dataBits) {
		case 5:
			return 5;
		case 6:
			return 6;
		case 7:
			return 7;
		default:
			return 8;
	}
}

static inline uart_parity_t parityFilter(uint8_t usbParity)
{
	switch (usbParity) {
		case 1:
			return UART_PARITY_ODD;
		case 2:
			return UART_PARITY_EVEN;
		default:
			return UART_PARITY_NONE;
	}
}

static inline uint stopbitsFilter(uint8_t stopBits)
{
	switch (stopBits) {
		case 2:
			return 2;
		default:
			return 1;
	}
}


void uartUpdateCfg()
{
	mutex_enter_blocking(&uartDataStor.lcMtx);

	if (uartDataStor.usbLc.bit_rate != uartDataStor.uartLc.bit_rate) {
		uart_set_baudrate(uartInst, uartDataStor.usbLc.bit_rate);
		uartDataStor.uartLc.bit_rate = uartDataStor.usbLc.bit_rate;
	}

	if ((uartDataStor.usbLc.stop_bits != uartDataStor.uartLc.stop_bits) ||
	    (uartDataStor.usbLc.parity != uartDataStor.uartLc.parity) ||
	    (uartDataStor.usbLc.data_bits != uartDataStor.uartLc.data_bits)) {
		uart_set_format(uartInst,
				databitsFilter(uartDataStor.usbLc.data_bits),
				stopbitsFilter(uartDataStor.usbLc.stop_bits),
				parityFilter(uartDataStor.usbLc.parity));
		uartDataStor.uartLc.data_bits = uartDataStor.usbLc.data_bits;
		uartDataStor.uartLc.parity = uartDataStor.usbLc.parity;
		uartDataStor.uartLc.stop_bits = uartDataStor.usbLc.stop_bits;
	}

	mutex_exit(&uartDataStor.lcMtx);
}

// handler new data from uart
void uartIrqHandler(void)
{
	if (uart_is_readable(uartInst)) {
		mutex_enter_blocking(&uartDataStor.uartMtx);

		while (uart_is_readable(uartInst) && (uartDataStor.uartPos < BUFFER_SIZE)) {
			uartDataStor.uartBuffer[uartDataStor.uartPos] = uart_getc(uartInst);
			uartDataStor.uartPos++;
		}

		mutex_exit(&uartDataStor.uartMtx);
	}
}

// send date from uartDataStor buffer to uart
void uartWriteBytes()
{
	if (uartDataStor.usbPos && mutex_try_enter(&uartDataStor.usbMtx, NULL)) {
		uint32_t count = 0;

		while (uart_is_writable(uartInst) && count < uartDataStor.usbPos) {
			uart_putc_raw(uartInst, uartDataStor.usbBuffer[count]);
			count++;
		}

		if (count < uartDataStor.usbPos){
			memmove(uartDataStor.usbBuffer, &uartDataStor.usbBuffer[count], uartDataStor.usbPos - count);
		}
		uartDataStor.usbPos -= count;

		mutex_exit(&uartDataStor.usbMtx);
	}
}

// init uart data storage
void initUartData()
{
	/* USB CDC LC */
	uartDataStor.usbLc.bit_rate = DEF_BIT_RATE;
	uartDataStor.usbLc.data_bits = DEF_DATA_BITS;
	uartDataStor.usbLc.parity = DEF_PARITY;
	uartDataStor.usbLc.stop_bits = DEF_STOP_BITS;

	/* UART LC */
	uartDataStor.uartLc.bit_rate = DEF_BIT_RATE;
	uartDataStor.uartLc.data_bits = DEF_DATA_BITS;
	uartDataStor.uartLc.parity = DEF_PARITY;
	uartDataStor.uartLc.stop_bits = DEF_STOP_BITS;

	/* Buffer */
	uartDataStor.uartPos = 0;
	uartDataStor.usbPos = 0;

	/* Mutex */
	mutex_init(&uartDataStor.lcMtx);
	mutex_init(&uartDataStor.uartMtx);
	mutex_init(&uartDataStor.usbMtx);
}

// init uart hardware and IRQ
void initUartHardware()
{
	/* Pinmux */
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
	gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

	/* UART start */
	uart_init(uartInst, uartDataStor.usbLc.bit_rate);
	uart_set_hw_flow(uartInst, false, false);
	uart_set_format(uartInst, databitsFilter(uartDataStor.usbLc.data_bits),
			stopbitsFilter(uartDataStor.usbLc.stop_bits),
			parityFilter(uartDataStor.usbLc.parity));
	uart_set_fifo_enabled(uartInst, false);

	/* UART RX Interrupt */
	irq_set_exclusive_handler(UART_IRQ, &uartIrqHandler);
	irq_set_enabled(UART_IRQ, true);
	uart_set_irq_enables(uartInst, true, false);
}