/*
 * command_parser.c
 *
 *  Created on: Dec 12, 2024
 *      Author: Admin
 */

#include "command_parser.h"

#define TIMEOUT 300

unsigned char start_send_data = RESET;
unsigned char stop_send_data = RESET;
unsigned char flag_timeout = RESET;
unsigned char string[100];
int ADC_value = 0;
uint16_t timeout_counter = TIMEOUT;

UART_HandleTypeDef* huart;
ADC_HandleTypeDef* hadc;

enum command_parser_state {
	IDLE,
	START_PROCESS,
	RECEIVE_R,
	RECEIVE_S,
	RECEIVE_T,
	RECEIVE_O,
	RECEIVE_K,
	START_SEND_DATA_SIGNAL,
	STOP_SEND_DATA_SIGNAL
};

enum uart_communication_state {
	COMMUNICATE_ERROR,
	WAITING,
	TIME_OUT_STATE,
	START_SEND_DATA,
	STOP_SEND_DATA
};

enum command_parser_state commandParserState =  IDLE;
enum uart_communication_state uartCommunicationState = WAITING;

void Init_Parser(UART_HandleTypeDef* huart1, ADC_HandleTypeDef* hadc1){
	huart = huart1;
	hadc =	hadc1;
}

void command_parser_fsm(void){
	unsigned char bufferCheck = buffer[index_buffer - 1];

	switch(commandParserState){
	case IDLE:{
		if(bufferCheck == '!'){
			commandParserState = START_PROCESS;
		}
		break;
	}
	case START_PROCESS:{
		if(bufferCheck == 'O'){
			commandParserState = RECEIVE_O;
		}
		else if(bufferCheck == 'R'){
			commandParserState = RECEIVE_R;
		}
		else{
			commandParserState = IDLE;
		}
		break;
	}
	case RECEIVE_R:{
		if(bufferCheck == 'S'){
			commandParserState = RECEIVE_S;
		}
		else{
			commandParserState = IDLE;
		}
		break;
	}
	case RECEIVE_S:{
		if(bufferCheck == 'T'){
			commandParserState = RECEIVE_T;
		}
		else{
			commandParserState = IDLE;
		}
		break;
	}
	case RECEIVE_T:{
		if(bufferCheck == '#'){
			commandParserState = START_SEND_DATA_SIGNAL;
		}
		else{
			commandParserState = IDLE;
		}
		break;
	}
	case RECEIVE_O:{

		if(bufferCheck == 'K'){
			commandParserState = RECEIVE_K;
		}
		else{
			commandParserState = IDLE;
		}
		break;
	}
	case RECEIVE_K:{
		if(bufferCheck == '#'){
			commandParserState = STOP_SEND_DATA_SIGNAL;
		}
		else{
			commandParserState = IDLE;
		}
		break;
	}
	case START_SEND_DATA_SIGNAL:{
		ADC_value = HAL_ADC_GetValue(hadc);
		sprintf((void*)string, "!ADC=%d#\r\n", ADC_value);
		start_send_data = SET;
		stop_send_data = RESET;
		commandParserState = IDLE;
		break;
	}
	case STOP_SEND_DATA_SIGNAL:{
		HAL_UART_Transmit(huart, (void*)"STOP\r\n", sizeof("STOP\r\n"), 100);

		start_send_data = RESET;
		stop_send_data = SET;
		commandParserState = IDLE;
		break;
	}
	}

}


void uart_communication_fsm(void){
	switch(uartCommunicationState){
	case COMMUNICATE_ERROR:{
		HAL_UART_Transmit(huart, (void*)"COMMUNICATE ERROR\r\n", sizeof("COMMUNICATE ERROR\r\n"), 100);
		uartCommunicationState = WAITING;
		stop_send_data = RESET;
		break;
	}
	case WAITING:{
		if(start_send_data){
			uartCommunicationState = START_SEND_DATA;
		}
		if(stop_send_data){
			uartCommunicationState = COMMUNICATE_ERROR;
		}
		break;
	}
	case TIME_OUT_STATE:{
		if(flag_timer[1]){
			timeout_counter--;
			setTimer(1, 10);
		}

		// change state
		if(timeout_counter == 0){
			timeout_counter = TIMEOUT;
			uartCommunicationState = START_SEND_DATA;
		}
		if(stop_send_data){
			timeout_counter = TIMEOUT;
			uartCommunicationState = STOP_SEND_DATA;
		}
		break;
	}
	case START_SEND_DATA:{
		//  send data
		if(flag_timer[2]){
			HAL_UART_Transmit(huart, (void*)string, sizeof(string), 100);
			setTimer(2, 500);;
		}


		if(stop_send_data){
			uartCommunicationState = STOP_SEND_DATA;
		}

		if(buffer_flag){
			uartCommunicationState = TIME_OUT_STATE;
		}
		break;
	}
	case STOP_SEND_DATA:{
		stop_send_data = RESET;
		uartCommunicationState = WAITING;
		break;
	}
	default:{
		break;
	}
	}
}
