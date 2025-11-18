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
	if (mode != MAN_MODE) return;

	// Change mode to config_mode
	if (is_button_pressed(BUTTON_MODE) && is_mode_button_locked == 0) {
		is_mode_button_locked = 1;
		clear_all_LEDs();

		mode = CONFIG_MODE;
		status = CONFIG_RED;
		last_config_status = CONFIG_RED;

//		if (SCH_Delete_Task(fsm_man_task_id)) {
//			fsm_man_task_id = NO_TASK_ID;
//		}
//		fsm_config_task_id = SCH_Add_Task(fsm_config_run, 0, 100);

		is_mode_button_locked = 1;
		return;
	}
	else is_mode_button_locked = 0;

	if (is_button_pressed(BUTTON_NEXT_OR_UP)) {
		if (is_up_button_locked == 0) {
			if (status == MAN_RED) status = MAN_GREEN;
			if (status == MAN_GREEN) status = MAN_RED;

			is_up_button_locked = 1;
		}
	} else is_up_button_locked = 0;

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
	clear_all_7seg_en();
}
