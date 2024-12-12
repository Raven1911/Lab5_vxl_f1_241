/*
 * command_parser.h
 *
 *  Created on: Dec 12, 2024
 *      Author: Admin
 */

#ifndef INC_COMMAND_PARSER_H_
#define INC_COMMAND_PARSER_H_

#include "main.h"
#include "software_uart.h"
#include "software_timer.h"
#include "stm32f1xx_hal.h"



void command_parser_fsm(void);
void uart_communication_fsm(void);
void Init_Parser(UART_HandleTypeDef* huart1, ADC_HandleTypeDef* hadc1);

#endif /* INC_COMMAND_PARSER_H_ */
