#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "Std_Types.h"

void StateMachine_Init(void);
void StateMachine_Update(uint8 key);

/* EXTI callback setters:
 * Called from interrupt handlers to signal events to the state machine
 */
void StateMachine_SetDoorbellFlag(void);
void StateMachine_SetResetFlag(void);

/* Getter function to check reset flag state */
uint8 StateMachine_IsResetFlagSet(void);

#endif
