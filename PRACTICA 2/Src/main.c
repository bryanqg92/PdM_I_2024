

/**
 ******************************************************************************
 * @file    UART/UART_Printf/Src/main.c
 * @author  MCD Application Team
 * @brief   This example shows how to retarget the C library printf function
 *          to the UART.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
 * @{
 */

/** @addtogroup UART_Printf
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Private function prototypes -----------------------------------------------*/

static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
#define DelayBlink1 100
#define DelayBlink2 500
#define DelayBlink3 1000


void delayInit(delay_t *delay, tick_t duration) {

	if (duration <= 0)
	{
		Error_Handler();
	}
	else
	{
		delay->duration = duration;
		delay->running = false;
	}
}

bool_t delayRead(delay_t *delay) {

	if(delay != NULL)
	{

		if (!delay->running) {
			delay->startTime = HAL_GetTick();
			delay->running = true;
			return false; // Se retorna False porque el tiempo no se cumple inicialmente
		}
		else {

			tick_t currentTime = HAL_GetTick();

			if (currentTime - delay->startTime >= delay->duration) {
				delay->running = false;
				return true;
			}
			else {
				return false;
			}
		}
	}
	else
	{
		Error_Handler();
	}

}

void delayWrite(delay_t *delay, tick_t duration) {

	if(delay!=NULL && duration <=0)
	{
		delay->duration = duration;
	}
	else
	{
		Error_Handler();
	}
}

int main(void)
{

	HAL_Init();

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Initialize BSP Led for LED2 */
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);

	/************* PUNTO 2 ****************/

	// Se iniciliza retardo no bloqueante
	delay_t delay_led1,delay_led2,delay_led3;

	delayInit(&delay_led1,DelayBlink1);
	delayInit(&delay_led2,DelayBlink2);
	delayInit(&delay_led3,DelayBlink3);


	while (1) {

		if (delayRead(&delay_led1)) {
			BSP_LED_Toggle(LED1);
		}

		if (delayRead(&delay_led2)) {
			BSP_LED_Toggle(LED2);
		}

		if (delayRead(&delay_led3)) {
			BSP_LED_Toggle(LED3);
		}
	}


	/************ PUNTO 3 *********************************

	// Condiciones del punto 3

	delay_t delay1;
	uint32_t periods[] = {1000, 200, 100};
	int num_patterns = sizeof(periods) / sizeof(periods[0]);


	while (1){
	    for (int i = 0; i < num_patterns; i++)
	    {
	    	// Encender el LED
	    	BSP_LED_Toggle(LED1);
	    	delayWrite(&delay, periods[i] / 2); // 50% de ciclo de trabajo
	    	while (!delayRead(&delay)) {}

	      // Apagar el LED
	    	BSP_LED_Toggle(LED1);
	        delayWrite(&delay, periods[i] / 2); // 50% de ciclo de trabajo
	        while (!delayRead(&delay)) {}
	    }
	  }

	 */



}


/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	if(HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
