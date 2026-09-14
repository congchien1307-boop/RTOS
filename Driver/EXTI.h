#include "STM32_reg.h"
#define EXTI_IMR_MR0            (1 << 0)
#define EXTI_RTSR_TR0             (1 << 0)
#define EXTI_PR_PR0                (1 << 0)
#define EXTI0_IRQn
#define IRQ_TX_DONE_MASK   0x08
#define REG_IRQ_FLAGS            0x12
#define EXTI_IMR_MR22    (1 << 22)
#define EXTI_RTSR_TR22   (1 << 22)
#define EXTI_PR_PR22     (1 << 22)
#define EXTI0_IRQn   6
typedef struct 
{
    volatile uint32_t  IMR;
    volatile uint32_t  RTSR;
    volatile uint32_t  PR;
}EXTI_TypeDef;
typedef struct 
{
    volatile uint32_t EXTICR1;
}SYSCFG_TypeDef;
extern void NVIC_EnableIRQ_Reg(uint8_t irqNumber);
extern void NVIC_SetPriority_Reg(uint8_t irqNumber, uint8_t priority);


