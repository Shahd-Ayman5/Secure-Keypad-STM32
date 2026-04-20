#include <Std_Types.h>
#include "Gpio.h"
#include "Gpio_Private.h"
#include "../LIB/Bit_MATH.h"

uint32 addressMap[8] = {GPIOA_BASE_ADDR, GPIOB_BASE_ADDR, GPIOC_BASE_ADDR, GPIOD_BASE_ADDR, GPIOE_BASE_ADDR, GPIOF_BASE_ADDR, GPIOG_BASE_ADDR, GPIOH_BASE_ADDR};

void Gpio_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState) {

    uint8 addressIndex = PortName - GPIO_A;

    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];

    gpioDevice->GPIO_MODER  &= ~(0x03 << (PinNumber * 2));
    gpioDevice->GPIO_MODER |= (PinMode << (PinNumber * 2));

    if (PinMode == GPIO_INPUT) {
        gpioDevice->GPIO_PUPDR &= ~(0x03 << (PinNumber * 2));
        gpioDevice->GPIO_PUPDR |= (DefaultState << (PinNumber * 2));
    } else {
        gpioDevice->GPIO_OTYPER &= ~(0x1 << PinNumber);
        gpioDevice->GPIO_OTYPER |= (DefaultState << (PinNumber));
    }
}

uint8 Gpio_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data) {
    uint8 status = NOK;
    uint8 addressIndex = PortName - GPIO_A;
    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];
    uint8 pinMode = READ_FIELD(gpioDevice->GPIO_MODER, 0x03, PinNumber * 2);
    if (pinMode == GPIO_OUTPUT || pinMode == GPIO_AF) {
        gpioDevice->GPIO_ODR &= ~(0x1U << PinNumber);
        gpioDevice->GPIO_ODR  |= (Data << PinNumber);
        status = OK;
    }
    return status;
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNum) {
    uint8 data = 0;
    uint8 addressIndex = PortName - GPIO_A;
    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];
    data = READ_FIELD(gpioDevice->GPIO_IDR, 0x1, PinNum);
    return data;
}
