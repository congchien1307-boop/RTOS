#include "STM32_reg.h"
#include "EXTI.h"
#include "GPIO.h"
#include "SX128.h"
#include "FreeRTOS.h"
#include "SPI.h"
#include "semphr.h"
extern SemaphoreHandle_t xLoRaTxDoneSem;
extern void xPortSysTickHandler(void);
extern void xPortPendSVHandler(void);
extern void vPortSVCHandler(void);

void SysTick_Handler(void) { xPortSysTickHandler(); }
void PendSV_Handler(void)  { xPortPendSVHandler(); }
void SVC_Handler(void)     { vPortSVCHandler(); }

extern void NVIC_EnableIRQ_Reg(uint8_t irqNumber)
{
    NVIC_ISER0 |= (1UL << irqNumber);   /* set bit tương ứng -> bật ngắt đó trong NVIC */
}

extern void NVIC_SetPriority_Reg(uint8_t irqNumber, uint8_t priority)
{
    /* Mỗi IRQ có 1 byte priority riêng, địa chỉ = base + irqNumber.
     * STM32F4 chỉ dùng 4 bit cao của byte đó (do configPRIO_BITS=4),
     * nên phải dịch priority lên 4 bit trước khi ghi. */
    volatile uint8_t *ipr = (volatile uint8_t*)(NVIC_IPR_BASE + irqNumber);
    *ipr = (uint8_t)(priority << 4);
}

void EXTI0_DIO0_Init(void)
{
    /* Chân PA0 làm input, không kéo lên/xuống (DIO0 tự kéo bởi SX1278) */
    RCC->AHB1ENR |= RCC_AHB1_AHB1ENR_GPIOAEN;
    GPIOA->MODER &= ~(1 << GPIO_MODER_0_POS);   /* 00 = input, cần thêm macro MODER0 nếu chưa có */

    /* Map EXTI line 0 -> Port A (SYSCFG_EXTICR1 bit [3:0] = 0000 nghĩa là PA0) */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR1 &= ~(0xF << 0);     /* bit [3:0] của EXTICR1 ứng với EXTI0, set = 0 -> Port A */

    EXTI->IMR  |= EXTI_IMR_MR0;    /* bật interrupt mask cho line 0 */
    EXTI->RTSR |= EXTI_RTSR_TR0;   /* trigger cạnh lên (DIO0 kéo cao khi TxDone) */

    NVIC_EnableIRQ_Reg(EXTI0_IRQn);
    NVIC_SetPriority_Reg(EXTI0_IRQn, 5);   /* phải <= configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY (=5) để gọi FromISR API an toàn */
}

/* Đặt trong file ngắt (ví dụ stm32f4xx_it.c hoặc file bạn tự tạo cho ngắt) */
void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR0) {
        EXTI->PR = EXTI_PR_PR0;   /* xoá cờ pending - GHI 1 để xoá, không phải ghi 0 */
        WriteReg(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);   /* clear cờ TxDone bên trong chip SX1278 */

        BaseType_t xHigherPrioTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(xLoRaTxDoneSem, &xHigherPrioTaskWoken);
        portYIELD_FROM_ISR(xHigherPrioTaskWoken);
    }
}
/* ===================== NVIC (base cố định theo kiến trúc ARM Cortex-M) ===================== */
#define NVIC_ISER0   (*(volatile uint32_t*)0xE000E100)   /* Interrupt Set-Enable Register 0, IRQ 0-31 */
#define NVIC_IPR_BASE  0xE000E400UL                       /* Interrupt Priority Register, 1 byte/IRQ */

/* STM32F4: EXTI0_IRQn = 6 (theo bảng vector table trong Reference Manual RM0368,
 * mục Interrupt and exception vectors — SỐ NÀY CỐ ĐỊNH do ST định nghĩa cho dòng F4,
 * không đổi theo board, nhưng đổi theo dòng chip khác, vd F1/F7 số IRQ khác). */
