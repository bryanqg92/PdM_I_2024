/*
 * valveControl.c
 *
 *  Created on: Apr 9, 2024
 *      Author: lean
 */

#include <math.h>

#include "valveControl.h"
#include "API_Delay.h"
#include "tankLevel.h"


static const uint16_t TIME_CHANGE_LEVEL = 10000;
static const float DELTA_LEVEL = 1.0;

uint8_t valvesState;
static delay_t changeLevel;
static float previousLevel;
static bool_t emergencyValve;

static bool_t valveErrorHandler(void);
static void initValvePin(GPIO_TypeDef *port, uint16_t pin);

bool_t valveManagerInit(void) {

	initValvePin(DRAIN_VALVE_PORT, DRAIN_VALVE_PIN);
	initValvePin(TANK_VALVE_PORT, TANK_VALVE_PIN);
	initValvePin(EMERGENCY_VALVE_PORT, EMERGENCY_VALVE_PIN);

	delayInit(&changeLevel, TIME_CHANGE_LEVEL);

	previousLevel = distance;


	return true;
}

void valveController(uint8_t level) {

    if (valveErrorHandler()) {

        valvesState = VALVE_ERROR;
        HAL_GPIO_WritePin(TANK_VALVE_PORT, TANK_VALVE_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(EMERGENCY_VALVE_PORT, EMERGENCY_VALVE_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(DRAIN_VALVE_PORT, DRAIN_VALVE_PIN, GPIO_PIN_SET);
    }
    else{

        switch (level) {


            case LEVEL_ERROR:
                valvesState = EMERGENCY_VALVE;
                HAL_GPIO_WritePin(TANK_VALVE_PORT, TANK_VALVE_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(EMERGENCY_VALVE_PORT, EMERGENCY_VALVE_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(DRAIN_VALVE_PORT, DRAIN_VALVE_PIN, GPIO_PIN_RESET);
                break;

            case LEVEL_FIVE:
                valvesState = DRAIN_VALVE;
                HAL_GPIO_WritePin(TANK_VALVE_PORT, TANK_VALVE_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(EMERGENCY_VALVE_PORT, EMERGENCY_VALVE_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(DRAIN_VALVE_PORT, DRAIN_VALVE_PIN, GPIO_PIN_SET);

                break;

            default:
                valvesState = TANK_VALVE;
                HAL_GPIO_WritePin(TANK_VALVE_PORT, TANK_VALVE_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(EMERGENCY_VALVE_PORT, EMERGENCY_VALVE_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(DRAIN_VALVE_PORT, DRAIN_VALVE_PIN, GPIO_PIN_RESET);

                break;
        }

    }

}


static bool_t valveErrorHandler(void){


	if(delayRead(&changeLevel)){

		emergencyValve = false;
		float currentLevel = distance;

		if (fabs(currentLevel - previousLevel) < DELTA_LEVEL){
			emergencyValve = true;
		}

		previousLevel = currentLevel;
	}

	return emergencyValve;

}

static void initValvePin(GPIO_TypeDef *port, uint16_t pin) {

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}

