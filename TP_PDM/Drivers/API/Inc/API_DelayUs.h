/*
 * API_DelayUs.h
 *
 *  Created on: Apr 3, 2024
 *      Author: lean
 */

#ifndef API_INC_API_DELAYUS_H_
#define API_INC_API_DELAYUS_H_

#include <stdint.h>
#include <stdbool.h>

typedef bool bool_t;

typedef struct {
   uint32_t startTime;
   uint32_t duration;
   bool_t running;
} usDelay_t;

bool_t Timer_Init(void);
uint32_t getUsTick(void);
void TimerStart(void);

void usDelayInit(usDelay_t *delay, uint32_t duration);
bool_t usDelayRead(usDelay_t *delay);
void usDelayWrite(usDelay_t *delay, uint32_t duration);
void usResetDelay(usDelay_t *delay);

void usDelay(uint32_t delay);

#endif /* API_INC_API_DELAYUS_H_ */
