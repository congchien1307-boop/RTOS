#include "STM32_reg.h"
#include "SystemClock_config.h"
extern void SystemClock_config(void){
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));
    FLASH->ACR = FLASH_ARC_DCEN | FLASH_ARC_ICEN | FLASH_ARC_LATECY_2WS | FLASH_ARC_PRFTEN;
    RCC->PLLCFGR = (RCC_PLLCFGR_PLLM_Pos|RCC_PLLCFGR_PLLN_Pos|RCC_PLLCFGR_PLLP_Pos|RCC_PLLCFGR_PLLSRC_Pos);
    RCC->CR |= RCC_CR_PLLON; // turn on PLL
    RCC->CFGR |= RCC_CFGR_PPRE2;
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    
    
}