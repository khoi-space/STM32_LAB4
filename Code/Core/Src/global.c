/*
 * global.c
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "global.h"
#include "scheduler.h"

int status = INIT;
int mode = AUTO_MODE;
int is_mode_button_locked = 0;
int is_up_button_locked = 0;
int is_set_button_locked = 0;
int is_reset_button_locked = 0;

int red_counter_buffer = 5;
int amber_counter_buffer = 2;
int green_counter_buffer = 3;

int fsm_auto_task_id = NO_TASK_ID;
int fsm_man_task_id = NO_TASK_ID;
int fsm_config_task_id = NO_TASK_ID;

int led_7seg_buffer[4] = {0, 0, 0, 0};
