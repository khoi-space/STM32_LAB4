/*
 * fsm_manual.c
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "global.h"
#include "display7SEG.h"
#include "displayLED.h"
#include "fsm_manual.h"
#include "fsm_automatic.h"
#include "fsm_config.h"
#include "input_reading.h"
#include "scheduler.h"

static uint8_t counter;
#define COUNTER_CYCLE 10 //1000ms

void initMan(void) {
	clear_all_LEDs();
	clear_all_7seg_en();

	mode = MAN_MODE;
	status = MAN_DIR0_GREEN;
	set_7seg_buffer_0(99);
	set_7seg_buffer_1(99);
}

void fsm_manual_run(void) {
	if (mode != MAN_MODE) return;

	// BUTTON_MODE: change mode to config mode
	if (is_button_pressed(BUTTON_MODE) && is_mode_button_locked == 0) {
		is_mode_button_locked = 1;

		initConfig(); // Prepare resources for config mode
		return;
	} else if (!is_button_pressed(BUTTON_MODE)) {
		is_mode_button_locked = 0;
	}

	// BUTTON_NEXT_OR_UP: decide which direction to free
	if (is_button_pressed(BUTTON_NEXT_OR_UP) && is_up_button_locked == 0) {
		is_up_button_locked = 1;

		if (status == MAN_DIR1_GREEN) {
			clear_all_7seg_en();
			counter = amber_counter_buffer * COUNTER_CYCLE;
			status = MAN_DIR1_AMBER;
		}

		else if (status == MAN_DIR0_GREEN) {
			clear_all_7seg_en();
			counter = amber_counter_buffer * COUNTER_CYCLE;
			status = MAN_DIR0_AMBER;
		}
	} else if (!is_button_pressed(BUTTON_NEXT_OR_UP)) {
		is_up_button_locked = 0;
	}

	// AMBER count-down before switching to RED
	if (status == MAN_DIR1_AMBER) {
		if (counter <= 0) {
			status = MAN_DIR0_GREEN;
			set_7seg_buffer_0(99);
			set_7seg_buffer_1(99);
		} else {
			--counter;
			set_7seg_buffer_0(counter / 10);
			set_7seg_buffer_1(counter / 10);
		}
	}

	if (status == MAN_DIR0_AMBER) {
		if (counter <= 0) {
			status = MAN_DIR1_GREEN;
			set_7seg_buffer_0(99);
			set_7seg_buffer_1(99);
		} else {
			--counter;
			set_7seg_buffer_0(counter / 10);
			set_7seg_buffer_1(counter / 10);
		}
	}

	// BUTTON_RESET (long press): reset state
	if (is_button_pressed_1s(BUTTON_RESET)) {
		initMan();
	}

	// Display
	switch(status) {
		case MAN_DIR0_AMBER: // RED - AMBER
			set_red_LEDs(ON, OFF);
			set_amber_LEDs(OFF, ON);
			set_green_LEDs(OFF, OFF);
			break;
		case MAN_DIR0_GREEN: // RED - GREEN
			set_red_LEDs(ON, OFF);
			set_amber_LEDs(OFF, OFF);
			set_green_LEDs(OFF, ON);
			break;
		case MAN_DIR1_AMBER: // AMBER - RED
			set_red_LEDs(OFF, ON);
			set_amber_LEDs(ON, OFF);
			set_green_LEDs(OFF, OFF);
			break;
		case MAN_DIR1_GREEN: // GREEN - RED
			set_red_LEDs(OFF, ON);
			set_amber_LEDs(OFF, OFF);
			set_green_LEDs(ON, OFF);
			break;
		default:
			status = MAN_DIR0_GREEN;
			break;
	}
}
