#include "leds.h"

// GPIO number (not pins)
#define ARCADE_LED_BTN1 5  // BUTTON LED TRIANGLE
#define ARCADE_LED_BTN2 4  // BUTTON LED SQUARE
#define ARCADE_LED_BTN3 3  // BUTTON LED CROSS
#define ARCADE_LED_BTN4 2  // BUTTON LED CIRCLE
#define ARCADE_LED_R1 10  // LEFT PARTITION LED RED
#define ARCADE_LED_G1 11  // LEFT PARTITION LED GREEN
#define ARCADE_LED_B1 9  // LEFT PARTITION LED BLUE
#define ARCADE_LED_R2 7  // RIGHT PARTITION LED RED
#define ARCADE_LED_G2 8 // RIGHT PARTITION LED GREEN
#define ARCADE_LED_B2 6 // RIGHT PARTITION LED BLUE

#define ARCADE_COUNT_LED 10
uint arcadeLeds[ARCADE_COUNT_LED] = {
	ARCADE_LED_BTN1,
	ARCADE_LED_BTN2,
	ARCADE_LED_BTN3,
	ARCADE_LED_BTN4,
	ARCADE_LED_R1,
	ARCADE_LED_G1,
	ARCADE_LED_B1,
	ARCADE_LED_R2,
	ARCADE_LED_G2,
	ARCADE_LED_B2
};

#define LED_CONN_STAT 25


#define LED_SYNC_CODE 0xff
#define ledDataSize 3
uint8_t ledDataIteration = 0;
uint8_t ledData[ledDataSize];

inline void ledDataClear() {
    ledDataIteration = 0;
    ledData[0] = 0;
}

void ledUpdate() {
    //BUTTONS 
    gpio_put(ARCADE_LED_BTN1, (ledData[1] & 0b00000001));
    gpio_put(ARCADE_LED_BTN2, (ledData[1] & 0b00000010));
    gpio_put(ARCADE_LED_BTN3, (ledData[1] & 0b00000100));
    gpio_put(ARCADE_LED_BTN4, (ledData[1] & 0b00001000));

    //SIDES
    gpio_put(ARCADE_LED_R1, (ledData[2] & 0b00000001));
    gpio_put(ARCADE_LED_G1, (ledData[2] & 0b00000010));
    gpio_put(ARCADE_LED_B1, (ledData[2] & 0b00000100));

    gpio_put(ARCADE_LED_R2, (ledData[2] & 0b00001000));
    gpio_put(ARCADE_LED_G2, (ledData[2] & 0b00010000));
    gpio_put(ARCADE_LED_B2, (ledData[2] & 0b00100000));
      
    ledDataClear();
}

void ledInsertData(uint8_t *data, int32_t len)
{
    for (uint i = 0; i < len; i++)
	{
		ledData[ledDataIteration] = data[i];
		ledDataIteration++;
		if(ledDataIteration > 2){
			ledUpdate();
		}
		if(ledData[0] != LED_SYNC_CODE){
			ledDataClear();
		}
	}
}

void ledSetStatus(int stat)
{
    gpio_put(LED_CONN_STAT, stat);
}

void initLedGpios()
{
	gpio_init(LED_CONN_STAT);
	gpio_set_dir(LED_CONN_STAT, GPIO_OUT);

	for (int i = 0; i < ARCADE_COUNT_LED; i++)
	{
		gpio_init(arcadeLeds[i]);
		gpio_set_dir(arcadeLeds[i], GPIO_OUT);
	}
}