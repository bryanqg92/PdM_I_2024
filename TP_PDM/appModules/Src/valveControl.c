/*
 * valveControl.c
 *
 *  Created on: Apr 9, 2024
 *      Author: lean
 */

/**
 * @file valveControl.c
 * @brief Implementación del controlador de válvulas para el sistema de gestión de nivel de agua.
 *
 * Este archivo contiene la implementación del controlador de válvulas para el sistema de gestión de nivel de agua.
 * Se proporcionan funciones para inicializar el controlador de válvulas y controlar el estado de las válvulas en
 * función del nivel de agua medido. Además, se incluyen funciones internas para la gestión de errores y la
 * inicialización de pines de las válvulas.
 *
 * @date Apr 9, 2024
 * @author lean
 */

#include <math.h>

#include "valveControl.h"
#include "API_Delay.h"
#include "tankLevel.h"

static const uint16_t TIME_CHANGE_LEVEL = 10000; 
static const float DELTA_LEVEL = 1.0; /**< Diferencia de nivel máxima tolerada para la detección de emergencia. EN CM*/

uint8_t valvesState;            //EXTERN 
static delay_t changeLevel; 
static float previousLevel; 
static bool_t emergencyValve; 

static bool_t valveErrorHandler(void); 
static void initValvePin(GPIO_TypeDef *port, uint16_t pin); 

/**
 * @brief Inicializa el controlador de válvulas.
 *
 * Esta función inicializa el controlador de válvulas. Configura los pines de las válvulas (drenaje, tanque y emergencia)
 * en el modo adecuado y establece el estado inicial de las válvulas. Además, inicializa la estructura de retardo
 * para el cambio de nivel.
 *
 * @return true si la inicialización es exitosa, false en caso contrario.
 */
bool_t valveManagerInit(void) {

    initValvePin(DRAIN_VALVE_PORT, DRAIN_VALVE_PIN);
    initValvePin(TANK_VALVE_PORT, TANK_VALVE_PIN);
    initValvePin(EMERGENCY_VALVE_PORT, EMERGENCY_VALVE_PIN);

    delayInit(&changeLevel, TIME_CHANGE_LEVEL);

    previousLevel = distance;

    return true;
}

/**
 * @brief Controla el estado de las válvulas en función del nivel de agua.
 *
 * Esta función controla el estado de las válvulas en función del nivel de agua medido. Determina si es necesario
 * activar la válvula de emergencia, la válvula de drenaje o mantener la válvula del tanque abierta.
 *
 * @param level Nivel de agua medido.
 */
void valveController(uint8_t level) {

    if (valveErrorHandler()) {
        valvesState = VALVE_ERROR;
        HAL_GPIO_WritePin(TANK_VALVE_PORT, TANK_VALVE_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(EMERGENCY_VALVE_PORT, EMERGENCY_VALVE_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(DRAIN_VALVE_PORT, DRAIN_VALVE_PIN, GPIO_PIN_SET);
    }
    else {
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

/**
 * @brief Maneja los errores de válvula.
 *
 * Esta función maneja los errores relacionados con el control de válvulas. Detecta si se ha producido una emergencia
 * basada en la falta de cambio de nivel esperado y activa la válvula de emergencia en caso necesario.
 *
 * @return true si se detecta una emergencia, false en caso contrario.
 */
static bool_t valveErrorHandler(void) {
    if (delayRead(&changeLevel)) {
        emergencyValve = false;
        float currentLevel = distance;
        if (fabs(currentLevel - previousLevel) < DELTA_LEVEL) {
            emergencyValve = true;
        }
        previousLevel = currentLevel;
    }
    return emergencyValve;
}

/**
 * @brief Inicializa un pin de válvula.
 *
 * Esta función inicializa un pin de válvula especificado por el puerto y el pin dados.
 *
 * @param port Puerto GPIO al que pertenece el pin de la válvula.
 * @param pin Pin GPIO de la válvula.
 */
static void initValvePin(GPIO_TypeDef *port, uint16_t pin) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}
