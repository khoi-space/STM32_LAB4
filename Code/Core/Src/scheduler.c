/*
 * scheduler.c
 *
 *  Created on: Nov 6, 2025
 *      Author: ADMIN
 */

#include "scheduler.h"
#include "main.h"


sTasks SCH_tasks_node_pool[SCH_MAX_TASKS];
sTasks *SCH_tasks_list_head = NULL;


// Get a free node in pool
static sTasks* getFreeNode(void) {
	for (int i = 0; i < SCH_MAX_TASKS; ++i) {
		if (SCH_tasks_node_pool[i].TaskID == 0) {
			return &SCH_tasks_node_pool[i];
		}
	}
	return NULL; // Run out of memory
}

// Put node into pool
static void releaseNode(sTasks* node) {
	if (node != NULL) {
		node->TaskID = NO_TASK_ID;
		node->pTask = 0;
		node->pNext = NULL;
	}
}

// Get new taskID
static uint32_t newTaskID = 0;
static uint32_t getNewTaskID(void) {
	newTaskID++;
	if (newTaskID == NO_TASK_ID) {
		newTaskID++; //Skip NO_TASK_ID value
	}
	return newTaskID;
}

void SCH_Init(void) {
	SCH_tasks_list_head = NULL;
	newTaskID = 0;

	for (int i = 0; i < SCH_MAX_TASKS; ++i) {
		SCH_tasks_node_pool[i].TaskID = 0;
		SCH_tasks_node_pool[i].pTask = 0;
		SCH_tasks_node_pool[i].Delay = 0;
		SCH_tasks_node_pool[i].Period = 0;
		SCH_tasks_node_pool[i].RunMe = 0;
		SCH_tasks_node_pool[i].pNext = NULL;
	}
}

uint32_t SCH_Add_Task(void (*pFunc)(), uint32_t DELAY, uint32_t PERIOD) {
	uint32_t _DELAY = DELAY / TIMER_CYCLE;
	uint32_t _PERIOD = PERIOD / TIMER_CYCLE;

	sTasks *newTask = getFreeNode();
	if (newTask == NULL) return NO_TASK_ID;

	newTask->pTask = pFunc;
	newTask->Delay = _DELAY;
	newTask->Period = _PERIOD;
	newTask->RunMe = 0;
	newTask->TaskID = getNewTaskID();
	newTask->pNext = NULL;

	if (SCH_tasks_list_head == NULL) {
		SCH_tasks_list_head = newTask;
		return newTask->TaskID;
	}

	if (_DELAY < SCH_tasks_list_head->Delay) {
		SCH_tasks_list_head->Delay -= _DELAY;
		newTask->pNext = SCH_tasks_list_head;
		SCH_tasks_list_head = newTask;
	} else {
		sTasks *curr = SCH_tasks_list_head;
		sTasks *prev = NULL;
		uint32_t sumDelay = 0;

		while (curr != NULL) {
			if (sumDelay + curr->Delay > _DELAY) {
				// Found the index to insert curr
				break;
			}
			sumDelay += curr->Delay;
			prev = curr;
			curr = curr->pNext;
		}

		newTask->Delay = _DELAY - sumDelay;

		if (curr != NULL) {
			curr->Delay -= newTask->Delay;
		}

		newTask->pNext = curr;
		if (prev != NULL) {
			prev->pNext = newTask;
		}
	}

	if (newTask->Delay == 0) {
		newTask->RunMe = 1;
	}

	return newTask->TaskID;
}

void SCH_Update(void) {
	sTasks *head = SCH_tasks_list_head;
	if (head == NULL) return;

	if (head->Delay > 0) {
		head->Delay--;
		if (head->Delay == 0) head->RunMe = 1;
	}
}

void SCH_Dispatch_Tasks(void) {
	if (SCH_tasks_list_head == NULL || SCH_tasks_list_head->RunMe == 0) return;

	sTasks *pTaskToRun = SCH_tasks_list_head;

	SCH_tasks_list_head = SCH_tasks_list_head->pNext;
	if (SCH_tasks_list_head != NULL && SCH_tasks_list_head->Delay == 0) {
		SCH_tasks_list_head->RunMe = 1;
	}

	void (*pFunc)(void) = pTaskToRun->pTask;

	(*pFunc)();

	if (pTaskToRun->Period != 0) { // Check for one-time task
		SCH_Add_Task(pTaskToRun->pTask, pTaskToRun->Period * TIMER_CYCLE, pTaskToRun->Period * TIMER_CYCLE);
	}

	releaseNode(pTaskToRun);
}

//void SCH_Dispatch_Tasks(void) {
//	while (SCH_tasks_list_head != NULL && SCH_tasks_list_head->RunMe > 0) {
//		sTasks *node = SCH_tasks_list_head;
//		SCH_tasks_list_head = node->pNext;
//
//		if (SCH_tasks_list_head != NULL && SCH_tasks_list_head->Delay == 0) {
//			SCH_tasks_list_head->RunMe = 1;
//		}
//
//		void (*pFunc)(void) = node->pTask;
//
//		pFunc();
//
//		if (node->Period != 0) {
//			SCH_Add_Task(pFunc, node->Period * TIMER_CYCLE, node->Period * TIMER_CYCLE);
//		}
//
//		releaseNode(node);
//	}
//}

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

			releaseNode(curr);
			return 1;
		}

		prev = curr;
		curr = curr->pNext;
	}

	return 0;
}
