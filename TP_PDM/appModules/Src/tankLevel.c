/*
 * tankLevel.c
 *
 *  Created on: Apr 1, 2024
 *      Author: lean
 */

/**
 * @file tankLevel.c
 * @brief Implementación del controlador para la medición de nivel de un tanque de agua utilizando un sensor ultrasónico en una placa STM32F4xx.
 *
 * Este archivo contiene la implementación de funciones para medir el nivel de agua en un tanque utilizando un sensor ultrasónico
 * en una placa STM32F4xx. Se proporcionan funciones para inicializar el sensor, realizar mediciones de nivel de agua y manejar errores.
 *
 * @date Apr 1, 2024
 * @author lean
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

uint8_t levelResult; /**< EXTERN Resultado del nivel de agua. */
float distance; /**< EXTERN Distancia medida por el sensor ultrasónico. */

static const uint16_t MAX_ECHO_TIMEOUT_US = 40000;
static const uint16_t TRIGGER_PERIOD_US = 10; 
static const uint16_t ERROR_TIME_BLINKY_MS = 300; 
static const uint8_t MAX_WATER_LEVEL_CM = 50;
static const float SOUND_VEL = 0.0343; // < Velocidad del sonido en el aire en metros por microsegundo.
static const float HALF_DISTANCE = 2.0; 

static void triggerPulse(void);
static void echoPulseMeasure(void);
static void waterDistance(void);
static void distanceTolevel(void);

static void levelSensor_Error_Handler(void);

/**
 * @brief Inicializa el periférico del sensor de nivel de agua.
 *
 * Esta función inicializa el periférico del sensor de nivel de agua. Configura los pines del microcontrolador
 * conectados al sensor ultrasónico (TRIGGER_PIN y ECHO_PIN) y los pone en el modo adecuado. Además, inicializa
 * las estructuras de retardo para el parpadeo de LEDs en caso de error (blinkyLedsError) y para el timeout de eco
 * del sensor ultrasónico (echoTimeout). Finalmente, realiza una lectura inicial del nivel de agua para establecer
 * el estado inicial del sistema.
 *
 * @return true si la inicialización es exitosa, false en caso contrario.
 */
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


/**
 * @brief Genera un pulso de activación para el sensor ultrasónico.
 *
 * Esta función genera un pulso de activación para el sensor ultrasónico mediante el pin de disparo (TRIGGER_PIN).
 */
static void triggerPulse(void) {
    HAL_GPIO_WritePin(TRIGGER_PORT, TRIGGER_PIN, GPIO_PIN_SET);
    usDelay(TRIGGER_PERIOD_US);
    HAL_GPIO_WritePin(TRIGGER_PORT, TRIGGER_PIN, GPIO_PIN_RESET);
}

/**
 * @brief Mide el ancho del pulso de eco recibido del sensor ultrasónico.
 *
 * Esta función mide el ancho del pulso de eco recibido del sensor ultrasónico y calcula su duración en microsegundos.
 */
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

/**
 * @brief Calcula la distancia del agua al sensor ultrasónico.
 *
 * Esta función calcula la distancia del agua al sensor ultrasónico utilizando el ancho del pulso de eco y la velocidad del sonido.
 */
static void waterDistance(void) {
    distance = (pulse_width  * SOUND_VEL) / HALF_DISTANCE;
}

/**
 * @brief Calcula el nivel de agua a partir de la distancia medida.
 *
 * Esta función calcula el nivel de agua a partir de la distancia medida por el sensor ultrasónico.
 */
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

/**
 * @brief Obtiene el nivel actual de agua en el tanque.
 *
 * Esta función realiza una serie de pasos para obtener el nivel actual de agua en el tanque utilizando el sensor ultrasónico.
 * Incluye generar un pulso de activación, medir el pulso de eco, calcular la distancia al agua y convertirla en un nivel de agua.
 */
void get_current_water_level(void) {
    triggerPulse();
    echoPulseMeasure();
    waterDistance();
    distanceTolevel();
}

/**
 * @brief Manejador de errores del sensor de nivel de agua.
 *
 * Esta función maneja los errores que puedan ocurrir durante la lectura del nivel de agua. En caso de error, hace parpadear
 * un LED indicador.
 */
static void levelSensor_Error_Handler(void){
    if(delayRead(&blinkyLedsError)){
        BSP_LED_Toggle(LED1);
    }
}
