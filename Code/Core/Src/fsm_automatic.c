/*
 * fsm_automatic.c
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "global.h"
#include "display7SEG.h"
#include "displayLED.h"
#include "fsm_automatic.h"
#include "fsm_manual.h"
#include "fsm_config.h"
#include "input_reading.h"
#include "scheduler.h"

int red_counter = 5;
int amber_counter = 2;
int green_counter = 3;

int time_route0 = 0;
int time_route1 = 0;

void fsm_automatic_run(void) {
	if (fsm_auto_task_id == NO_TASK_ID) return;

	switch(status) {
		case INIT:
			red_counter = red_counter_buffer;
			amber_counter = amber_counter_buffer;
			green_counter = green_counter_buffer;
			status = AUTO_DIR1_GREEN;

			time_route0 = green_counter;
			time_route1 = green_counter + amber_counter;
			break;

		case AUTO_DIR1_GREEN:
			set_red_LEDs(OFF , ON);
			set_amber_LEDs(OFF, OFF);
			set_green_LEDs(ON, OFF);

			time_route0 = green_counter;
			time_route1 = green_counter + amber_counter_buffer;

			if (green_counter <= 0) {
				status = AUTO_DIR1_AMBER;
				amber_counter = amber_counter_buffer;
			} else {
				green_counter--;
			}
			break;

		case AUTO_DIR1_AMBER:
			set_red_LEDs(OFF, ON);
			set_amber_LEDs(ON, OFF);
			set_green_LEDs(OFF, OFF);

			time_route0 = amber_counter;
			time_route1 = amber_counter;

			if (amber_counter <= 0) {
				status = AUTO_DIR2_GREEN;
				green_counter = green_counter_buffer;
			} else {
				amber_counter--;
			}
			break;

		case AUTO_DIR2_GREEN:
			set_red_LEDs(ON, OFF);
			set_amber_LEDs(OFF, OFF);
			set_green_LEDs(OFF, ON);

			time_route0 = green_counter + amber_counter_buffer;
			time_route1 = green_counter;

			if (green_counter <= 0) {
				status = AUTO_DIR2_AMBER;
				amber_counter = amber_counter_buffer;
			} else {
				green_counter--;
			}
			break;

		case AUTO_DIR2_AMBER:
			set_red_LEDs(ON, OFF);
			set_amber_LEDs(OFF, ON);
			set_green_LEDs(OFF, OFF);

			time_route0 = amber_counter;
			time_route1 = amber_counter;

			if (amber_counter <= 0) {
				status = AUTO_DIR1_GREEN;
				green_counter = green_counter_buffer;
			} else {
				amber_counter--;
			}
			break;

		default:
			status = INIT;
			break;
	}

	set_7seg_buffer_0(time_route0);
	set_7seg_buffer_1(time_route1);
}
