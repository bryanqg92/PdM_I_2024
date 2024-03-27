/*
 * API_debounce.c
 *
 *  Created on: Mar 21, 2024
 *      Author: lean
 */

/**
 * @file API_debounce.c
 * @brief Implementación del módulo de debounce.
 */

#include <stdint.h>
#include <stdbool.h>

#include <API_Debounce.h>
#include <API_Delay.h>

#include "stm32f4xx_nucleo_144.h"

/**
 * @brief Estados del estado de la máquina de estado del debounce.
 */
typedef enum {
	BUTTON_UP,      
	BUTTON_FALLING, 
	BUTTON_DOWN,   
	BUTTON_RAISING 
} debounceState_t;

static debounceState_t fsmState; 
static bool_t isKeyPressed = false; /**< Estado actual del botón (presionado o no). */

bool_t buttonState; 				/**< Estado actual del botón físico. */
delay_t debounceInit; 

/**
 * @brief Manejador de errores de la máquina de estado.
 */
static void FSM_error_handler(void){

	buttonState = BSP_PB_GetState(BUTTON_USER);

	if (buttonState == GPIO_PIN_RESET){
		isKeyPressed = false;
		fsmState = BUTTON_UP;
	}
	else{

		while (buttonState == GPIO_PIN_SET)
		{
			BSP_LED_On(LED2);
		}
	}

}

/**
 * @brief Inicializa la máquina de estado del debounce.
 * @param debounceTime Tiempo de debounce en milisegundos.
 */
void debounceFSM_init(uint32_t debounceTime) {
	fsmState = BUTTON_UP;
	delayInit(&debounceInit, debounceTime);
}

/**
 * @brief Actualiza la máquina de estado del debounce.
 */
void debounceFSM_update() {

	buttonState = BSP_PB_GetState(BUTTON_USER);


	switch(fsmState) {

	case BUTTON_UP:

		if (buttonState == GPIO_PIN_SET){
			fsmState = BUTTON_FALLING;
		}
		break;


	case BUTTON_FALLING:

		if (delayRead(&debounceInit)) {
			fsmState = BUTTON_DOWN;
			isKeyPressed = true;
			buttonPressed();
		}
		else {
			fsmState = BUTTON_UP;
		}
		break;

	case BUTTON_DOWN:

		fsmState = (buttonState == GPIO_PIN_RESET) ? BUTTON_RAISING : fsmState;
		break;

	case BUTTON_RAISING:

		if (delayRead(&debounceInit)) {
			fsmState = BUTTON_UP;
			isKeyPressed = false;
			buttonReleased();
		}
		else {
			fsmState = BUTTON_DOWN;
		}

		break;


	default:
		FSM_error_handler();
		break;
	}
}

/**
 * @brief Lee el estado del botón.
 * @return Estado del botón (presionado o no).
 */
bool_t readKey() {
	bool_t read = isKeyPressed;
	isKeyPressed = false;
	return read;
}

/**
 * @brief Manejador de evento para el botón presionado.
 */
void buttonPressed() {
	BSP_LED_On(LED3);
}

/**
 * @brief Manejador de evento para el botón liberado.
 */
void buttonReleased() {
	BSP_LED_Off(LED3);
}
