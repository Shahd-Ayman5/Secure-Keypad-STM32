#include "Utils.h"
#include "../State_Machine/State_Machine.h"

void delay_ms(uint32 ms) {
    for (uint32 i = 0; i < ms * 1000; i++) {
    }
}

void delay_Toggle_ms(uint32 ms) {
    for (uint32 i = 0; i < ms * 1000; i++) {
        if (StateMachine_IsResetFlagSet() == 1)
        {
            break;
        }
    }
}