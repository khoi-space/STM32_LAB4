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

static int blink_counter = 0;
#define BLINK_CYCLE	5 //500ms

void initConfig(void) {
	clear_all_LEDs();
	clear_all_7seg_en();

	mode = CONFIG_MODE;
	status = CONFIG_RED;
	temp_counter = red_counter_buffer;
}

void balanceCounter(void) {
    // amber = 20% of red, integer math
    amber_counter_buffer = red_counter_buffer / 5;
    if (amber_counter_buffer < 1) amber_counter_buffer = 1;

    green_counter_buffer = red_counter_buffer - amber_counter_buffer;
    if (green_counter_buffer <= 0) {
        amber_counter_buffer = (red_counter_buffer >= 2) ? 1 : 0;
        green_counter_buffer = red_counter_buffer - amber_counter_buffer;
        if (green_counter_buffer <= 0) green_counter_buffer = 1;
    }
}


void fsm_config_run(void) {
	if (mode != CONFIG_MODE) {
		return;
	}

	// BUTTON_MODE: change mode to auto mode
	if (is_button_pressed(BUTTON_MODE) && is_mode_button_locked == 0) {
		is_mode_button_locked = 1;

		balanceCounter();
		initAuto(); // Prepare resources for auto mode

		is_mode_button_locked = 1;
		return;
	} else if (!is_button_pressed(BUTTON_MODE)) {
		is_mode_button_locked = 0;
	}

	// BUTTON_NEXT_OR_UP: count up temp_counter
	if (is_button_pressed(BUTTON_NEXT_OR_UP) && is_up_button_locked == 0) {
		is_up_button_locked = 1;
		temp_counter += 1;
		if (temp_counter > 99) temp_counter = 0;
	} else if (!is_button_pressed(BUTTON_NEXT_OR_UP)) {
		is_up_button_locked = 0;
	}

	// BUTTON_SET: set temp_counter to counter
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
			balanceCounter();
			temp_counter = red_counter_buffer;
			status = CONFIG_RED;
		}
	} else if (!is_button_pressed(BUTTON_SET)) {
		is_set_button_locked = 0;
	}

	// BUTTON_RESET (just click): reset temp_counter to the lastest counter
	if (is_button_pressed(BUTTON_RESET) && is_reset_button_locked == 0) {
		is_reset_button_locked = 1;

		if (status == CONFIG_RED) temp_counter = red_counter_buffer;
		else if (status == CONFIG_AMBER) temp_counter = amber_counter_buffer;
		else if (status == CONFIG_GREEN) temp_counter = green_counter_buffer;
	} else if (!is_button_pressed(BUTTON_RESET)) {
		is_reset_button_locked = 0;
	}

	// BUTTON_RESET (long press): reset state
	if (is_button_pressed_1s(BUTTON_RESET)) {
		initConfig();
	}

	// Use a counter to increase the delay to 500ms
	if (blink_counter <= 0) {
		blink_counter = BLINK_CYCLE;

		switch(status) {
			case CONFIG_RED:
				set_7seg_buffer_0(1);
				blink_red_LEDs();
				set_amber_LEDs(OFF, OFF);
				set_green_LEDs(OFF, OFF);
				break;
			case CONFIG_AMBER:
				set_7seg_buffer_0(2);
				blink_amber_LEDs();
				set_red_LEDs(OFF, OFF);
				set_green_LEDs(OFF, OFF);
				break;
			case CONFIG_GREEN:
				set_7seg_buffer_0(3);
				blink_green_LEDs();
				set_red_LEDs(OFF, OFF);
				set_amber_LEDs(OFF, OFF);
				break;
			default:
				status = CONFIG_RED;
				break;
		}

		set_7seg_buffer_1(temp_counter);
	} else {
		blink_counter--;
	}

//	update_7seg_multiplex();
}
