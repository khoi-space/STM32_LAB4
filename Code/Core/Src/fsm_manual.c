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

static int counter;
#define COUNTER_CYCLE 10 //1000ms

void fsm_manual_run(void) {
	if (mode != MAN_MODE) return;

	// Change mode to config_mode
	if (is_button_pressed(BUTTON_MODE) && is_mode_button_locked == 0) {
		is_mode_button_locked = 1;
		clear_all_LEDs();
		clear_all_7seg_en();

		mode = CONFIG_MODE;
		status = CONFIG_RED;
		last_config_status = CONFIG_RED;
		temp_counter = red_counter_buffer;
		return;
	} else if (!is_button_pressed(BUTTON_MODE)) {
		is_mode_button_locked = 0;
	}

	if (is_button_pressed(BUTTON_NEXT_OR_UP) && is_up_button_locked == 0) {
		is_up_button_locked = 1;

		if (status == MAN_DIR2_GREEN) {
			clear_all_7seg_en();
			counter = amber_counter_buffer * COUNTER_CYCLE;
			status = MAN_DIR2_AMBER;
		}

		else if (status == MAN_DIR1_GREEN) {
			clear_all_7seg_en();
			counter = amber_counter_buffer * COUNTER_CYCLE;
			status = MAN_DIR1_AMBER;
		}
	} else if (!is_button_pressed(BUTTON_NEXT_OR_UP)) {
		is_up_button_locked = 0;
	}

	if (status == MAN_DIR2_AMBER) {
		if (counter <= 0) {
			status = MAN_DIR1_GREEN;
		} else --counter;
	}

	if (status == MAN_DIR1_AMBER) {
		if (counter <= 0) {
			status = MAN_DIR2_GREEN;
		} else --counter;
	}

	// Display
	switch(status) {
		case MAN_DIR2_AMBER: // Dir1: RED - Dir2: AMBER
			set_red_LEDs(ON, OFF);
			set_amber_LEDs(OFF, ON);
			set_green_LEDs(OFF, OFF);
			break;
		case MAN_DIR2_GREEN: // Dir1: RED - Dir2: GREEN
			set_red_LEDs(ON, OFF);
			set_amber_LEDs(OFF, OFF);
			set_green_LEDs(OFF, ON);
			break;
		case MAN_DIR1_AMBER: // Dir1: AMBER - Dir2: RED
			set_red_LEDs(OFF, ON);
			set_amber_LEDs(ON, OFF);
			set_green_LEDs(OFF, OFF);
			break;
		case MAN_DIR1_GREEN: // Dir1: GREEN - Dir2: RED
			set_red_LEDs(OFF, ON);
			set_amber_LEDs(OFF, OFF);
			set_green_LEDs(ON, OFF);
			break;
		default:
			status = MAN_DIR1_GREEN;
			break;
	}
}
