#include "STM32_reg.h"
#include "SysTick.h"
void Delay_us(uint32_t us)
{
    SysTick->LOAD = 16 - 1;  // Giả sử HSI 16MHz chưa cấu hình PLL, 16 tick = 1us
    SysTick->VAL  = 0;
    SysTick->CTRL = (1 << 2) | (1 << 0); // CLKSOURCE = 1 (AHB), ENABLE = 1

    for (uint32_t i = 0; i < us; i++) {
        SysTick->LOAD = 15;
        SysTick->VAL = 0;
        while ((SysTick->CTRL & (1 << 16)) == 0); // chờ COUNTFLAG
    }
}