/*
 * API_uart.c
 *
 *  Created on: Mar 30, 2024
 *      Author: lean
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_nucleo_144.h"

#include "API_uart.h"

#include <stdio.h>
#include <string.h>

static UART_HandleTypeDef UartHandle;


static void Error_Handler(void);



bool_t uartInit() {


	UartHandle.Instance = USARTx;
	UartHandle.Init.BaudRate = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&UartHandle) != HAL_OK) {
		Error_Handler();
	}


	printf("\n\r UART configured as follows:"
			"\n\r\t INSTANCE      =   USART3"
			"\n\r\t BaudRate      =   9600"
			"\n\r\t WordLength    =   8B"
			"\n\r\t Init.StopBits =   1"
			"\n\r\t Init.Parity   =   NONE"
			"\n\r\t Init.Mode     =   MODE_TX_RX"
			"\n\r\t HwFlowCtl      =   NONE"
			"\n\r\t OverSampling  =   16 \n\r"
	);


	return true;
}


void uartSendString(uint8_t *pstring) {
	uint16_t length = strlen((char *)pstring);
	HAL_UART_Transmit(&UartHandle, pstring, length, UART_TIMEOUT_MS);
}



void uartSendStringSize(uint8_t *pstring, uint16_t size) {
	HAL_UART_Transmit(&UartHandle, pstring, size, UART_TIMEOUT_MS);
}

void uartReceiveStringSize(uint8_t *pstring, uint16_t size) {
	HAL_UART_Receive(&UartHandle, pstring, size, UART_TIMEOUT_MS);
}

PUTCHAR_PROTOTYPE {
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART3 and Loop until the end of transmission */
	HAL_UART_Transmit(&UartHandle, (uint8_t*) &ch, 1, UART_TIMEOUT_MS);

	return ch;
}



static void Error_Handler(void){
	while(1){
		BSP_LED_On(LED2);
	}
}
