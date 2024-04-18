

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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "API_uart.h"
#include "API_LCD_I2C.h"
#include "API_DelayUs.h"

#include "stm32f4xx_hal.h"

#include "tankLevel.h"
#include "valveControl.h"
#include "LCD_Manager.h"
#include "UART_Manager.h"


//LCD MESSAGES OK AND ERROR

const uint8_t lcd_i2c_ok[] = "LCD_i2c OK";
const uint8_t lcd_uart_ok[] = "UART COM OK";
const uint8_t lcd_timer_ok[] = "usTimer OK";
const uint8_t lcd_valves_ok[] = "Valves OK";

const uint8_t lcd_uart_err[] = "UART COM ERROR";
const uint8_t lcd_timer_err[] = "usTimer ERROR";
const uint8_t lcd_valves_err[] = "Valves ERROR";


//UART MESSAGES OK AND ERROR
const uint8_t uart_i2c_ok[] = "\r\nLCD_i2c OK";
const uint8_t uart_timer_ok[] = "\r\nusTimer OK";
const uint8_t uart_valves_ok[] = "\r\nValves OK";

const uint8_t uart_i2c_err[] = "\r\nLCD_i2c ERROR";
const uint8_t uart_timer_err[] = "\r\nusTimer ERROR";
const uint8_t uart_valves_err[] = "\r\nValves ERROR";


/* Private function prototypes -----------------------------------------------*/

static void SystemClock_Config(void);
static void Error_Handler(void);
static void appSystemInit(void);

static FSM_WaterController appState = INIT;

int main(void)
{

	HAL_Init();

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Initialize BSP Led for LEDs */
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
	//BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);

	while (1) {

		switch (appState){

		case INIT:

			appSystemInit();
			appState = TANK_LEVEL;
			break;

		case TANK_LEVEL:

			get_current_water_level();
			appState = VALVE_CONTROL;
			break;

		case VALVE_CONTROL:

			valveController(levelResult);
			appState = DATA_DISPLAY;
			break;

		case DATA_DISPLAY:

			if (levelResult != LEVEL_ERROR){
				levelShow();
			}
			else{
				levelErrorShow();
			}

			if(valvesState != VALVE_ERROR){
				valveShow();
			}
			else{
				valvesErrorShow();
			}
			appState = UART_DISPLAY;
			break;

		case UART_DISPLAY:

			UARTshowParams();
			appState = TANK_LEVEL;

			break;

		}

	}

}



static void appSystemInit(void){

	Lcd_Clear();

	if(I2C_Init() && Lcd_Init()){

		initShow((uint8_t *)lcd_i2c_ok, LCD);

		/*Create Error icons*/
		Lcd_CGRAM_CreateChar(0, levelErrorIcon);
		Lcd_CGRAM_CreateChar(1, valveErrorIcon);

		if(uartInit()){

			initShow((uint8_t *)lcd_uart_ok,UART);
			uartSendString((uint8_t *)uart_i2c_ok);
		}
		else{
			initShow((uint8_t *)lcd_uart_err,UART);
		}


		if(Timer_Init() && tankLevelInit()){
			initShow((uint8_t *)lcd_timer_ok,LEVEL);
			uartSendString((uint8_t *)uart_timer_ok);
		}
		else{
			initShow((uint8_t *)lcd_timer_err,LEVEL);
			uartSendString((uint8_t *)uart_timer_err);
		}

		if(valveManagerInit()){
			initShow((uint8_t *)lcd_valves_ok,VALVES);
			uartSendString((uint8_t *)uart_valves_ok);
		}
		else{
			initShow((uint8_t *)lcd_valves_err,VALVES);
			uartSendString((uint8_t *)uart_valves_err);
		}
		appState = TANK_LEVEL;
	}
	else{
		appState = INIT,
		Error_Handler();
	}

	endLCDShow();

}


/*
 * @brief Calculate a randomly value between two delay time
 * @return Calculate value
 */


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
 *
 *
 *
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
