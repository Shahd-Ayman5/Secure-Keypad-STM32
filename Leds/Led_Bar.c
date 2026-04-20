#include "Led.h"
#include "Led_Bar.h"
#include "Gpio.h"

/* =========================
   Configuration
   ========================= */

#define LED_SIZE 4   // based on number of digits in password

/* Define LEDs as LED_t structs */
static LED_t ledBar[LED_SIZE] = {
    {GPIO_B, 0},
    {GPIO_B, 1},
    {GPIO_B, 2},
    {GPIO_B, 3}
};

/* =========================
    Init
   ========================= */
void LEDBar_Init(void)
{
    for (uint8 i = 0; i < LED_SIZE; i++)
    {
        LED_Init(&ledBar[i]);
        LED_Off(&ledBar[i]);
    }
}

/* =========================
    Set Level
   ========================= */
void LEDBar_SetLevel(uint8 level)
{
    if (level > LED_SIZE)
        level = LED_SIZE;

    for (uint8 i = 0; i < LED_SIZE; i++)
    {
        if (i < level)
        {
            LED_On(&ledBar[i]);
        }
    }
}

/* =========================
    Clear
   ========================= */
void LEDBar_Clear(void)
{
    for (uint8 i = 0; i < LED_SIZE; i++)
    {
        LED_Off(&ledBar[i]);
    }
}