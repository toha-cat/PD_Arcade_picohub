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

void ledSlidesUpdate(uint8_t inData) 
{
    gpio_put(ARCADE_LED_R1, (inData & 0b00000001));
    gpio_put(ARCADE_LED_G1, (inData & 0b00000010));
    gpio_put(ARCADE_LED_B1, (inData & 0b00000100));

    gpio_put(ARCADE_LED_R2, (inData & 0b00001000));
    gpio_put(ARCADE_LED_G2, (inData & 0b00010000));
    gpio_put(ARCADE_LED_B2, (inData & 0b00100000));
}

void ledBtnsUpdate(uint8_t inData) 
{
    gpio_put(ARCADE_LED_BTN1, (inData & 0b00000001));
    gpio_put(ARCADE_LED_BTN2, (inData & 0b00000010));
    gpio_put(ARCADE_LED_BTN3, (inData & 0b00000100));
    gpio_put(ARCADE_LED_BTN4, (inData & 0b00001000));
}

void ledInsertData(uint8_t *data, int32_t len)
{
    for (uint i = 0; i < len; i++)
	{
		uint8_t typeData = data[i] & 0b11000000;
		switch(typeData){
		case 0b00000000:
			ledSlidesUpdate(data[i]);
			break;
		case 0b11000000:
			ledBtnsUpdate(data[i]);
			break;
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