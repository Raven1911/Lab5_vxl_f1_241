/*
 * software_timer.h
 *
 *  Created on: Aug 1, 2024
 *      Author: Admin
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_


#include <stdint.h>

#include "main.h"

/////////////
//#include "stm32f4xx_hal.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2;
extern uint16_t flag_timer[10];


void timer_init();
void setTimer(uint8_t index, uint16_t duration);
void timer_run(uint8_t index);


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif /* INC_SOFTWARE_TIMER_H_ */
