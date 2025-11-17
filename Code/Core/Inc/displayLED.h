/*
 * displayLEDs.h
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */

#ifndef INC_DISPLAYLED_H_
#define INC_DISPLAYLED_H_

void blink_debug_led(void);
void blink_red_LEDs(void);
void blink_amber_LEDs(void);
void blink_green_LEDs(void);

void set_red_LEDs(int status_0, int status_1);
void set_amber_LEDs(int status_0, int status_1);
void set_green_LEDs(int status_0, int status_1);

void clear_all_LEDs(void);


#endif /* INC_DISPLAYLED_H_ */
