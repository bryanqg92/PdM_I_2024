/*
 * tankLevel.h
 *
 *  Created on: Apr 1, 2024
 *      Author: lean
 */

#ifndef INC_TANKLEVEL_H_
#define INC_TANKLEVEL_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define TRIGGER_PIN             GPIO_PIN_2
#define TRIGGER_PORT            GPIOG
#define ECHO_PIN                GPIO_PIN_3
#define ECHO_PORT               GPIOG

typedef bool bool_t;

bool_t tankLevelInit(void);
void get_current_water_level(void);

extern float distance;
extern uint8_t levelResult;

typedef enum {
    LEVEL_ZERO,
    LEVEL_ONE,
    LEVEL_TWO,
    LEVEL_THREE,
    LEVEL_FOUR,
    LEVEL_FIVE,
    LEVEL_ERROR
} tank_Levels;

#endif /* INC_TANKLEVEL_H_ */
