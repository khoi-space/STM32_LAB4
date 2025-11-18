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

void fsm_manual_run(void) {
//	if (fsm_man_task_id == NO_TASK_ID) return;

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
