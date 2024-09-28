// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2021 Álvaro Fernández Rojas <noltari@gmail.com>
 *
 * This file is based on a file originally part of the
 * MicroPython project, http://micropython.org/
 *
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 * Copyright (c) 2019 Damien P. George
 */

#include <hardware/flash.h>
#include <tusb.h>

#define DESC_STR_MAX 20

#define USBD_VID 0x2E8A /* Raspberry Pi */
#define USBD_PID 0x000A /* Raspberry Pi Pico SDK CDC */

#define USBD_DESC_LEN  (TUD_CONFIG_DESC_LEN + (CFG_TUD_CDC * TUD_CDC_DESC_LEN) + TUD_HID_DESC_LEN)
#define USBD_MAX_POWER_MA 500

enum
{
  ITF_NUM_CDC1 = 0,
  ITF_NUM_CDC1_DATA,
  ITF_NUM_CDC2,
  ITF_NUM_CDC2_DATA,
  ITF_NUM_HID,
  ITF_NUM_TOTAL
};


#define USBD_CDC_0_EP_CMD 0x81
#define USBD_CDC_1_EP_CMD 0x83

#define USBD_CDC_0_EP_OUT 0x02
#define USBD_CDC_1_EP_OUT 0x04

#define USBD_CDC_0_EP_IN 0x82
#define USBD_CDC_1_EP_IN 0x84

#define USBD_CDC_CMD_MAX_SIZE 8
#define USBD_CDC_IN_OUT_MAX_SIZE 64

#define USBD_CDC_EP      0x85

#define USBD_HID_IN_OUT_MAX_SIZE 10

#define USBD_STR_0 0x00
#define USBD_STR_MANUF 0x01
#define USBD_STR_PRODUCT 0x02
#define USBD_STR_SERIAL 0x03
#define USBD_STR_SERIAL_LEN 17
#define USBD_STR_CDC1 0x04
#define USBD_STR_CDC2 0x05
#define USBD_STR_HID 0x06


static const tusb_desc_device_t usbd_desc_device = {
	.bLength = sizeof(tusb_desc_device_t),
	.bDescriptorType = TUSB_DESC_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = TUSB_CLASS_MISC,
	.bDeviceSubClass = MISC_SUBCLASS_COMMON,
	.bDeviceProtocol = MISC_PROTOCOL_IAD,
	.bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
	.idVendor = USBD_VID,
	.idProduct = USBD_PID,
	.bcdDevice = 0x0100,
	.iManufacturer = USBD_STR_MANUF,
	.iProduct = USBD_STR_PRODUCT,
	.iSerialNumber = USBD_STR_SERIAL,
	.bNumConfigurations = 1,
};

uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_GAMEPAD()
};


static const uint8_t usbd_desc_cfg[USBD_DESC_LEN] = {
	TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, USBD_STR_0, USBD_DESC_LEN,
		TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, USBD_MAX_POWER_MA),

	TUD_CDC_DESCRIPTOR(ITF_NUM_CDC1, USBD_STR_CDC1, USBD_CDC_0_EP_CMD,
		USBD_CDC_CMD_MAX_SIZE, USBD_CDC_0_EP_OUT, USBD_CDC_0_EP_IN,
		USBD_CDC_IN_OUT_MAX_SIZE),

	TUD_CDC_DESCRIPTOR(ITF_NUM_CDC2, USBD_STR_CDC2, USBD_CDC_1_EP_CMD,
		USBD_CDC_CMD_MAX_SIZE, USBD_CDC_1_EP_OUT, USBD_CDC_1_EP_IN,
		USBD_CDC_IN_OUT_MAX_SIZE),

	// HID: Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
  	TUD_HID_DESCRIPTOR(ITF_NUM_HID, USBD_STR_HID, HID_ITF_PROTOCOL_NONE, 
		sizeof(desc_hid_report), USBD_CDC_EP, CFG_TUD_HID_EP_BUFSIZE, 
		USBD_HID_IN_OUT_MAX_SIZE)

};

static char usbd_serial[USBD_STR_SERIAL_LEN] = "000000000000";

static const char *const usbd_desc_str[] = {
	[USBD_STR_MANUF] = "Raspberry Pi",
	[USBD_STR_PRODUCT] = "Pico",
	[USBD_STR_SERIAL] = usbd_serial,
	[USBD_STR_CDC1] = "Bridge CDC",
	[USBD_STR_CDC2] = "LEDs CDC",
	[USBD_STR_HID] = "Buttons gamepad",
};

const uint8_t *tud_descriptor_device_cb(void)
{
	return (const uint8_t *) &usbd_desc_device;
}

const uint8_t *tud_descriptor_configuration_cb(uint8_t index)
{
	return usbd_desc_cfg;
}

const uint16_t *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
	static uint16_t desc_str[DESC_STR_MAX];
	uint8_t len;

	if (index == 0) {
		desc_str[1] = 0x0409;
		len = 1;
	} else {
		const char *str;
		char serial[USBD_STR_SERIAL_LEN];

		if (index >= sizeof(usbd_desc_str) / sizeof(usbd_desc_str[0]))
			return NULL;

		str = usbd_desc_str[index];
		for (len = 0; len < DESC_STR_MAX - 1 && str[len]; ++len)
			desc_str[1 + len] = str[len];
	}

	desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * len + 2);

	return desc_str;
}


uint8_t const * tud_hid_descriptor_report_cb(uint8_t itf)
{
  (void) itf;
  return desc_hid_report;
}

void usbd_serial_init(void)
{
	uint8_t id[8];

	flash_get_unique_id(id);

	snprintf(usbd_serial, USBD_STR_SERIAL_LEN, "%02X%02X%02X%02X%02X%02X%02X%02X",
		 id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]);
}
