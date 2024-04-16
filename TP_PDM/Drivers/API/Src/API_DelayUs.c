/*
 * API_DelayUs.c
 *
 *  Created on: Apr 3, 2024
 *      Author: lean
 */


#include "API_DelayUs.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"

#define BITS8_LIMIT			4294967295



static void Error_Handler(void);

TIM_HandleTypeDef htim5;



bool_t Timer_Init(void) {


	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim5.Instance = TIM5;
	htim5.Init.Prescaler =  90 - 1;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = BITS8_LIMIT;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
	{
		Error_Handler();
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	//__HAL_TIM_SET_COUNTER(&htim5, 0);
	HAL_TIM_Base_Start_IT(&htim5);

	return true;
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

	if(tim_baseHandle->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();

	}
}

uint32_t getUsTick(void) {
	//__HAL_TIM_SET_COUNTER(&htim5, 0);
	//	uint32_t usStartTime = __HAL_TIM_GET_COUNTER(&htim5);
	//	HAL_TIM_Base_Start_IT(&htim5);
	//	return usStartTime;

	return __HAL_TIM_GET_COUNTER(&htim5);

}

void TimerStart(void) {
	__HAL_TIM_SET_COUNTER(&htim5, 0);

	//	HAL_TIM_Base_Stop_IT(&htim5);
	//	uint32_t usCurrentTime = __HAL_TIM_GET_COUNTER(&htim5);
	//	return usCurrentTime;

}

/*uint32_t Timer_GetMicroseconds(void) {

	uint32_t timer_value = usCurrentTime - usStartTime;
	return  timer_value;
}*/



/*
 * Blocking Delay
 * */

void usDelay(uint32_t delay) {

	uint32_t cycles = delay * (HAL_RCC_GetHCLKFreq() / 1000000)/14;

	for (uint32_t i = 0; i < cycles; ++i) {
		__asm("NOP");
	}
}


/*********************************************************************************************/

/*
 * Non Blocking microseconds Delay
 *
 */

void usDelayInit(usDelay_t *delay, uint32_t duration) {

	if (duration <= 0){
		Error_Handler();
	}
	else{
		delay->duration = duration;
		delay->running = false;
	}
}

bool_t usDelayRead(usDelay_t *delay) {

	bool state = false;
	if(delay != NULL){

		uint32_t currentTime = getUsTick();

		if (!delay->running) {
			delay->startTime = getUsTick();
			delay->running = true;
		}

		else {

			if (currentTime - delay->startTime >= delay->duration) {
				delay->running = false;
				state = true;
			}
		}

	}
	else{
		Error_Handler();
	}


	return state;
}

void usDelayWrite(usDelay_t *delay, uint32_t duration) {

	if(delay!=NULL && duration > 0){
		delay->duration = duration;
	}
	else{
		Error_Handler();
	}
}

void usResetDelay(usDelay_t *delay){
	delay->running = false;
}

/*********************************************************************************************/

static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}


