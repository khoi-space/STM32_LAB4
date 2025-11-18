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
#define COUNTER_CYCLE 10;

void fsm_manual_run(void) {
//	if (fsm_man_task_id == NO_TASK_ID) return;

	if (status == MAN_DIR1_AMBER) {
		if (counter <= 0) {
			counter = amber_counter_buffer * COUNTER_CYCLE;
			status = MAN_DIR2_GREEN;
		} else --counter;
	}

	if (status == MAN_DIR2_AMBER) {
		if (counter <= 0) {
			counter = amber_counter_buffer * COUNTER_CYCLE;
			status = MAN_DIR1_GREEN;
		} else --counter;
	}

	switch(status) {
		case MAN_DIR2_GREEN: // RED - GREEN
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
		case MAN_DIR2_AMBER: // RED - AMBER
			set_red_LEDs(ON, OFF);
			set_amber_LEDs(OFF, ON);
			set_green_LEDs(OFF, OFF);
			break;
		default:
			status = MAN_DIR1_GREEN;
			break;
	}
}
