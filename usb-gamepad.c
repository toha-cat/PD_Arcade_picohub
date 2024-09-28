#include "usb-gamepad.h"
#include <tusb.h>
#include "tusb_config.h"

// Налаштування пінів для кнопок
#define BUTTON_PIN_1 18
#define BUTTON_PIN_2 19
#define BUTTON_PIN_3 20
#define BUTTON_PIN_4 21
#define BUTTON_PIN_5 22

hid_gamepad_report_t gamepadReport = {0};

void initGamepadGpios() {
    gpio_init(BUTTON_PIN_1);
    gpio_set_dir(BUTTON_PIN_1, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_1);

    gpio_init(BUTTON_PIN_2);
    gpio_set_dir(BUTTON_PIN_2, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_2);

    gpio_init(BUTTON_PIN_3);
    gpio_set_dir(BUTTON_PIN_3, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_3);

    gpio_init(BUTTON_PIN_4);
    gpio_set_dir(BUTTON_PIN_4, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_4);

    gpio_init(BUTTON_PIN_5);
    gpio_set_dir(BUTTON_PIN_5, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_5);
}

void gamepadTask() {
    gamepadReport.buttons = 0;
    if (!gpio_get(BUTTON_PIN_1)) gamepadReport.buttons |= (1 << 0);
    if (!gpio_get(BUTTON_PIN_2)) gamepadReport.buttons |= (1 << 1);
    if (!gpio_get(BUTTON_PIN_3)) gamepadReport.buttons |= (1 << 2);
    if (!gpio_get(BUTTON_PIN_4)) gamepadReport.buttons |= (1 << 3);
    if (!gpio_get(BUTTON_PIN_5)) gamepadReport.buttons |= (1 << 4);

    if (tud_hid_ready()) {
        tud_hid_report(0, &gamepadReport, sizeof(gamepadReport));
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    (void) itf;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void) itf;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}