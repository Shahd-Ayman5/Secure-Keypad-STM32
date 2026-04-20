#include "Keypad.h"
#include "../State_Machine/State_Machine.h"
#include "../Leds/Led_Bar.h"
#include "../7_Segment/7_Segment.h"
#include "../Leds/Status_Led.h"
#include "../Leds/Led.h"
#include "Rcc.h"
#include "Utils.h"
#include "../Exti/Exti.h"
#include "../Gpio/Gpio.h"

/* ISR: called when Emergency Reset button is pressed */
void Emergency_Reset_Handler(void)
{
    /* Set reset flag to be handled by the state machine on next update */
    StateMachine_SetResetFlag();
}

/* ISR: called when Door Bell button is pressed */
void DoorBell_Handler(void)
{
    /* Set doorbell flag to be handled by the state machine on next update */
    StateMachine_SetDoorbellFlag();
}

int main()
{
    uint8 key;
    uint8 lastKey = NO_KEY;

    /* Initialize system clocks */
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);
    Rcc_Enable(RCC_GPIOD);
    Rcc_Enable(RCC_SYSCFG);    /* Required for EXTI configuration */


    Exti_Init(11, EXTI_PORT_A, EXTI_EDGE_FALLING, Emergency_Reset_Handler);
    Exti_SetPriority(11, 0);    /* Lower priority than Emergency Reset */
    Exti_Enable(11);

    /* Door Bell Button on PE15 → EXTI Line 15 */
    Exti_Init(12, EXTI_PORT_A, EXTI_EDGE_FALLING, DoorBell_Handler);
    Exti_SetPriority(12, 1);    /* Lower priority than Emergency Reset */
    Exti_Enable(12);

    /* Initialize all peripherals */
    Keypad_Init();
    LEDBar_Init();
    SevenSeg_Init();
    StatusLED_Init();

    /* Initialize state machine to default LOCKED state */
    StateMachine_Init();

    while(1)
    {

        key = Keypad_Scan();

        /* Only update state machine on new key press */
        if(key != NO_KEY && lastKey == NO_KEY)
        {
            StateMachine_Update(key);
        }

        lastKey = key;
    }

    return 0;
}