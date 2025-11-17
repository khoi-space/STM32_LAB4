/*
 * fsm_automatic.h
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */

#ifndef INC_FSM_AUTOMATIC_H_
#define INC_FSM_AUTOMATIC_H_

#include "main.h"
#include "global.h"

extern int red_counter;
extern int amber_counter;
extern int green_counter;

void traffic_machine(int mode);

void fsm_automatic_run(void);

#endif /* INC_FSM_AUTOMATIC_H_ */
