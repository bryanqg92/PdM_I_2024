
/*
 * API_debounce.c
 *
 *  Created on: Mar 21, 2024
 *      Author: lean
 */

#ifndef API_DEBOUNCE_H_
#define API_DEBOUNCE_H_

#include <stdint.h>
#include <stdbool.h>

#include "API_Delay.h"


/**
 * @file API_Debounce.h
 * @brief Header file for debounce functionality.
 */

/**
 * @brief Initialize the debounce Finite State Machine (FSM).
 * @param debounceTime The debounce time in milliseconds.
 */
void debounceFSM_init(uint32_t debounceTime);



/**
 * @brief Update the debounce FSM. This function should be called periodically.
 */
void debounceFSM_update();

/**
 * @brief Read the state of the button.
 * @return True if the button is pressed, false otherwise.
 */
bool_t readKey();


void buttonPressed();
void buttonReleased();

#endif /* API_DEBOUNCE_H_ */
