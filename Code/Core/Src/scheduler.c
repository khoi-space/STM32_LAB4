/*
 * scheduler.c
 *
 *  Created on: Nov 6, 2025
 *      Author: ADMIN
 */

#include "scheduler.h"

static uint32_t getNewTaskID(void);
static uint32_t newTaskID = 0;

sTasks *SCH_tasks_list_head = NULL;

void SCH_Init(void) {
	SCH_tasks_list_head = NULL;
	newTaskID = 0;
}

uint32_t SCH_Add_Task(void (*pFunc)(), uint32_t DELAY, uint32_t PERIOD) {
	uint32_t _DELAY = DELAY / TIMER_CYCLE;
	uint32_t _PERIOD = PERIOD / TIMER_CYCLE;

	uint8_t newTaskIdx = 0;
	uint32_t sumDelay = 0;
	uint32_t newDelay = 0;

	for (newTaskIdx = 0; newTaskIdx < SCH_MAX_TASKS; ++newTaskIdx) {
		sumDelay = sumDelay + SCH_tasks_list[newTaskIdx].Delay;
		if (sumDelay > _DELAY) {
			newDelay = _DELAY - (sumDelay - SCH_tasks_list[newTaskIdx].Delay);
			SCH_tasks_list[newTaskIdx].Delay = sumDelay - _DELAY;

			// Shift tasks to right to create a gap for the newTask
			for (uint8_t i = SCH_MAX_TASKS - 1; i > newTaskIdx; --i) {
				if(SCH_tasks_list[i - 1].pTask != 0x0000) {
					SCH_tasks_list[i].pTask = SCH_tasks_list[i - 1].pTask;
					SCH_tasks_list[i].Period = SCH_tasks_list[i - 1].Period;
					SCH_tasks_list[i].Delay = SCH_tasks_list[i - 1].Delay;
//					SCH_tasks_list[i].RunMe = SCH_tasks_list[i - 1].RunMe;
					SCH_tasks_list[i].TaskID = SCH_tasks_list[i - 1].TaskID;
				}
			}

			// Insert newTask to the gap
			SCH_tasks_list[newTaskIdx].pTask = pFunc;
			SCH_tasks_list[newTaskIdx].Delay = newDelay;
			SCH_tasks_list[newTaskIdx].Period = _PERIOD;

			if (SCH_tasks_list[newTaskIdx].Delay == 0) {
				SCH_tasks_list[newTaskIdx].RunMe = 1;
			} else {
				SCH_tasks_list[newTaskIdx].RunMe = 0;
			}
			SCH_tasks_list[newTaskIdx].TaskID = getNewTaskID();
			return SCH_tasks_list[newTaskIdx].TaskID;
		} else {
			if (SCH_tasks_list[newTaskIdx].pTask == 0x0000) { //Insert when list is empty or the tail
				SCH_tasks_list[newTaskIdx].pTask = pFunc;
				SCH_tasks_list[newTaskIdx].Delay = _DELAY - sumDelay;
				SCH_tasks_list[newTaskIdx].Period = _PERIOD;
				if (SCH_tasks_list[newTaskIdx].Delay == 0) {
					SCH_tasks_list[newTaskIdx].RunMe = 1;
				} else {
					SCH_tasks_list[newTaskIdx].RunMe = 0;
				}
				SCH_tasks_list[newTaskIdx].TaskID = getNewTaskID();
				return SCH_tasks_list[newTaskIdx].TaskID;
			}
		}
	}
	return SCH_tasks_list[newTaskIdx].TaskID;
}

void SCH_Update(void) {
	if (SCH_tasks_list[0].pTask != 0 && SCH_tasks_list[0].RunMe == 0) {
		if (SCH_tasks_list[0].Delay > 0) {
			SCH_tasks_list[0].Delay--;
			if (SCH_tasks_list[0].Delay ==  0) {
				SCH_tasks_list[0].RunMe = 1;
			}
		}
	}
}

void SCH_Dispatch_Tasks(void) {
	if (SCH_tasks_list[0].RunMe > 0) {
		sTasks tempTask = SCH_tasks_list[0];
		SCH_Delete_Task(SCH_tasks_list[0].TaskID);
		(*tempTask.pTask)();
//		tempTask.RunMe = 0; // Reset RunMe
		if (tempTask.Period != 0) {
			SCH_Add_Task(tempTask.pTask, tempTask.Period * TIMER_CYCLE, tempTask.Period * TIMER_CYCLE);
		}
	}
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
	uint8_t Return_code = 0;
	uint8_t taskIdx;
	uint8_t j;

	if (taskID != NO_TASK_ID) {
		for (taskIdx = 0; taskIdx < SCH_MAX_TASKS; ++taskIdx) { // Search for TaskID
			if (SCH_tasks_list[taskIdx].TaskID == taskID) {
				Return_code = 1; // Found TaskID
				if (taskIdx < SCH_MAX_TASKS - 1) {
					if (SCH_tasks_list[taskIdx + 1].pTask != 0x0000) {
						SCH_tasks_list[taskIdx + 1].Delay += SCH_tasks_list[taskIdx].Delay;
					}
				}

				// Shift right tasks in order to replace the task at taskIdx
				for (j = taskIdx; j < SCH_MAX_TASKS - 1; ++j) {
//					if (SCH_tasks_list[j + 1].pTask != 0x0000) {
						SCH_tasks_list[j].pTask = SCH_tasks_list[j + 1].pTask;
						SCH_tasks_list[j].Period = SCH_tasks_list[j + 1].Period;
						SCH_tasks_list[j].Delay = SCH_tasks_list[j + 1].Delay;
						SCH_tasks_list[j].RunMe = SCH_tasks_list[j + 1].RunMe;
						SCH_tasks_list[j].TaskID = SCH_tasks_list[j + 1].TaskID;
//					}
				}

				// Remove the last
				SCH_tasks_list[j].pTask = 0;
				SCH_tasks_list[j].Period = 0;
				SCH_tasks_list[j].Delay = 0;
				SCH_tasks_list[j].RunMe = 0;
				SCH_tasks_list[j].TaskID = 0;
				return Return_code;
			}
		}
	}
	return Return_code;
}

static uint32_t getNewTaskID(void) {
	newTaskID++;
	if (newTaskID == NO_TASK_ID) {
		newTaskID++; //Skip NO_TASK_ID value
	}
	return newTaskID;
}
