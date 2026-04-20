#ifndef STATUS_LED_H_
#define STATUS_LED_H_

#include "Led.h"

/* Init */
void StatusLED_Init(void);

/* Actions */
void LED_Success_On(void);
void LED_Success_Off(void);

void LED_Alarm_On(void);
void LED_Alarm_Off(void);

void Doorbell_LED_Pulse(void);
void Doorbell_LED_Off(void);

#endif