/*
 * scheduler.h
 *
 *  Created on: Nov 6, 2025
 *      Author: ADMIN
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

typedef struct sTasks_Node {
	void (*pTask)(void);
	uint32_t	Delay;
	uint32_t	Period;
	uint8_t		RunMe;
	uint32_t	TaskID;

	struct sTasks_Node *pNext;
}sTasks;

#define SCH_MAX_TASKS	20
#define NO_TASK_ID		0
#define TIMER_CYCLE		10

void SCH_Init(void);
uint32_t SCH_Add_Task(void (*pFunc)(), uint32_t _DELAY, uint32_t _PERIOD);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
uint8_t SCH_Delete_Task(uint32_t taskID);

#endif /* INC_SCHEDULER_H_ */
