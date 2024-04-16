/*
 * API_uart.h
 *
 * This header file defines the API for the UART (Universal Asynchronous Receiver Transmitter) peripheral.
 * only for UART3 stlink comm
 *
 *  Created on: Mar 30, 2024
 *      Author: lean
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_


#include <stdbool.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"




#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


#define UART_TIMEOUT_MS 2000

#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART3



typedef bool bool_t;

bool_t uartInit();												//Inicialización de UART
void uartSendString(uint8_t *pstring);							//envía una cadena hasta encontrar \0
void uartSendStringSize(uint8_t *pstring, uint16_t size); 		//envía una cadena de longotid size
void uartReceiveStringSize(uint8_t * pstring, uint16_t size); 	//recibe una cadena de longitud size y lo guarda en pstring


#endif /* API_INC_API_UART_H_ */
