/*
 * LCD_Manager.c
 *
 *  Created on: Apr 7, 2024
 *      Author: lean
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

const char levelErrorIcon[8] = {0x04,0x0A,0x11,0x04,0X0A,0x11,0x04,0x0A};
const char valveErrorIcon[8] = {0x0E,0x04,0x04,0x15,0x1F,0x1F,0x1B,0x11};

static char levelMsg[LEVEL_BUFFER_SIZE];

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

void endLCDShow(void){
	HAL_Delay(DELAY_DISPLAY_SHOW);
	Lcd_Clear();

}
void levelShow(void){
	Lcd_Set_Cursor(1,1);
	sprintf(levelMsg,"NIVEL %d", levelResult);
	Lcd_Send_String((char *)levelMsg);
	Lcd_Set_Cursor(1,19);
	Lcd_Send_String(" ");
}

void levelErrorShow(void){

	Lcd_Set_Cursor(1,19);

	if(delayRead(&blinkyErrorIcons)){
		blinkyErrorState = !blinkyErrorState;
	}

	if(blinkyErrorState){
		Lcd_CGRAM_WriteChar(0);;
	}
	else{
		Lcd_Send_String(" ");
	}

}

void valveShow(void){

	Lcd_Set_Cursor(2,1);

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


