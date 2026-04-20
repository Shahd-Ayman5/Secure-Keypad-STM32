#include "Led.h"
#include "Gpio.h"
#include "Utils.h"

void LED_Init(const LED_t* led)
{
    Gpio_Init(led->port, led->pin, GPIO_OUTPUT, GPIO_PUSH_PULL);
    LED_Off(led);
}

void LED_On(const LED_t* led)
{
    Gpio_WritePin(led->port, led->pin, HIGH);
}

void LED_Off(const LED_t* led)
{
    Gpio_WritePin(led->port, led->pin, LOW);
}

void LED_Toggle(const LED_t* led) {

    Gpio_WritePin(led->port, led->pin, HIGH);
    delay_Toggle_ms(200);
    Gpio_WritePin(led->port, led->pin, LOW);

}
