/*
 * scheduler.c
 *
 *  Created on: Nov 6, 2025
 *      Author: ADMIN
 */

#include "scheduler.h"
#include "main.h"

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

	sTasks *newTask = (sTasks*)malloc(sizeof(sTasks));
	if (newTask == NULL) return NO_TASK_ID;

	newTask->pTask = pFunc;
	newTask->Delay = _DELAY;
	newTask->Period = _PERIOD;
	newTask->RunMe = (_DELAY == 0) ? 1 : 0;
	newTask->TaskID = getNewTaskID();
	newTask->pNext = NULL;

	if (SCH_tasks_list_head == NULL) {
		SCH_tasks_list_head = newTask;
		return newTask->TaskID;
	}

	sTasks *prev = NULL;
	sTasks *curr = SCH_tasks_list_head;
	uint32_t sumDelay = 0;

	while (curr != NULL && sumDelay + curr->Delay <= _DELAY) {
		sumDelay += curr->Delay;
		prev = curr;
		curr = curr->pNext;
	}

	newTask->Delay = _DELAY - sumDelay;

	if (curr != NULL) {
		curr->Delay -= newTask->Delay;
	}

	if (prev == NULL) {
		newTask->pNext = SCH_tasks_list_head;
		SCH_tasks_list_head = newTask;
	} else {
		newTask->pNext = curr;
		prev->pNext = newTask;
	}

	return newTaskID;
}

void SCH_Update(void) {
	if (SCH_tasks_list_head != NULL) {
		if (SCH_tasks_list_head->RunMe == 0) {
			if (SCH_tasks_list_head->Delay > 0) {
				SCH_tasks_list_head->Delay--;
				if (SCH_tasks_list_head->Delay == 0) {
					SCH_tasks_list_head->RunMe = 1;
				}
			}
		}
	}
}

void SCH_Dispatch_Tasks(void) {
	if (SCH_tasks_list_head != NULL && SCH_tasks_list_head->RunMe > 0) {
		sTasks *pTaskToRun = SCH_tasks_list_head;

		(*pTaskToRun->pTask)(); // Run task

		if (pTaskToRun->Period != 0) {
			uint32_t DELAY = pTaskToRun->Delay * TIMER_CYCLE;
			uint32_t PERIOD = pTaskToRun->Period * TIMER_CYCLE;

			SCH_Add_Task(pTaskToRun->pTask, DELAY, PERIOD);
		}

		SCH_Delete_Task(pTaskToRun->TaskID);
	}
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
	if (taskID == NO_TASK_ID || SCH_tasks_list_head == NULL) return 0;

	sTasks *prev = NULL;
	sTasks *curr = SCH_tasks_list_head;

	while (curr != NULL) {
		if (curr->TaskID == taskID) { // Find taskID
			if (curr->pNext != NULL) {
				curr->pNext->Delay += curr->Delay;
			}

			// Delete node
			if (prev == NULL) {
				SCH_tasks_list_head = curr->pNext;
			} else {
				prev->pNext = curr->pNext;
			}

			free(curr);
			return 1;
		}

		prev = curr;
		curr = curr->pNext;
	}

	return 0;
}


static uint32_t getNewTaskID(void) {
	newTaskID++;
	if (newTaskID == NO_TASK_ID) {
		newTaskID++; //Skip NO_TASK_ID value
	}
	return newTaskID;
}
