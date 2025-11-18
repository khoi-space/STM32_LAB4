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
		if (status == MAN_RED) {
			clear_all_7seg_en();
			status = MAN_GREEN;
		}
		else if (status == MAN_GREEN) {
			counter = amber_counter_buffer * COUNTER_CYCLE;
			status = MAN_AMBER;
		}
	} else if (!is_button_pressed(BUTTON_NEXT_OR_UP)) {
		is_up_button_locked = 0;
	}

	if (status == MAN_AMBER) {
		if (counter <= 0) {
			counter = amber_counter_buffer * COUNTER_CYCLE;
			status = MAN_RED;
		} else --counter;
	}

	// Display
	switch(status) {
		case MAN_RED:
			set_red_LEDs(ON, OFF);
			set_amber_LEDs(OFF, OFF);
			set_green_LEDs(OFF, ON);
			break;
		case MAN_AMBER:
			set_red_LEDs(OFF, ON);
			set_amber_LEDs(ON, OFF);
			set_green_LEDs(OFF, OFF);
			break;
		case MAN_GREEN:
			set_red_LEDs(OFF, ON);
			set_amber_LEDs(OFF, OFF);
			set_green_LEDs(ON, OFF);
			break;
		default:
			status = MAN_RED;
			break;
	}
}
