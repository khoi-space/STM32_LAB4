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

static int last_config_status = -1;

//static int blink_counter = 0;
#define BLINK_CYCLE	5 //500ms

void fsm_config_run(void) {
	if (fsm_config_task_id == NO_TASK_ID) {
		last_config_status = -1;
		return;
	}

	// Blink LEDs every 500ms
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
}
