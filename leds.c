#include "leds.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

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

#define ARCADE_LED_PWM_PIN 15 // PWM out pin for brightness
#define ARCADE_LED_PWM_WRAP 12500

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

inline uint readBite(uint data, uint nBite)
{
	return (data >> nBite) & 0b00000001;
}

void ledSlidesUpdate(uint8_t inData) 
{
    gpio_put(ARCADE_LED_R1, readBite(inData, 0));
    gpio_put(ARCADE_LED_G1, readBite(inData, 1));
    gpio_put(ARCADE_LED_B1, readBite(inData, 2));

    gpio_put(ARCADE_LED_R2, readBite(inData, 3));
    gpio_put(ARCADE_LED_G2, readBite(inData, 4));
    gpio_put(ARCADE_LED_B2, readBite(inData, 5));
}

void ledBtnsUpdate(uint8_t inData) 
{
    gpio_put(ARCADE_LED_BTN1, readBite(inData, 0));
    gpio_put(ARCADE_LED_BTN2, readBite(inData, 1));
    gpio_put(ARCADE_LED_BTN3, readBite(inData, 2));
    gpio_put(ARCADE_LED_BTN4, readBite(inData, 3));
}

void ledBrightnessUpdate(uint8_t inData) 
{
	const int maxInputValue = 50;
    uint16_t levelPer = inData & 0b00111111;

	if(levelPer > maxInputValue){
		levelPer = maxInputValue;
	}

	uint slice_num = pwm_gpio_to_slice_num(ARCADE_LED_PWM_PIN);
    uint chan = pwm_gpio_to_channel(ARCADE_LED_PWM_PIN);

	uint16_t level = levelPer * ARCADE_LED_PWM_WRAP / maxInputValue;
    pwm_set_chan_level(slice_num, chan, level);

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
		case 0b01000000:
			// set brightness
			ledBrightnessUpdate(data[i]);
			break;
		}
	}
}

void ledSetStatus(int stat)
{
    gpio_put(LED_CONN_STAT, stat);
}

void initLedPwm(void) {
    // set GPIO to PWM
    gpio_set_function(ARCADE_LED_PWM_PIN, GPIO_FUNC_PWM);
    
    // get slice/channel for GPIO
    uint slice_num = pwm_gpio_to_slice_num(ARCADE_LED_PWM_PIN);
    uint chan = pwm_gpio_to_channel(ARCADE_LED_PWM_PIN);
    
    // setup PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config,  ARCADE_LED_PWM_WRAP);
    pwm_init(slice_num, &config, true);
    
    // set the initial level to 100%
    pwm_set_chan_level(slice_num, chan,  ARCADE_LED_PWM_WRAP);
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

	initLedPwm();
}