/*
 * LCD_Manager.c
 *
 *  Created on: Apr 7, 2024
 *      Author: lean
 */

/**
 * @file LCD_Manager.c
 * @brief Implementación del controlador para la gestión del LCD.
 *
 * Este archivo contiene la implementación del controlador para la gestión del LCD. Proporciona funciones para inicializar
 * y mostrar mensajes en el LCD, así como para mostrar información relacionada con el nivel de agua y el estado de las válvulas.
 * También incluye funciones para mostrar indicadores de error personalizados.
 *
 * @date Apr 7, 2024
 * @author lean
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "API_LCD_I2C.h"
#include "API_Delay.h"

#include "LCD_Manager.h"
#include "tankLevel.h"
#include "valveControl.h"
#include "stm32f4xx_hal.h"

static const uint16_t DELAY_DISPLAY_SHOW = 5000; 

delay_t blinkyErrorIcons; 
bool blinkyErrorState = true; 

/* Iconos personalizados para indicadores de error */
const char levelErrorIcon[8] = {0x04,0x0A,0x11,0x04,0X0A,0x11,0x04,0x0A}; 
const char valveErrorIcon[8] = {0x0E,0x04,0x04,0x15,0x1F,0x1F,0x1B,0x11}; 

static char levelMsg[LEVEL_BUFFER_SIZE]; 

/**
 * @brief Inicializa la pantalla del LCD y muestra un mensaje.
 *
 * Esta función inicializa la pantalla del LCD y muestra un mensaje en la posición indicada por el parámetro 'module'.
 *
 * @param msg Mensaje a mostrar en el LCD.
 * @param module Módulo de la aplicación para el que se muestra el mensaje (LCD, UART, LEVEL o VALVES).
 */
void initShow(uint8_t* msg, app_SystemInit module){

	delayInit(&blinkyErrorIcons,BLINKY_ERROR_TIME_MS);

	switch (module){

	case LCD:
		Lcd_Set_Cursor(1,1);
		break;

	case UART:
		Lcd_Set_Cursor(2,1);
		break;

	case LEVEL:
		Lcd_Set_Cursor(3,1);
		break;

	case VALVES:
		Lcd_Set_Cursor(4,1);
		break;

	}
	Lcd_Send_String((char *)msg);
}

/**
 * @brief Finaliza la visualización en el LCD.
 *
 * Esta función finaliza la visualización en el LCD después de un tiempo determinado.
 */
void endLCDShow(void){
	HAL_Delay(DELAY_DISPLAY_SHOW);
	Lcd_Clear();
}

/**
 * @brief Muestra el nivel de agua en el LCD.
 *
 * Esta función muestra el nivel de agua actual en el LCD.
 */
void levelShow(void){
	Lcd_Set_Cursor(1,1);
	sprintf(levelMsg,"NIVEL %d", levelResult);
	Lcd_Send_String((char *)levelMsg);
	Lcd_Set_Cursor(1,19);
	Lcd_Send_String(" ");
}

/**
 * @brief Muestra el indicador de error de nivel en el LCD.
 *
 * Esta función muestra el indicador de error de nivel en el LCD.
 */
void levelErrorShow(void){

	Lcd_Set_Cursor(1,19);

	if(delayRead(&blinkyErrorIcons)){
		blinkyErrorState = !blinkyErrorState;
	}

	/*realiza toogle de variable bool para el blink de iconos de error*/
	if(blinkyErrorState){
		Lcd_CGRAM_WriteChar(0);;
	}
	else{
		Lcd_Send_String(" ");
	}

}

/**
 * @brief Muestra el estado de las válvulas en el LCD.
 *
 * Esta función muestra el estado actual de las válvulas en el LCD.
 */
void valveShow(void){

	Lcd_Set_Cursor(2,1);

	/*los espacios de cada salida de ajustan al tamaño del BUFFER_SIZE*/
	if(valvesState == EMERGENCY_VALVE){
		Lcd_Send_String("EMERGENCIA    ");
	}
	else if (valvesState == DRAIN_VALVE){
		Lcd_Send_String("DRENAJE       ");
	}
	else{
		Lcd_Send_String("LLENADO       ");
	}

	Lcd_Set_Cursor(1,20);
	Lcd_Send_String(" ");
}

/**
 * @brief Muestra el indicador de error de válvulas en el LCD.
 *
 * Esta función muestra el indicador de error de válvulas en el LCD.
 */
void valvesErrorShow(void){

	Lcd_Set_Cursor(1,20);

	if(delayRead(&blinkyErrorIcons)){
		blinkyErrorState = !blinkyErrorState;
	}

	if(blinkyErrorState){
		Lcd_CGRAM_WriteChar(1);
	}
	else{
		Lcd_Send_String(" ");
	}

}
