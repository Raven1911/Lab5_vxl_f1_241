/*
 * software_timer.c
 *
 *  Created on: Aug 1, 2024
 *      Author: Admin
 */


#include "software_timer.h"
//#include "button.h"

#define TIMER_CYCLE 10

uint16_t flag_timer[10];
uint16_t timer_counter[10];
uint16_t timer_MUL[10];



void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
}
/////////////////////////////////////////////////////////////
void setTimer(uint8_t index, uint16_t duration){
	timer_MUL[index] = duration/TIMER_CYCLE;
	flag_timer[index] = 0;
	timer_counter[index] = timer_MUL[index];
}

void timer_run(uint8_t index){
	if(timer_counter[index] > 0){
		timer_counter[index]--;
		if(timer_counter[index] <= 0){
			flag_timer[index] = 1;
			timer_counter[index] = timer_MUL[index];
		}
	}

}

///////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		for(int i = 0; i < 10; i++){
			timer_run(i);
		}
		//getKeyInput();
		//SCH_Update_Task();

	}

}


