/*
 * UART_Manager.c
 *
 *  Created on: Apr 10, 2024
 *      Author: lean
 */
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


#include "API_uart.h"
#include "API_Delay.h"

#include "UART_Manager.h"
#include "tankLevel.h"
#include "valveControl.h"

#include "stm32f4xx_hal.h"

static delay_t UARTtxPeriod;
static bool delayInitialized = false;
static char UARTLevelMsg[UART_BUFFER_SIZE];
static const char *UARTlevelMsgError = "\r\nError al medir nivel. Verificar sensor";


static void UARTlevelShow(void);
static void UARTvalveShow(void);
static void UARTdelayInit(void);

void UARTshowParams(){

	if(delayInitialized){
		if(delayRead(&UARTtxPeriod)){
			UARTlevelShow();
			UARTvalveShow();
		}
	}else{
		UARTdelayInit();
	}
}

static void UARTlevelShow(void){

	if(levelResult != LEVEL_ERROR){

		sprintf(UARTLevelMsg,"\r\nNIVEL %d", levelResult);
		uartSendString((uint8_t *)UARTLevelMsg);
	}
	else{
		uartSendString((uint8_t *)UARTlevelMsgError);
	}


}


static void UARTvalveShow(void){


	if(valvesState == VALVE_ERROR){
		uartSendString((uint8_t *)"\r\nError en valvulas, nivel no cambia");
	}
	else if(valvesState == EMERGENCY_VALVE){
		uartSendString((uint8_t *)"\r\nValvula emergencia ON, Error sensor nivel");
	}
	else if(valvesState == DRAIN_VALVE){
		uartSendString((uint8_t *)"\r\nValvula drenaje activa");
	}
	else if (valvesState == TANK_VALVE){
		uartSendString((uint8_t *)"\r\nValvula llenado activa");
	}


}

static void UARTdelayInit(void){
	delayInit(&UARTtxPeriod,PERIOD_TX_MS);
	delayInitialized = true;
}

