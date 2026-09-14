#include "STM32_reg.h"
#include "FreeRTOS.h"
#include "RTC.h"
#include "EXTI.h"
#include "SysTick.h"
#include "task.h"
#include "SystemClock_config.h"
extern void vApplicationSleep(TickType_t xExpectedIdleTime)
{
    /* Vô hiệu hoá ngắt trước khi kiểm tra điều kiện ngủ - tránh race condition
     * (1 ngắt xảy ra đúng lúc đang quyết định ngủ hay không) */
    __asm volatile ("cpsid i" ::: "memory");

    /* Hỏi lại chính FreeRTOS: "vẫn còn an toàn để ngủ không?" - vì giữa lúc
     * Idle Task gọi hàm này và lúc mình vô hiệu hoá ngắt ở trên, có thể đã
     * có task khác được unblock (event xảy ra), nếu vậy KHÔNG được ngủ */
    if (eTaskConfirmSleepModeStatus() == eAbortSleep) {
        __asm volatile ("cpsie i" ::: "memory");
        return;
    }

    if (xExpectedIdleTime >= pdMS_TO_TICKS(5)) {
        /* --- Toàn bộ nội dung PowerMgmt_PreSleep() cũ đưa vào đây --- */
        uint32_t wutCounts = (uint32_t)(((uint64_t)(xExpectedIdleTime - 1) * 2048U) / 1000U);
        if (wutCounts == 0)      wutCounts = 1;
        if (wutCounts > 0xFFFF)  wutCounts = 0xFFFF;

        RTC->WPR = 0xCA; RTC->WPR = 0x53;
        RTC->CR &= ~RTC_CR_WUTE;
        while (!(RTC->ISR & RTC_ISR_WUTWF)) { }
        RTC->WUTR = wutCounts;
        RTC->CR = (RTC->CR & ~0x7U) | RTC_CR_WUCKSEL_DIV16;
        RTC->CR |= RTC_CR_WUTE | RTC_CR_WUTIE;
        RTC->WPR = 0xFF;
        RTC->ISR &= ~RTC_ISR_WUTF;
        EXTI->PR = EXTI_PR_PR22;

        SysTick->CTRL &= ~1U;

        PWR->CR &= ~PWR_CR_PDDS;
        PWR->CR |= PWR_CR_LPDS;
        SCB_SCR |= SCB_SCR_SLEEPDEEP;

        __asm volatile ("cpsie i" ::: "memory");  /* bật lại ngắt NGAY TRƯỚC wfi - bắt buộc, để wfi vẫn được đánh thức */
        __asm volatile ("wfi");
        __asm volatile ("cpsid i" ::: "memory");  /* tắt lại ngay khi vừa tỉnh, tránh ngắt xen vào lúc đang phục hồi clock */

        /* --- Toàn bộ nội dung PowerMgmt_PostSleep() cũ đưa vào đây --- */
        SCB_SCR &= ~SCB_SCR_SLEEPDEEP;
        SystemClock_config();
        SysTick->CTRL |= 1U;

        if (xExpectedIdleTime > 0) {
            vTaskStepTick(xExpectedIdleTime - 1);
        }
    }

    __asm volatile ("cpsie i" ::: "memory");   /* bật lại ngắt trước khi trả quyền lại cho Idle Task */
}