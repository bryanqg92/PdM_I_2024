/*
 * valceControl.h
 *
 *  Created on: Apr 9, 2024
 *      Author: lean
 */

#ifndef INC_VALVECONTROL_H_
#define INC_VALVECONTROL_H_

#include <stdbool.h>

#include "stm32f4xx_hal.h"


#define DRAIN_VALVE_PORT		GPIOG
#define DRAIN_VALVE_PIN			GPIO_PIN_0

#define TANK_VALVE_PORT			GPIOG
#define TANK_VALVE_PIN			GPIO_PIN_1

#define EMERGENCY_VALVE_PORT	GPIOG
#define EMERGENCY_VALVE_PIN		GPIO_PIN_9


extern uint8_t valvesState;
typedef bool bool_t;

typedef enum{

	TANK_VALVE,
	DRAIN_VALVE,
	EMERGENCY_VALVE,
	VALVE_ERROR

}valveControllerState;

bool_t valveManagerInit(void);
void valveController(uint8_t level);


#endif /* INC_VALVECONTROL_H_ */
