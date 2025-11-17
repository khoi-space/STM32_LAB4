/*
 * display7SEG.h
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */

#ifndef INC_DISPLAY7SEG_H_
#define INC_DISPLAY7SEG_H_

void clear_all_7seg_en(void);

void set_7seg_buffer_0(int num);
void set_7seg_buffer_1(int num);

void update_7seg_multiplex(void);
#endif /* INC_DISPLAY7SEG_H_ */
