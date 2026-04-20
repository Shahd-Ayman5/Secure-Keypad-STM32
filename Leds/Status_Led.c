#include "Status_Led.h"
#include "Gpio.h"

/* Define LEDs (hardware mapping) */
static LED_t successLED = {GPIO_A, 0};
static LED_t alarmLED   = {GPIO_A, 1};
static LED_t DoorbellLED   = {GPIO_A, 2};



/* ================= INIT ================= */
void StatusLED_Init(void)
{
    LED_Init(&successLED);
    LED_Init(&alarmLED);
    LED_Init(&DoorbellLED);

    LED_Off(&successLED);
    LED_Off(&alarmLED);
    LED_Off(&DoorbellLED);
}

/* ================= SUCCESS ================= */
void LED_Success_On(void)
{
    LED_On(&successLED);
}

void LED_Success_Off(void)
{
    LED_Off(&successLED);
}

/* ================= ALARM ================= */
void LED_Alarm_On(void)
{
    LED_On(&alarmLED);
}

void LED_Alarm_Off(void)
{
    LED_Off(&alarmLED);
}

void Doorbell_LED_Pulse(void) {
    LED_Toggle(&DoorbellLED);
}

void Doorbell_LED_Off(void) {
    LED_Off(&DoorbellLED);
}