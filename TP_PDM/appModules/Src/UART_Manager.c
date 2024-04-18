/*
 * UART_Manager.c
 *
 *  Created on: Apr 10, 2024
 *      Author: lean
 */

/**
 * @file UART_Manager.c
 * @brief Implementación del gestor UART para mostrar mensajes relacionados con el nivel del tanque y el control de válvulas.
 *
 * Este archivo contiene la implementación del gestor UART que muestra mensajes relacionados con el nivel del tanque y el control de válvulas.
 * Utiliza la comunicación UART para enviar los mensajes.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "API_uart.h"       
#include "API_Delay.h"      

#include "UART_Manager.h"   
#include "tankLevel.h"      
#include "valveControl.h"   

#include "stm32f4xx_hal.h"  /

static delay_t UARTtxPeriod;  
static bool delayInitialized = false; 
static char UARTLevelMsg[UART_BUFFER_SIZE];  
static const char *UARTlevelMsgError = "\r\nError al medir nivel. Verificar sensor";  

/**
 * @brief Muestra los parámetros del tanque y las válvulas a través de UART.
 *
 * Esta función muestra los parámetros actuales del tanque (nivel) y el estado de las válvulas (llenado, drenaje, emergencia)
 * a través de la comunicación UART.
 */
void UARTshowParams(){

    if(delayInitialized){
        if(delayRead(&UARTtxPeriod)){
            UARTlevelShow();
            UARTvalveShow();
        }
    } else {
        UARTdelayInit();
    }
}

/**
 * @brief Muestra el nivel del tanque a través de UART.
 *
 * Esta función muestra el nivel actual del tanque a través de la comunicación UART.
 * Si hay un error en la medición del nivel, se envía un mensaje de error.
 */
static void UARTlevelShow(void){

    if(levelResult != LEVEL_ERROR){

        sprintf(UARTLevelMsg,"\r\nNIVEL %d", levelResult);
        uartSendString((uint8_t *)UARTLevelMsg);
    } else {
        uartSendString((uint8_t *)UARTlevelMsgError);
    }
}

/**
 * @brief Muestra el estado de las válvulas a través de UART.
 *
 * Esta función muestra el estado actual de las válvulas (llenado, drenaje, emergencia) a través de la comunicación UART.
 */
static void UARTvalveShow(void){

    if(valvesState == VALVE_ERROR){
        uartSendString((uint8_t *)"\r\nError en valvulas, nivel no cambia");
    } else if(valvesState == EMERGENCY_VALVE){
        uartSendString((uint8_t *)"\r\nValvula emergencia ON, Error sensor nivel");
    } else if(valvesState == DRAIN_VALVE){
        uartSendString((uint8_t *)"\r\nValvula drenaje activa");
    } else if (valvesState == TANK_VALVE){
        uartSendString((uint8_t *)"\r\nValvula llenado activa");
    }
}

/**
 * @brief Inicializa el retardo para la transmisión UART.
 *
 * Esta función inicializa el retardo para garantizar que los mensajes se envíen a intervalos regulares.
 */
static void UARTdelayInit(void){
    delayInit(&UARTtxPeriod,PERIOD_TX_MS);
    delayInitialized = true;
}