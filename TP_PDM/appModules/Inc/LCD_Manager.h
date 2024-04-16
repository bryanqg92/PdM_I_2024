/*
 * LCD_Manager.h
 *
 *  Created on: Apr 7, 2024
 *      Author: lean
 */

#ifndef INC_LCD_MANAGER_H_
#define INC_LCD_MANAGER_H_

#define LEVEL_BUFFER_SIZE		8
#define VALVES_BUFFER_SIZE 		20
#define BLINKY_ERROR_TIME_MS	500

#include <stdint.h>

typedef enum{
	LCD,
	UART,
	LEVEL,
	VALVES
}app_SystemInit;

extern const char levelErrorIcon[8];
extern const char valveErrorIcon[8];

void initShow(uint8_t* msg, app_SystemInit module);
void endLCDShow(void);
void levelShow(void);
void levelErrorShow(void);
void valveShow(void);
void valvesErrorShow(void);

#endif /* INC_LCD_MANAGER_H_ */

