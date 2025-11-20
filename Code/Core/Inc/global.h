/*
 * global.h
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"

#define INIT		0
#define	AUTO_DIR0_GREEN	1
#define	AUTO_DIR0_AMBER	2
#define AUTO_DIR1_GREEN	3
#define AUTO_DIR1_AMBER	4

#define MAN_DIR0_GREEN	10
#define MAN_DIR0_AMBER	11
#define MAN_DIR1_GREEN	12
#define MAN_DIR1_AMBER	13

#define CONFIG_RED		20
#define CONFIG_AMBER	21
#define	CONFIG_GREEN	22

#define AUTO_MODE 	30
#define MAN_MODE	31
#define CONFIG_MODE	32

extern int status;
extern int mode;
extern int is_mode_button_locked;
extern int is_up_button_locked;
extern int is_set_button_locked;
extern int is_reset_button_locked;

extern int fsm_auto_task_id;
extern int fsm_man_task_id;
extern int fsm_config_task_id;

extern int red_counter_buffer;
extern int amber_counter_buffer;
extern int green_counter_buffer;

extern int led_7seg_buffer[4];

#endif /* INC_GLOBAL_H_ */
