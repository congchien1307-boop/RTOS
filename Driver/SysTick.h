#include "STM32_reg.h"
#ifndef SYSTICK_H
#define SYSTICK_H
#define SYSTICK_BASE   0xE000E010UL

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_Typedef;

#define SysTick  ((SysTick_Typedef *) SYSTICK_BASE)
void Delay_us(uint32_t us);
#endif // SYSTICK_H