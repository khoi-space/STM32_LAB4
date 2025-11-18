/*
 * fsm_config.c
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "global.h"
#include "display7SEG.h"
#include "displayLED.h"
#include "fsm_config.h"
#include "fsm_automatic.h"
#include "fsm_manual.h"
#include "input_reading.h"
#include "scheduler.h"

int temp_counter = 0;

int last_config_status = -1;

static int blink_counter = 0;
#define BLINK_CYCLE	5 //500ms

void fsm_config_run(void) {
	if (mode != CONFIG_MODE) {
		last_config_status = -1;
		return;
	}

	// Change mode to auto_mode
	if (is_button_pressed(BUTTON_MODE) && is_mode_button_locked == 0) {
		is_mode_button_locked = 1;
		clear_all_LEDs();

		red_counter = red_counter_buffer;
		amber_counter = amber_counter_buffer;
		green_counter = green_counter_buffer;

		mode = AUTO_MODE;
		status = AUTO_DIR2_GREEN;
		last_config_status = -1;

		is_mode_button_locked = 1;
		return;
	} else if (!is_button_pressed(BUTTON_MODE)) {
		is_mode_button_locked = 0;
	}

	// If button_next_or_up pressed, count up temp_counter
	if (is_button_pressed(BUTTON_NEXT_OR_UP) && is_up_button_locked == 0) {
		is_up_button_locked = 1;
		temp_counter += 1;
		if (temp_counter > 99) temp_counter = 0;
	} else if (!is_button_pressed(BUTTON_NEXT_OR_UP)) {
		is_up_button_locked = 0;
	}

	// If button_set pressed, set temp_counter to counter
	if (is_button_pressed(BUTTON_SET) && is_set_button_locked == 0) {
		is_set_button_locked = 1;
		if (status == CONFIG_RED) {
			red_counter_buffer = temp_counter;
			temp_counter = amber_counter_buffer;
			status = CONFIG_AMBER;
		}
		else if (status == CONFIG_AMBER) {
			amber_counter_buffer = temp_counter;
			temp_counter = green_counter_buffer;
			status = CONFIG_GREEN;
		}
		else if (status == CONFIG_GREEN) {
			green_counter_buffer = temp_counter;
			temp_counter = red_counter_buffer;
			status = CONFIG_RED;
		}
	} else if (is_button_pressed(BUTTON_SET)) {
		is_set_button_locked = 0;
	}


	// Blink LEDs every 500ms
//	switch(status) {
//		case CONFIG_RED:
//			blink_red_LEDs();
//			set_amber_LEDs(OFF, OFF);
//			set_green_LEDs(OFF, OFF);
//			break;
//		case CONFIG_AMBER:
//			blink_amber_LEDs();
//			set_red_LEDs(OFF, OFF);
//			set_green_LEDs(OFF, OFF);
//			break;
//		case CONFIG_GREEN:
//			blink_green_LEDs();
//			set_red_LEDs(OFF, OFF);
//			set_amber_LEDs(OFF, OFF);
//			break;
//		default:
//			status = CONFIG_RED;
//			break;
//	}

	set_7seg_buffer_0(2);
	set_7seg_buffer_1(temp_counter);
	update_7seg_multiplex();

	if (blink_counter <= 0) {
		blink_counter = BLINK_CYCLE;

		switch(status) {
			case CONFIG_RED:
				blink_red_LEDs();
				set_amber_LEDs(OFF, OFF);
				set_green_LEDs(OFF, OFF);
				break;
			case CONFIG_AMBER:
				blink_amber_LEDs();
				set_red_LEDs(OFF, OFF);
				set_green_LEDs(OFF, OFF);
				break;
			case CONFIG_GREEN:
				blink_green_LEDs();
				set_red_LEDs(OFF, OFF);
				set_amber_LEDs(OFF, OFF);
				break;
			default:
				status = CONFIG_RED;
				break;
		}
	} else {
		blink_counter--;
	}
}
