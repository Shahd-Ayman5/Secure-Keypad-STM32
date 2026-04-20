#ifndef EXTI_H
#define EXTI_H

#include "Std_Types.h"

/*──────────────────────────────────────────────
 * Line Numbers
 *──────────────────────────────────────────────*/
#define EXTI_LINE_0    0U
#define EXTI_LINE_1    1U
#define EXTI_LINE_2    2U
#define EXTI_LINE_3    3U
#define EXTI_LINE_4    4U
#define EXTI_LINE_5    5U
#define EXTI_LINE_6    6U
#define EXTI_LINE_7    7U
#define EXTI_LINE_8    8U
#define EXTI_LINE_9    9U
#define EXTI_LINE_10   10U
#define EXTI_LINE_11   11U
#define EXTI_LINE_12   12U
#define EXTI_LINE_13   13U
#define EXTI_LINE_14   14U
#define EXTI_LINE_15   15U

/*──────────────────────────────────────────────
 * Port Names (SYSCFG EXTICR mapping)
 *──────────────────────────────────────────────*/
#define EXTI_PORT_A     0U
#define EXTI_PORT_B     1U
#define EXTI_PORT_C     2U
#define EXTI_PORT_D     3U
#define EXTI_PORT_E     4U
#define EXTI_PORT_F     5U
#define EXTI_PORT_G     6U
#define EXTI_PORT_H     7U

/*──────────────────────────────────────────────
 * Edge Types
 *──────────────────────────────────────────────*/
#define EXTI_EDGE_RISING     0U
#define EXTI_EDGE_FALLING    1U
#define EXTI_EDGE_BOTH       2U

/*──────────────────────────────────────────────
 * NVIC Priority Levels
 * STM32F4 uses the upper 4 bits of the priority byte.
 * Lower value = higher priority.
 *──────────────────────────────────────────────*/
#define EXTI_PRIORITY_HIGH      0U   /* Used for Emergency Reset */
#define EXTI_PRIORITY_MEDIUM    5U   /* Used for Door Bell       */
#define EXTI_PRIORITY_LOW      10U

/*──────────────────────────────────────────────
 * Project-Specific Button Assignments
 * Change the line numbers to match your wiring.
 *──────────────────────────────────────────────*/
#define EXTI_EMERGENCY_RESET_LINE    EXTI_LINE_0   /* PB0  – change if needed */
#define EXTI_EMERGENCY_RESET_PORT    EXTI_PORT_B

#define EXTI_DOOR_BELL_LINE          EXTI_LINE_1   /* PB1  – change if needed */
#define EXTI_DOOR_BELL_PORT          EXTI_PORT_B


typedef void (*ExtiCallback)(void);

/*──────────────────────────────────────────────
 * API
 *──────────────────────────────────────────────*/

/**
 * @brief  Configures an EXTI line (port mux, edge, callback).
 *         Does NOT enable the interrupt – call Exti_Enable() separately.
 */
void Exti_Init(uint8 LineNumber, uint8 PortName, uint8 EdgeType, ExtiCallback Callback);

/**
 * @brief  Sets the NVIC interrupt priority for a given EXTI line.
 *         Must be called BEFORE Exti_Enable().
 * @param  Priority  0 (highest) … 15 (lowest) – use EXTI_PRIORITY_* macros.
 */
void Exti_SetPriority(uint8 LineNumber, uint8 Priority);

/**
 * @brief  Enables the EXTI line in both EXTI IMR and NVIC.
 */
void Exti_Enable(uint8 LineNumber);

/**
 * @brief  Disables the EXTI line in both EXTI IMR and NVIC.
 */
void Exti_Disable(uint8 LineNumber);

#endif /* EXTI_H */
