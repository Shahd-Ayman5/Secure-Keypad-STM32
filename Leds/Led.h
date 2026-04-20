#ifndef LED_H_
#define LED_H_

#include "Std_Types.h"

typedef struct {
    uint8 port;
    uint8 pin;
} LED_t;

void LED_Init(const LED_t* led);
void LED_On(const LED_t* led);
void LED_Off(const LED_t* led);
void LED_Toggle(const LED_t* led);

#endif