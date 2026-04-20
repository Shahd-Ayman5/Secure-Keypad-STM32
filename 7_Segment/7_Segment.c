#include "Gpio.h"
#include "7_Segment.h"
#include "Bit_MATH.h"

uint8 sevenSegmentConnectionMap[7][2] = {
    {SEGMENT_A},
    {SEGMENT_B},
    {SEGMENT_C},
    {SEGMENT_D},
    {SEGMENT_E},
    {SEGMENT_F},
    {SEGMENT_G}
};

const uint8 sevenSegmentTableMap[10] = {
    0x3F, 0x06, 0x5B,
    0x4F, 0x66, 0x6D,
    0x7D, 0x07, 0x7F, 0x6F
};

void SevenSeg_Init(void)
{
    for (uint8 i = 0; i < 7; i++) {
        Gpio_Init(sevenSegmentConnectionMap[i][0],
                  sevenSegmentConnectionMap[i][1],
                  GPIO_OUTPUT,
                  GPIO_PUSH_PULL);

        /* Turn OFF initially */
        Gpio_WritePin(sevenSegmentConnectionMap[i][0],
                      sevenSegmentConnectionMap[i][1],
                      0);
    }
}

void SevenSeg_Display(uint8 number) {
    if (number > 9) {
        return;
    }
    uint8 value = 0;
    for (uint8 i = 0; i < 7; i++) {
        value = READ_BIT(sevenSegmentTableMap[number], i);
        // value = (sevenSegmentTableMap[number] >> i) & 0x01;
        Gpio_WritePin(sevenSegmentConnectionMap[i][0], sevenSegmentConnectionMap[i][1], value);
    }
}

void SevenSeg_Clear(void)
{
    for(uint8 i = 0; i < 7; i++)
    {
        Gpio_WritePin(sevenSegmentConnectionMap[i][0],
                      sevenSegmentConnectionMap[i][1],
                      0);
    }
}