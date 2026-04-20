#include "Exti.h"

/*──────────────────────────────────────────────
 * Register Structs
 *──────────────────────────────────────────────*/
typedef struct {
    volatile uint32 IMR;
    volatile uint32 EMR;
    volatile uint32 RTSR;
    volatile uint32 FTSR;
    volatile uint32 SWIER;
    volatile uint32 PR;
} ExtiType;

typedef struct {
    volatile uint32 NVIC_ISER[8];   /* 0x000 – Interrupt Set-Enable   */
    uint32          _r1[24];
    volatile uint32 NVIC_ICER[8];   /* 0x080 – Interrupt Clear-Enable */
    uint32          _r2[24];
    volatile uint32 NVIC_ISPR[8];   /* 0x100 – Set Pending            */
    uint32          _r3[24];
    volatile uint32 NVIC_ICPR[8];   /* 0x180 – Clear Pending          */
    uint32          _r4[24];
    volatile uint32 NVIC_IABR[8];   /* 0x200 – Active Bit             */
    uint32          _r5[56];
    volatile uint8  NVIC_IPR[240];  /* 0x300 – Priority (1 byte/IRQ)  */
} NvicType;

typedef struct {
    uint32 MEMRMP;
    uint32 PMC;
    uint32 EXTICR[4];
    uint32 _r[2];
    uint32 CMPCR;
} SyscfgType;

/*──────────────────────────────────────────────
 * Peripheral Base Addresses
 *──────────────────────────────────────────────*/
#define EXTI    ((ExtiType*)  0x40013C00)
#define NVIC    ((NvicType*)  0xE000E100)
#define SYSCFG  ((SyscfgType*)0x40013800)

/*
 * STM32F4 implements 4 priority bits in the UPPER nibble of each IPR byte.
 * Writing (priority << 4) places the value in those bits.
 */
#define NVIC_PRIORITY_SHIFT   4U

/*──────────────────────────────────────────────
 * Internal State
 *──────────────────────────────────────────────*/
ExtiCallback ExtiCallbacks[16] = {0};

/*
 * Maps EXTI line → NVIC IRQ number (STM32F4xx)
 *   Lines 0-4   → dedicated IRQs 6-10
 *   Lines 5-9   → shared IRQ 23
 *   Lines 10-15 → shared IRQ 40
 */
static const uint8 ExtiLineNumberNvicMap[16] = {
    6, 7, 8, 9, 10,
    23, 23, 23, 23, 23,
    40, 40, 40, 40, 40, 40
};

/*──────────────────────────────────────────────
 * Public API
 *──────────────────────────────────────────────*/

void Exti_Init(uint8 LineNumber, uint8 PortName, uint8 EdgeType, ExtiCallback Callback) {
    uint8 sysConfigIndex            = LineNumber / 4;
    uint8 sysConfigLogicalBitPos    = LineNumber % 4;

    /* Route the GPIO port to this EXTI line via SYSCFG */
    SYSCFG->EXTICR[sysConfigIndex] &= ~(0x0FU << (sysConfigLogicalBitPos * 4U));
    SYSCFG->EXTICR[sysConfigIndex] |=  (PortName << (sysConfigLogicalBitPos * 4U));

    /* Register callback */
    ExtiCallbacks[LineNumber] = Callback;

    /* Clear both edge bits first, then set the requested one */
    EXTI->RTSR &= ~(0x01U << LineNumber);
    EXTI->FTSR &= ~(0x01U << LineNumber);

    switch (EdgeType) {
        case EXTI_EDGE_RISING:
            EXTI->RTSR |= (0x01U << LineNumber);
            break;
        case EXTI_EDGE_FALLING:
            EXTI->FTSR |= (0x01U << LineNumber);
            break;
        case EXTI_EDGE_BOTH:
            EXTI->RTSR |= (0x01U << LineNumber);
            EXTI->FTSR |= (0x01U << LineNumber);
            break;
        default:
            break;
    }
}

/**
 * Sets the NVIC preemption priority for the IRQ that serves this EXTI line.
 *
 * For the Emergency Reset button call:
 *     Exti_SetPriority(EXTI_EMERGENCY_RESET_LINE, EXTI_PRIORITY_HIGH);
 *
 * For the Door Bell button call:
 *     Exti_SetPriority(EXTI_DOOR_BELL_LINE, EXTI_PRIORITY_MEDIUM);
 *
 * Always call this BEFORE Exti_Enable().
 */
void Exti_SetPriority(uint8 LineNumber, uint8 Priority) {
    uint8 irqNumber = ExtiLineNumberNvicMap[LineNumber];
    NVIC->NVIC_IPR[irqNumber] = (uint8)(Priority << NVIC_PRIORITY_SHIFT);
}

void Exti_Enable(uint8 LineNumber) {
    EXTI->IMR |= (0x01U << LineNumber);
    uint8 irqNumber = ExtiLineNumberNvicMap[LineNumber];
    NVIC->NVIC_ISER[irqNumber / 32U] |= (0x01U << (irqNumber % 32U));
}

void Exti_Disable(uint8 LineNumber) {
    EXTI->IMR &= ~(0x01U << LineNumber);
    uint8 irqNumber = ExtiLineNumberNvicMap[LineNumber];
    NVIC->NVIC_ICER[irqNumber / 32U] |= (0x01U << (irqNumber % 32U));
}

/*──────────────────────────────────────────────
 * IRQ Handlers
 *──────────────────────────────────────────────*/

void EXTI0_IRQHandler(void) {
    if (ExtiCallbacks[0]) {
        ExtiCallbacks[0]();
    }
    EXTI->PR |= (0x01U << 0); /* Write 1 to clear pending bit */
}

void EXTI1_IRQHandler(void) {
    if (ExtiCallbacks[1]) {
        ExtiCallbacks[1]();
    }
    EXTI->PR |= (0x01U << 1);
}

void EXTI2_IRQHandler(void) {
    if (ExtiCallbacks[2]) {
        ExtiCallbacks[2]();
    }
    EXTI->PR |= (0x01U << 2);
}

void EXTI3_IRQHandler(void) {
    if (ExtiCallbacks[3]) {
        ExtiCallbacks[3]();
    }
    EXTI->PR |= (0x01U << 3);
}

void EXTI4_IRQHandler(void) {
    if (ExtiCallbacks[4]) {
        ExtiCallbacks[4]();
    }
    EXTI->PR |= (0x01U << 4);
}

void EXTI9_5_IRQHandler(void) {
    uint8 i;
    for (i = 5U; i <= 9U; i++) {
        if (EXTI->PR & (0x01U << i)) {
            if (ExtiCallbacks[i]) {
                ExtiCallbacks[i]();
            }
            EXTI->PR |= (0x01U << i); /* Clear AFTER calling handler */
        }
    }
}

void EXTI15_10_IRQHandler(void) {
    uint8 i;
    for (i = 10U; i <= 15U; i++) {
        if (EXTI->PR & (0x01U << i)) {
            if (ExtiCallbacks[i]) {
                ExtiCallbacks[i]();
            }
            EXTI->PR |= (0x01U << i); /* Clear AFTER calling handler */
        }
    }
}