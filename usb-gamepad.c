#include "usb-gamepad.h"
#include <tusb.h>
#include "tusb_config.h"
#include "pico/time.h"
#include "bsp/board.h"

// Button pins
#define BUTTON_PIN_1 18
#define BUTTON_PIN_2 19
#define BUTTON_PIN_3 20
#define BUTTON_PIN_4 21
#define BUTTON_PIN_5 22

typedef struct __attribute__((packed)) {
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t z;
    int8_t rx;
} hid_gamepad_report_win_t;

hid_gamepad_report_win_t gamepadReport = {0};

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
/*
uint32_t board_millis(void) {
    return to_ms_since_boot(get_absolute_time());
}
*/
void gamepadTask() {

    const uint32_t interval_ms = 1;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms) return;  // not enough time
    start_ms += interval_ms;

    gamepadReport.buttons = 0;
    if (!gpio_get(BUTTON_PIN_1)) gamepadReport.buttons |= (1 << 0);
    if (!gpio_get(BUTTON_PIN_2)) gamepadReport.buttons |= (1 << 1);
    if (!gpio_get(BUTTON_PIN_3)) gamepadReport.buttons |= (1 << 2);
    if (!gpio_get(BUTTON_PIN_4)) gamepadReport.buttons |= (1 << 3);
    if (!gpio_get(BUTTON_PIN_5)) gamepadReport.buttons |= (1 << 4);

    gamepadReport.x = 0;
    gamepadReport.y = 0;
    gamepadReport.z = 0;
    gamepadReport.rx = 0;

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