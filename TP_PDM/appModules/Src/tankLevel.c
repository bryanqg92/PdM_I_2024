/*
 * tankLevel.c
 *
 *  Created on: Apr 1, 2024
 *      Author: lean
 */

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "stm32f4xx_nucleo_144.h"
#include "stm32f4xx_hal.h"

#include "API_Delay.h"
#include "API_DelayUs.h"
#include "tankLevel.h"


static delay_t blinkyLedsError;
static usDelay_t echoTimeout;

static uint32_t pulse_width;
static int32_t waterLevel;
static bool_t readlevelError;

uint8_t levelResult;
float distance;


static const uint16_t MAX_ECHO_TIMEOUT_US = 40000;
static const uint16_t TRIGGER_PERIOD_US = 10;
static const uint16_t ERROR_TIME_BLINKY_MS = 300;
static const uint8_t MAX_WATER_LEVEL_CM = 50;
static const float SOUND_VEL = 0.0343;
static const float HALF_DISTANCE = 2.0;



static void triggerPulse(void);
static void echoPulseMeasure(void);
static void waterDistance(void) ;
static void distanceTolevel(void);

static void levelSensor_Error_Handler(void);

bool_t tankLevelInit(void){

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOG_CLK_ENABLE();


	HAL_GPIO_WritePin(TRIGGER_PORT, TRIGGER_PIN, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = TRIGGER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TRIGGER_PORT, &GPIO_InitStruct);


	GPIO_InitStruct.Pin = ECHO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(ECHO_PORT, &GPIO_InitStruct);

	delayInit(&blinkyLedsError, ERROR_TIME_BLINKY_MS);
	usDelayInit(&echoTimeout, MAX_ECHO_TIMEOUT_US);

	get_current_water_level();

	readlevelError = false;

	return true;

}

static void triggerPulse(void) {

	HAL_GPIO_WritePin(TRIGGER_PORT, TRIGGER_PIN, GPIO_PIN_SET);
	usDelay(TRIGGER_PERIOD_US);
	HAL_GPIO_WritePin(TRIGGER_PORT, TRIGGER_PIN, GPIO_PIN_RESET);
}



static void echoPulseMeasure(void) {


	while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_RESET){

		if (usDelayRead(&echoTimeout)) {
			levelSensor_Error_Handler();
		}

	}

	usResetDelay(&echoTimeout);

	uint32_t starter = getUsTick();
	while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_SET) {

		if (usDelayRead(&echoTimeout)) {
			levelSensor_Error_Handler();
		}

	}
	uint32_t finisher = getUsTick();

	usResetDelay(&echoTimeout);
	pulse_width = finisher - starter;

}




static void waterDistance(void) {
	distance = (pulse_width  * SOUND_VEL)/ HALF_DISTANCE;
}



static void distanceTolevel(void) {
    waterLevel = MAX_WATER_LEVEL_CM - distance;
    readlevelError = false;

    switch (waterLevel) {
        case 0 ... 2:
            levelResult = LEVEL_ZERO;
            break;

        case 3 ... 10:
            levelResult = LEVEL_ONE;
            break;

        case 11 ...25:
            levelResult = LEVEL_TWO;
            break;

        case 26 ... 35:
            levelResult = LEVEL_THREE;
            break;

        case 36 ...46:
            levelResult = LEVEL_FOUR;
            break;

        default:
            if (waterLevel >= 47) {
                levelResult = LEVEL_FIVE;
            } else {
                levelResult = LEVEL_ERROR;
                readlevelError = true;
            }
            break;
    }
}





void get_current_water_level(void) {

	triggerPulse();
	echoPulseMeasure();
	waterDistance();
	distanceTolevel();

}


static void levelSensor_Error_Handler(void){

	if(delayRead(&blinkyLedsError)){
		BSP_LED_Toggle(LED1);
	}
}
