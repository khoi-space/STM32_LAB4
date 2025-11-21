/*
 * fsm_automatic.c
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "global.h"
#include "display7SEG.h"
#include "displayLED.h"
#include "fsm_automatic.h"
#include "fsm_manual.h"
#include "fsm_config.h"
#include "input_reading.h"
#include "scheduler.h"

static uint8_t red_counter = 5;
static uint8_t amber_counter = 2;
static uint8_t green_counter = 3;

static uint8_t time_route0 = 0;
static uint8_t time_route1 = 0;

#define COUNTER_CYCLE	10 //1000ms
static uint8_t counter = COUNTER_CYCLE;

void initAuto(void) {
	clear_all_LEDs();
	clear_all_7seg_en();

	red_counter = red_counter_buffer;
	amber_counter = amber_counter_buffer;
	green_counter = green_counter_buffer;

	mode = AUTO_MODE;
	status = AUTO_DIR0_GREEN;
}

void fsm_automatic_run(void) {
	if (mode != AUTO_MODE) return;

	// BUTTON_MODE: change mode to manual mode
	if (is_button_pressed(BUTTON_MODE) && is_mode_button_locked == 0) {
		is_mode_button_locked = 1;

		initMan(); // Prepare resources for manual mode

		return;
	} else if (!is_button_pressed(BUTTON_MODE)) {
		is_mode_button_locked = 0;
	}

	// BUTTON_RESET (long press): reset state
	if (is_button_pressed_1s(BUTTON_RESET)) {
		initAuto();
	}

	// Use counter to increase delay to 1000ms
	if (counter <= 0) {
		counter = COUNTER_CYCLE;
		switch(status) {
			case INIT:
				red_counter = red_counter_buffer;
				amber_counter = amber_counter_buffer;
				green_counter = green_counter_buffer;
				status = AUTO_DIR0_GREEN;

				time_route0 = green_counter;
				time_route1 = green_counter + amber_counter;

			case AUTO_DIR0_GREEN:
				set_red_LEDs(OFF , ON);
				set_amber_LEDs(OFF, OFF);
				set_green_LEDs(ON, OFF);

				time_route0 = green_counter;
				time_route1 = green_counter + amber_counter_buffer;

				if (green_counter <= 0) {
					status = AUTO_DIR0_AMBER;
					amber_counter = amber_counter_buffer;
				} else {
					green_counter--;
				}
				break;

			case AUTO_DIR0_AMBER:
				set_red_LEDs(OFF, ON);
				set_amber_LEDs(ON, OFF);
				set_green_LEDs(OFF, OFF);

				time_route0 = amber_counter;
				time_route1 = amber_counter;

				if (amber_counter <= 0) {
					status = AUTO_DIR1_GREEN;
					green_counter = green_counter_buffer;
				} else {
					amber_counter--;
				}
				break;

			case AUTO_DIR1_GREEN:
				set_red_LEDs(ON, OFF);
				set_amber_LEDs(OFF, OFF);
				set_green_LEDs(OFF, ON);

				time_route0 = green_counter + amber_counter_buffer;
				time_route1 = green_counter;

				if (green_counter <= 0) {
					status = AUTO_DIR1_AMBER;
					amber_counter = amber_counter_buffer;
				} else {
					green_counter--;
				}
				break;

			case AUTO_DIR1_AMBER:
				set_red_LEDs(ON, OFF);
				set_amber_LEDs(OFF, ON);
				set_green_LEDs(OFF, OFF);

				time_route0 = amber_counter;
				time_route1 = amber_counter;

				if (amber_counter <= 0) {
					status = AUTO_DIR0_GREEN;
					green_counter = green_counter_buffer;
				} else {
					amber_counter--;
				}
				break;

			default:
				status = INIT;
				break;
		}

		set_7seg_buffer_0(time_route0);
		set_7seg_buffer_1(time_route1);
	} else --counter;
}
