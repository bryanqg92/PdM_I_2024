/*
 * API_DelayUs.c
 *
 *  Created on: Apr 3, 2024
 *      Author: lean
 */

/**
 * @file API_DelayUs.c
 * @brief Implementación de funciones de retardo en microsegundos (us) mediante temporizadores para la placa STM32F4xx.
 *
 * Este archivo contiene la implementación de funciones para generar retardos en microsegundos utilizando temporizadores
 * en la placa STM32F4xx. Se proporcionan funciones para inicializar temporizadores, generar retardos bloqueantes y no
 * bloqueantes, así como funciones para configurar y leer estados de los retardos no bloqueantes.
 *
 * @date Apr 3, 2024
 * @author lean
 */

#include "API_DelayUs.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"

#define BITS8_LIMIT			4294967295

/**
 * @brief Manejador de errores genérico.
 *
 * Esta función se utiliza para manejar errores genéricos. En caso de error, enciende el LED2 y entra en un bucle infinito.
 */
static void Error_Handler(void);

TIM_HandleTypeDef htim5; /**< Estructura para el manejo del temporizador TIM5. */

/**
 * @brief Inicializa el temporizador utilizado para generar retardos en microsegundos.
 *
 * Esta función inicializa el temporizador TIM5 para generar retardos en microsegundos. Configura el temporizador
 * con una frecuencia de reloj interna y lo inicia.
 *
 * @return true si la inicialización es exitosa, false en caso contrario.
 */
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

    HAL_TIM_Base_Start_IT(&htim5);

    return true;
}

/**
 * @brief Inicializa los recursos necesarios para el temporizador TIM5.
 *
 * Esta función inicializa los recursos necesarios para el funcionamiento del temporizador TIM5.
 *
 * @param tim_baseHandle Estructura de manejo del temporizador TIM.
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if(tim_baseHandle->Instance==TIM5)
    {
        __HAL_RCC_TIM5_CLK_ENABLE();
    }
}

/**
 * @brief Obtiene el valor actual del contador del temporizador TIM5 en microsegundos.
 *
 * Esta función devuelve el valor actual del contador del temporizador TIM5 en microsegundos.
 *
 * @return Valor actual del contador del temporizador TIM5 en microsegundos.
 */
uint32_t getUsTick(void) {
    return __HAL_TIM_GET_COUNTER(&htim5);
}

/**
 * @brief Reinicia el contador del temporizador TIM5.
 *
 * Esta función reinicia el contador del temporizador TIM5 a cero.
 */
void TimerStart(void) {
    __HAL_TIM_SET_COUNTER(&htim5, 0);
}

/**
 * @brief Genera un retardo bloqueante en microsegundos.
 *
 * Esta función genera un retardo bloqueante en microsegundos utilizando ciclos NOP.
 *
 * @param delay Duración del retardo en microsegundos.
 */
void usDelay(uint32_t delay) {
    uint32_t cycles = delay * (HAL_RCC_GetHCLKFreq() / 1000000)/14; // se calibró a prueba y error, denominador no puede ser impar

    for (uint32_t i = 0; i < cycles; ++i) {
        __asm("NOP");
    }
}

/*********************************************************************************************/

/**
 * @brief Inicializa una estructura de retardo no bloqueante en microsegundos.
 *
 * Esta función inicializa una estructura de retardo no bloqueante en microsegundos con la duración especificada.
 *
 * @param delay Puntero a la estructura de retardo no bloqueante.
 * @param duration Duración del retardo en microsegundos.
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

/**
 * @brief Lee el estado de un retardo no bloqueante en microsegundos.
 *
 * Esta función lee el estado de un retardo no bloqueante en microsegundos. Devuelve true si el retardo ha concluido,
 * false en caso contrario.
 *
 * @param delay Puntero a la estructura de retardo no bloqueante.
 * @return true si el retardo ha concluido, false en caso contrario.
 */
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

/**
 * @brief Actualiza la duración de un retardo no bloqueante en microsegundos.
 *
 * Esta función actualiza la duración de un retardo no bloqueante en microsegundos con el valor especificado.
 *
 * @param delay Puntero a la estructura de retardo no bloqueante.
 * @param duration Nueva duración del retardo en microsegundos.
 */
void usDelayWrite(usDelay_t *delay, uint32_t duration) {
    if(delay!=NULL && duration > 0){
        delay->duration = duration;
    }
    else{
        Error_Handler();
    }
}

/**
 * @brief Reinicia un retardo no bloqueante en microsegundos.
 *
 * Esta función reinicia un retardo no bloqueante en microsegundos, estableciendo su estado a no corriendo.
 *
 * @param delay Puntero a la estructura de retardo no bloqueante.
 */
void usResetDelay(usDelay_t *delay){
    delay->running = false;
}

/*********************************************************************************************/

/**
 * @brief Manejador de errores genérico.
 *
 * Esta función se utiliza para manejar errores genéricos. En caso de error, enciende el LED2 y entra en un bucle infinito.
 */
static void Error_Handler(void)
{
    /* Turn LED2 on */
    BSP_LED_On(LED2);
    while (1)
    {
    }
}
