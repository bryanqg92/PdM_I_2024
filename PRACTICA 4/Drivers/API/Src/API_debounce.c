/*
 * API_debounce.c
 *
 *  Created on: Mar 21, 2024
 *      Author: lean
 */
#include <stdint.h>
#include <stdbool.h>

#include <API_Debounce.h>
#include <API_Delay.h>

#include "stm32f4xx_nucleo_144.h"

typedef enum {
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

static debounceState_t fsmState;
static bool_t isKeyPressed = false;

delay_t debounceInit;

void debounceFSM_init(uint32_t debounceTime) {
	fsmState = BUTTON_UP;
	delayInit(&debounceInit, debounceTime);
}

void debounceFSM_update() {

	bool_t buttonState = BSP_PB_GetState(BUTTON_USER);


	switch(fsmState) {

	case BUTTON_UP:
		if (buttonState == GPIO_PIN_SET){
			fsmState = BUTTON_FALLING;
			isKeyPressed = false;
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
		if (buttonState == GPIO_PIN_SET){}
		else if (buttonState == GPIO_PIN_RESET){
			fsmState = BUTTON_RAISING;
		}
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
	}
}

bool_t readKey() {
	bool_t read = isKeyPressed;
	isKeyPressed = false;
	return read;
}

void buttonPressed() {
	BSP_LED_On(LED3);
}

void buttonReleased() {
	BSP_LED_Off(LED3);
}
