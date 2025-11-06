/*
 * scheduler.c
 *
 *  Created on: Nov 6, 2025
 *      Author: ADMIN
 */

#include "scheduler.h"

sTasks SCH_tasks_list[SCH_MAX_TASKS];
uint8_t current_index_task = 0;

void SCH_Init(void) {

}

void SCH_Add_Task(void (*pFunction)(), uint32_t _Delay, uint32_t _Period) {

}

void SCH_Update(void) {

}

void SCH_Dispatch_Tasks(void) {

}

void SCH_Delete_Task(uint32_t taskID) {

}
