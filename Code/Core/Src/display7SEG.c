/*
 * display7SEG.c
 *
 *  Created on: Nov 11, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "global.h"
#include "display7SEG.h"

int digitSegment[10][7] = { // ACTIVE LOW
	{0,0,0,0,0,0,1}, //0
	{1,0,0,1,1,1,1}, //1
	{0,0,1,0,0,1,0}, //2
	{0,0,0,0,1,1,0}, //3
	{1,0,0,1,1,0,0}, //4
	{0,1,0,0,1,0,0}, //5
	{0,1,0,0,0,0,0}, //6
	{0,0,0,1,1,1,1}, //7
	{0,0,0,0,0,0,0}, //8
	{0,0,0,0,1,0,0} //9
};

void clear_all_7seg_en(void) {
	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, OFF);
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, OFF);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, OFF);
	HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, OFF);
}

void display7SEG_0(int num) {
	if (num < 0 || num > 9) return;

	HAL_GPIO_WritePin(SEG_A_0_GPIO_Port, SEG_A_0_Pin, digitSegment[num][0]);
	HAL_GPIO_WritePin(SEG_B_0_GPIO_Port, SEG_B_0_Pin, digitSegment[num][1]);
	HAL_GPIO_WritePin(SEG_C_0_GPIO_Port, SEG_C_0_Pin, digitSegment[num][2]);
	HAL_GPIO_WritePin(SEG_D_0_GPIO_Port, SEG_D_0_Pin, digitSegment[num][3]);
	HAL_GPIO_WritePin(SEG_E_0_GPIO_Port, SEG_E_0_Pin, digitSegment[num][4]);
	HAL_GPIO_WritePin(SEG_F_0_GPIO_Port, SEG_F_0_Pin, digitSegment[num][5]);
	HAL_GPIO_WritePin(SEG_G_0_GPIO_Port, SEG_G_0_Pin, digitSegment[num][6]);
}

void display7SEG_1(int num) {
	if (num < 0 || num > 9) return;

	HAL_GPIO_WritePin(SEG_A_1_GPIO_Port, SEG_A_1_Pin, digitSegment[num][0]);
	HAL_GPIO_WritePin(SEG_B_1_GPIO_Port, SEG_B_1_Pin, digitSegment[num][1]);
	HAL_GPIO_WritePin(SEG_C_1_GPIO_Port, SEG_C_1_Pin, digitSegment[num][2]);
	HAL_GPIO_WritePin(SEG_D_1_GPIO_Port, SEG_D_1_Pin, digitSegment[num][3]);
	HAL_GPIO_WritePin(SEG_E_1_GPIO_Port, SEG_E_1_Pin, digitSegment[num][4]);
	HAL_GPIO_WritePin(SEG_F_1_GPIO_Port, SEG_F_1_Pin, digitSegment[num][5]);
	HAL_GPIO_WritePin(SEG_G_1_GPIO_Port, SEG_G_1_Pin, digitSegment[num][6]);
}

void set_7seg_buffer_0(int num) {
	led_7seg_buffer[0] = num / 10;
	led_7seg_buffer[1] = num % 10;
}

void set_7seg_buffer_1(int num) {
	led_7seg_buffer[2] = num / 10;
	led_7seg_buffer[3] = num % 10;
}

static int led_7seg_idx = 0;
void update_7seg_multiplex(void) {
	clear_all_7seg_en();

	switch(led_7seg_idx) {
		case 0:
			display7SEG_0(led_7seg_buffer[0]); // Dùng chân ..._0
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, ON);
			break;
		case 1:
			display7SEG_0(led_7seg_buffer[1]); // Dùng chân ..._0
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, ON);
			break;
		case 2:
			display7SEG_1(led_7seg_buffer[2]); // Dùng chân ..._1
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, ON);
			break;
		case 3:
			display7SEG_1(led_7seg_buffer[3]); // Dùng chân ..._1
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, ON);
			break;
		default: break;
	}

	led_7seg_idx = (led_7seg_idx + 1) % 4;
}
