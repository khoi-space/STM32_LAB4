/*
 * fsm_btn_handler.c
 *
 *  Created on: Nov 12, 2025
 *      Author: ADMIN
 */

#include "fsm_btn_handler.h"
#include "main.h"
#include "global.h"
#include "input_reading.h"
#include "display7SEG.h"
#include "displayLED.h"
#include "fsm_automatic.h"

static int temp_counter = 0;
static int last_config_status = -1;

void fsm_btn_handler_run(void) {
	if (is_button_pressed(BUTTON_MODE)) {
		if (is_mode_button_locked == 0) {
			clear_all_LEDs();

			if (mode == AUTO_MODE) {
				mode = MAN_MODE;
				status = MAN_RED;
				set_7seg_buffer(0, 0);
			}
			else if (mode == MAN_MODE) {
				mode = CONFIG_MODE;
				status = CONFIG_RED;
				temp_counter = red_counter_buffer;
				last_config_status = CONFIG_RED;
			}
			else if (mode == CONFIG_MODE) {
				mode = AUTO_MODE;
				status = AUTO_DIR1_GREEN;

				red_counter = red_counter_buffer;
				amber_counter = amber_counter_buffer;
				green_counter = green_counter_buffer;
				last_config_status = -1;
			}
			is_mode_button_locked = 1;
			return;
		}
	} else is_mode_button_locked = 0;

	switch(mode) {
	case AUTO_MODE:
		break;

	case MAN_MODE:
		if (is_button_pressed(BUTTON_NEXT_OR_UP)) {
			if (is_up_button_locked == 0) {
				if (status == MAN_RED) status = MAN_GREEN;
				else if (status == MAN_GREEN) status = MAN_RED;
				is_up_button_locked = 1;
			}
		} else is_up_button_locked = 0;
		clear_all_7seg_en();
		break;
	case CONFIG_MODE:
		if (status != last_config_status) {
			if (status == CONFIG_RED) temp_counter = red_counter_buffer;
			else if (status == CONFIG_AMBER) temp_counter = amber_counter_buffer;
			else if (status == CONFIG_GREEN) temp_counter = green_counter_buffer;
			last_config_status = status;
		}

		if (is_button_pressed(BUTTON_NEXT_OR_UP)) {
			if (is_up_button_locked == 0) {
				temp_counter++;
				if (temp_counter > 99) temp_counter = 1;
				is_up_button_locked = 1;
			}
		} else is_up_button_locked = 0;

		if (is_button_pressed(BUTTON_SET)) {
			if (is_set_button_locked == 0) {
				if (status == CONFIG_RED) {
					red_counter_buffer = temp_counter;
					status = CONFIG_AMBER;
				}
				else if (status == CONFIG_AMBER) {
					amber_counter_buffer = temp_counter;
					status = CONFIG_GREEN;
				}
				else if (status == CONFIG_GREEN) {
					green_counter_buffer = temp_counter;
					status = CONFIG_RED;
				}
				is_set_button_locked = 1;
			}
		} else is_set_button_locked = 0;

		set_7seg_buffer(temp_counter, 3);
		break;
	}
}

