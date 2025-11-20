/*
 * input_reading.c
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "input_reading.h"

#define		N0_OF_BUTTONS				4
#define		DURATION_FOR_AUTO_INCREASE	100
#define		BUTTON_IS_PRESSED			GPIO_PIN_RESET
#define		BUTTON_IS_RELEASED			GPIO_PIN_SET

typedef enum ButtonState {
	BUTTON_RELEASED,
	BUTTON_PRESSED,
	BUTTON_PRESSED_MORE_THAN_1S
} ButtonState_t;

static GPIO_PinState buttonBuffer[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS];

static uint8_t flagForButtonPress1s[N0_OF_BUTTONS];
static uint16_t counterForButtonPress1s[N0_OF_BUTTONS];

void button_reading(void) {
	for (int button_id = 0; button_id < N0_OF_BUTTONS; ++button_id) {

		debounceButtonBuffer2[button_id] = debounceButtonBuffer1[button_id];

		switch (button_id) {
			case 0: debounceButtonBuffer1[button_id] = HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port, BUTTON_1_Pin); break;
			case 1: debounceButtonBuffer1[button_id] = HAL_GPIO_ReadPin(BUTTON_2_GPIO_Port, BUTTON_2_Pin); break;
			case 2: debounceButtonBuffer1[button_id] = HAL_GPIO_ReadPin(BUTTON_3_GPIO_Port, BUTTON_3_Pin); break;
			case 3: debounceButtonBuffer1[button_id] = HAL_GPIO_ReadPin(BUTTON_4_GPIO_Port, BUTTON_4_Pin); break;
			default:
				break;
		}

		if (debounceButtonBuffer1[button_id] == debounceButtonBuffer2[button_id]) {

			buttonBuffer[button_id] = debounceButtonBuffer1[button_id];

			if (buttonBuffer[button_id] == BUTTON_IS_PRESSED) {
				if (flagForButtonPress1s[button_id] == 0) {
					--counterForButtonPress1s[button_id];
				}
				if (counterForButtonPress1s[button_id] <= 0) {
					flagForButtonPress1s[button_id] = 1;
					counterForButtonPress1s[button_id] = DURATION_FOR_AUTO_INCREASE;
				}
			} else { // Reset
				counterForButtonPress1s[button_id] = DURATION_FOR_AUTO_INCREASE;
				flagForButtonPress1s[button_id] = 0;
			}
		}
	}
}

unsigned char is_button_pressed(unsigned char index) {
	if (index >= N0_OF_BUTTONS) return 0;
	return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
	if (index >= N0_OF_BUTTONS) return 0;
	return (flagForButtonPress1s[index] == 1);
}
