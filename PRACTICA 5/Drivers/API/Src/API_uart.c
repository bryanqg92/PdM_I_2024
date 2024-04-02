/*
 * API_uart.c
 *
 *  Created on: Mar 30, 2024
 *      Author: lean
 */
/**
  ******************************************************************************
  * @file           API_uart.c
  * @brief          Implementación de funciones para la comunicación UART.
  ******************************************************************************
  * Este archivo contiene las funciones para inicializar, enviar y recibir datos
  * a través de la interfaz UART en un microcontrolador STM32F429ZI.
  *
  * Utiliza unicamente USART3 que por defecto es el de la STLINK de la placa
  *
  ******************************************************************************
  *
  */


#include <stdio.h>
#include <string.h>


#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_nucleo_144.h"


#include "API_uart.h"


#define UART_TIMEOUT_MS 1000


static void Error_Handler(void);

/**
  * @brief  Inicializa la UART.
  * @retval true si la inicialización es exitosa, false si ocurre un error.
  */
bool_t uartInit() {

	/* Configuración de la estructura de la UART */
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

/**
  * @brief  Envía una cadena a través de la UART.
  * @param  pstring: Puntero a la cadena a enviar.
  * @retval None
  */
void uartSendString(uint8_t *pstring) {
	uint16_t length = strlen((char *)pstring);
	HAL_UART_Transmit(&UartHandle, pstring, length, UART_TIMEOUT_MS);
}

/**
  * @brief  Envía una cadena de tamaño específico a través de la UART.
  * @param  pstring: Puntero a la cadena a enviar.
  * @param  size: Tamaño de la cadena a enviar.
  * @retval None
  */
void uartSendStringSize(uint8_t *pstring, uint16_t size) {
	HAL_UART_Transmit(&UartHandle, pstring, size, UART_TIMEOUT_MS);
}

/**
  * @brief  Recibe una cadena de tamaño específico a través de la UART.
  * @param  pstring: Puntero al buffer donde se almacenará la cadena recibida.
  * @param  size: Tamaño de la cadena a recibir.
  * @retval None
  */
void uartReceiveStringSize(uint8_t *pstring, uint16_t size) {
	HAL_UART_Receive(&UartHandle, pstring, size, UART_TIMEOUT_MS);
}

/**
  * @brief  Implementación de fputc para redirigir la salida de printf a la UART.
  * @param  ch: Carácter a imprimir.
  * @param  stream: Puntero al flujo de salida (no utilizado).
  * @retval Carácter impreso.
  */
PUTCHAR_PROTOTYPE {
	/* Transmitir el carácter a través de la UART */
	HAL_UART_Transmit(&UartHandle, (uint8_t*) &ch, 1, UART_TIMEOUT_MS);

	return ch;
}

/**
  * @brief  Manejador de errores.
  * @note   Esta función se llama en caso de error y entra en un bucle infinito.
  * @retval None
  */
static void Error_Handler(void){
	while(1){
		BSP_LED_On(LED2); /**< Enciende el LED2 en caso de error */
	}
}
