#include "State_Machine.h"
#include "Keypad.h"
#include "../Leds/Led_Bar.h"
#include "../Leds/Status_Led.h"
#include "../7_Segment/7_Segment.h"
#include "Utils.h"

/* States */
typedef enum {
    LOCKED,
    UNLOCKED,
    ALARM
} SystemState;

/* System state variables */
static SystemState state = LOCKED;                      /* Current state of the system */
static char correctPassword[4] = {'1','2','3','4'};    /* Stored correct password */
static char enteredPassword[4];                         /* Buffer for user-entered digits */
static uint8 index = 0;                                 /* Current digit position in buffer */
static uint8 failCount = 0;                             /* Number of consecutive failed attempts */

/* Flags set by EXTI callbacks (volatile - written from ISR) */
static volatile uint8 doorbell_flag = 0;    /* Set when doorbell button is pressed */
static volatile uint8 reset_flag    = 0;    /* Set when emergency reset button is pressed */

/* ========================= */

/* Sets the doorbell flag to notify the state machine of a press event */
void StateMachine_SetDoorbellFlag(void)
{
    Doorbell_LED_Pulse();
}

/* Sets the reset flag to trigger a system restart from any state */
void StateMachine_SetResetFlag(void)
{
    reset_flag = 1;
    StateMachine_Init();
}

/* Getter function to check if reset flag is set */
uint8 StateMachine_IsResetFlagSet(void)
{
    return reset_flag;
}

/* Initialize the system to its default (LOCKED) state */
void StateMachine_Init(void)
{
    state         = LOCKED;    /* Set initial state to LOCKED */
    index         = 0;         /* Reset entered password index */
    failCount     = 0;         /* Reset failure counter */
    reset_flag    = 0;         /* Clear emergency reset flag */

    LEDBar_Clear();            /* Clear progress indicator LEDs */
    SevenSeg_Display(0);       /* Reset failure display to 0 */
    LED_Success_Off();         /* Turn off success indicator */
    LED_Alarm_Off();           /* Turn off alarm indicator */
    Doorbell_LED_Off();
}

/* Update state machine based on keypad input and external events */
void StateMachine_Update(uint8 key)
{
    /* Handle asynchronous events FIRST (interrupt-driven) */

    /* Emergency Reset: restore system to initial state from any state */
    // if(reset_flag)
    // {
    //     StateMachine_Init();     /* Reinitialize all variables and outputs */
    //     return;                  /* Exit immediately after reset */
    // }

    /* Doorbell event: trigger momentary indication without affecting lock state */
    // if(doorbell_flag)
    // {
    //     doorbell_flag = 0;       /* Clear doorbell flag */
    //     Doorbell_LED_Pulse();    /* Activate doorbell indicator briefly */
    // }

    /* If no key is pressed, skip further processing */
    // if(key == NO_KEY) return;

    switch(state)
    {
        
        /* ================= LOCKED ================= */
        case LOCKED:

            /* Store key only if there is space in the buffer */
            if(index < 4)
            {
                enteredPassword[index] = key;    /* Save entered digit to buffer */
                index++;                          /* Advance to next buffer position */
                LEDBar_SetLevel(index);           /* Light up one more LED on progress bar */
            }

            /* Once 4 digits are entered, validate the full sequence */
            if(index == 4)
            {
                uint8 passwordCorrect = 1;    /* Assume correct until mismatch found */
                uint8 i;

                /* Compare each entered digit against the stored password */
                for(i = 0; i < 4; i++)
                {
                    if(enteredPassword[i] != correctPassword[i])
                    {
                        passwordCorrect = 0;    /* Mark as incorrect on first mismatch */
                        break;
                    }
                }

                delay_ms(700);    /* Brief delay to give user time to see LED feedback */

                /* Correct password: transition to UNLOCKED */
                if(passwordCorrect)
                {
                    state     = UNLOCKED;    /* Move to UNLOCKED state */
                    failCount = 0;           /* Clear failure history on success */
                    LED_Success_On();        /* Turn on success indicator LED */
                }
                /* Wrong password: register failure and check lockout threshold */
                else
                {
                    failCount++;             /* Increment consecutive failure counter */

                    /* Lockout threshold reached: trigger alarm */
                    if(failCount >= 3)
                    {
                        state = ALARM;       /* Move to ALARM state */
                        LED_Alarm_On();      /* Turn on alarm indicator LED */
                    }
                }

                /* Reset input buffer for the next attempt */
                index = 0;
                LEDBar_Clear();                 /* Clear all progress indicator LEDs */
                SevenSeg_Display(failCount);    /* Update 7-segment with latest fail count */
            }
            break;

        /* ================= UNLOCKED ================= */
        case UNLOCKED:

            /* Lock command received: return to LOCKED state */
            if(key == '#')
            {
                state = LOCKED;        /* Transition back to LOCKED state */
                LED_Success_Off();     /* Turn off success indicator */
                LEDBar_Clear();        /* Clear any residual progress LEDs */
                index = 0;             /* Reset digit index for fresh entry */
            }
            /* All other keys are ignored while unlocked */
            break;

        /* ================= ALARM ================= */
        case ALARM:

            /* Ignore all keypad input while in ALARM state */
            /* Only an emergency reset (handled above) can exit this state */
            break;
    }
}