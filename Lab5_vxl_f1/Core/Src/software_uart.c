/*
 * software_uart.c
 *
 *  Created on: Dec 12, 2024
 *      Author: Admin
 */

#include "software_uart.h"


#define BUFFER_SIZE 30
unsigned char msg[] = "hello\n"; // use for transmit string type
unsigned char buffer[BUFFER_SIZE];
unsigned char buffer_receive = 0;
unsigned char buffer_flag = 0;
unsigned char index_buffer = 0;


void UART_SendString(UART_HandleTypeDef *huart, unsigned char* str){
	HAL_UART_Transmit(huart, (void*)msg, sprintf((void*)msg,"%s",str), 10);
}


void UART_SendByte(UART_HandleTypeDef* huart, unsigned char data){
	HAL_UART_Transmit(huart, &data, 1, 100);
}


void UART_StartReceive(UART_HandleTypeDef *huart){
	HAL_UART_Receive_IT(huart, &buffer_receive, 1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		buffer[index_buffer++] = buffer_receive;
		buffer_flag = 1;
		if(index_buffer == 30) index_buffer = 0;
		// communication loop
		HAL_UART_Receive_IT(huart, &buffer_receive, 1);

	}
}
