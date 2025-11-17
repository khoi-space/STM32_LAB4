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
#include "fsm_manual.h"
#include "fsm_config.h"
#include "scheduler.h"

//static int temp_counter = 0;
static int last_config_status = -1;

void fsm_btn_handler_run(void) {
	if (is_button_pressed(BUTTON_MODE) && is_mode_button_locked == 0) {
		is_mode_button_locked = 1;
		clear_all_LEDs();

		if (fsm_auto_task_id != NO_TASK_ID) {
			// Change to manual mode
			status = MAN_RED;
			if (SCH_Delete_Task(fsm_auto_task_id) == 1) {
				fsm_auto_task_id = NO_TASK_ID;
				fsm_man_task_id = SCH_Add_Task(fsm_manual_run, 0, 100);
			}
		}
		else if (fsm_man_task_id != NO_TASK_ID) {
			// Change to config mode
			status = CONFIG_RED;
			temp_counter = red_counter_buffer;
			last_config_status = CONFIG_RED;

			set_7seg_buffer_0(2); // Config_mode = 2
			set_7seg_buffer_1(temp_counter);

			if (SCH_Delete_Task(fsm_man_task_id) == 1) {
				fsm_man_task_id = NO_TASK_ID;
				fsm_config_task_id = SCH_Add_Task(fsm_config_run, 0, 500);
			}
		}
		else if (fsm_config_task_id != NO_TASK_ID) {
			status = AUTO_DIR1_GREEN;

			red_counter = red_counter_buffer;
			amber_counter = amber_counter_buffer;
			green_counter = green_counter_buffer;
			last_config_status = -1;

			if (SCH_Delete_Task(fsm_config_task_id) != NO_TASK_ID) {
				fsm_config_task_id = NO_TASK_ID;
				fsm_auto_task_id = SCH_Add_Task(fsm_automatic_run, 0, 1000);
			}
		}

		return;
	} else is_mode_button_locked = 0;

	if (is_button_pressed(BUTTON_NEXT_OR_UP) && is_up_button_locked == 0) {
		is_up_button_locked = 1;
		if (fsm_auto_task_id != NO_TASK_ID) {
			return;
		}
		else if (fsm_man_task_id != NO_TASK_ID) {
			if (status == MAN_RED) status = MAN_GREEN;
			else if (status == MAN_GREEN) status = MAN_RED;
			return;
		}
		else if (fsm_config_task_id != NO_TASK_ID) {
			++temp_counter;
			if (temp_counter > 99) temp_counter = 0;
			return;
		}

		return;
	} else is_up_button_locked = 0;

	if (is_button_pressed(BUTTON_SET) && is_set_button_locked == 0) {
		is_set_button_locked = 1;
		if (fsm_auto_task_id != NO_TASK_ID) {
			return;
		}
		else if (fsm_man_task_id != NO_TASK_ID) {
			return;
		}
		else if (fsm_config_task_id != NO_TASK_ID) {
			if (status == CONFIG_RED) {
				red_counter_buffer = temp_counter;
			}
			else if (status == CONFIG_AMBER) {
				amber_counter_buffer = temp_counter;
			}
			else if (status == CONFIG_GREEN) {
				green_counter_buffer = temp_counter;
			}
			return;
		}

		return;
	} else is_set_button_locked = 0;

//	switch(mode) {
//	case AUTO_MODE:
//		break;
//
//	case MAN_MODE:
//		if (is_button_pressed(BUTTON_NEXT_OR_UP)) {
//			if (is_up_button_locked == 0) {
//				if (status == MAN_RED) status = MAN_GREEN;
//				else if (status == MAN_GREEN) status = MAN_RED;
//				is_up_button_locked = 1;
//			}
//		} else is_up_button_locked = 0;
//		clear_all_7seg_en();
//		break;
//	case CONFIG_MODE:
//		if (status != last_config_status) {
//			if (status == CONFIG_RED) temp_counter = red_counter_buffer;
//			else if (status == CONFIG_AMBER) temp_counter = amber_counter_buffer;
//			else if (status == CONFIG_GREEN) temp_counter = green_counter_buffer;
//			last_config_status = status;
//		}
//
//		if (is_button_pressed(BUTTON_NEXT_OR_UP)) {
//			if (is_up_button_locked == 0) {
//				temp_counter++;
//				if (temp_counter > 99) temp_counter = 1;
//				is_up_button_locked = 1;
//			}
//		} else is_up_button_locked = 0;
//
//		if (is_button_pressed(BUTTON_SET)) {
//			if (is_set_button_locked == 0) {
//				if (status == CONFIG_RED) {
//					red_counter_buffer = temp_counter;
//					status = CONFIG_AMBER;
//				}
//				else if (status == CONFIG_AMBER) {
//					amber_counter_buffer = temp_counter;
//					status = CONFIG_GREEN;
//				}
//				else if (status == CONFIG_GREEN) {
//					green_counter_buffer = temp_counter;
//					status = CONFIG_RED;
//				}
//				is_set_button_locked = 1;
//			}
//		} else is_set_button_locked = 0;
//
//		set_7seg_buffer(temp_counter, 3);
//		break;
//	}
}

