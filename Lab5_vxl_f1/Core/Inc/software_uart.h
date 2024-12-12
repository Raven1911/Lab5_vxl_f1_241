/*
 * software_uart.h
 *
 *  Created on: Dec 12, 2024
 *      Author: Admin
 */

#ifndef INC_SOFTWARE_UART_H_
#define INC_SOFTWARE_UART_H_

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "usart.h"




extern unsigned char msg[]; // use for transmit string type
extern unsigned char buffer[];
extern unsigned char buffer_flag;
extern unsigned char index_buffer;


void UART_SendString(UART_HandleTypeDef* huart, unsigned char* str);
void UART_SendByte(UART_HandleTypeDef* huart, unsigned char data);
void UART_StartReceive(UART_HandleTypeDef* huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);

#endif /* INC_SOFTWARE_UART_H_ */
