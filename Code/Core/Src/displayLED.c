/*
 * displayLED.c
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "global.h"
#include "displayLED.h"

void blink_debug_led(void) {
	HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
}

void blink_red_LEDs(void) {
	HAL_GPIO_TogglePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin);
	HAL_GPIO_TogglePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin);
}
void blink_amber_LEDs(void) {
	HAL_GPIO_TogglePin(LED_AMBER_0_GPIO_Port, LED_AMBER_0_Pin);
	HAL_GPIO_TogglePin(LED_AMBER_1_GPIO_Port, LED_AMBER_1_Pin);
}
void blink_green_LEDs(void) {
	HAL_GPIO_TogglePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin);
	HAL_GPIO_TogglePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin);
}

void set_red_LEDs(int status_0, int status_1) {
	HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, status_0);
	HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, status_1);
}

void set_amber_LEDs(int status_0, int status_1) {
	HAL_GPIO_WritePin(LED_AMBER_0_GPIO_Port, LED_AMBER_0_Pin, status_0);
	HAL_GPIO_WritePin(LED_AMBER_1_GPIO_Port, LED_AMBER_1_Pin, status_1);
}

void set_green_LEDs(int status_0, int status_1) {
	HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, status_0);
	HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, status_1);
}

void clear_all_LEDs(void) {
	set_red_LEDs(OFF, OFF);
	set_amber_LEDs(OFF, OFF);
	set_green_LEDs(OFF, OFF);
}
