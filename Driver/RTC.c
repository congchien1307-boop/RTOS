#include "STM32_reg.h"
#include "SystemClock_config.h"
#include "RTC.h"
#include "EXTI.h"
void RTC_Init_Reg(void)
{
    /* 1. Bật clock PWR interface + cho phép ghi vào backup domain */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;

    /* 2. Bật LSE 32.768kHz làm nguồn cho RTC */
    RCC->BDCR |= RCC_BDCR_LSEON;
    while (!(RCC->BDCR & RCC_BDCR_LSERDY)) { }

    /* 3. Chọn LSE làm RTCCLK, bật RTC */
    RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
    RCC->BDCR |= RCC_BDCR_RTCEN;

    /* 4. Mở khoá ghi thanh ghi RTC (2 bước ghi cố định theo datasheet) */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    /* 5. Vào chế độ Init để cấu hình prescaler + giờ ban đầu */
    RTC->ISR |= RTC_ISR_INIT;
    while (!(RTC->ISR & RTC_ISR_INITF)) { }

    /* 6. Prescaler: LSE 32768Hz -> chia async 128, sync 256 -> ra đúng 1Hz cho bộ đếm giây */
    RTC->PRER = (127U << RTC_PRER_PREDIV_A_Pos) | (255U << RTC_PRER_PREDIV_S_Pos);

    /* 7. Giờ/ngày ban đầu để 0 - epoch sẽ không chính xác tuyệt đối, chỉ dùng để
     * tính khoảng cách thời gian tương đối giữa các lần đo, đủ cho tracker */
    RTC->TR = 0;
    RTC->DR = 0x00002101;   /* giá trị mặc định BCD, không quan trọng với use-case này */

    RTC->ISR &= ~RTC_ISR_INIT;   /* thoát Init mode */
    RTC->WPR = 0xFF;              /* khoá lại write protection */
}
uint32_t RTC_GetEpoch_Reg(void)
{
    uint32_t tr = RTC->TR;   /* đọc TR trước, DR sau - đúng thứ tự bắt buộc để unlock shadow register */
    uint32_t dr = RTC->DR;
    (void)dr;

    /* TR lưu dạng BCD: bit[21:20]=chục giờ, [19:16]=đơn vị giờ, tương tự phút/giây */
    uint32_t sec  = (tr & 0xF) + ((tr >> 4) & 0x7) * 10;
    uint32_t min  = ((tr >> 8) & 0xF) + ((tr >> 12) & 0x7) * 10;
    uint32_t hour = ((tr >> 16) & 0xF) + ((tr >> 20) & 0x3) * 10;

    /* Đơn giản hoá: chỉ quy đổi trong phạm vi 1 ngày (đủ dùng để tính khoảng
     * cách thời gian giữa các mẫu đo liên tiếp trong 1 phiên chạy) */
    return hour * 3600U + min * 60U + sec;
}
void EXTI22_RTC_Init(void)
{
    EXTI->IMR  |= EXTI_IMR_MR22;
    EXTI->RTSR |= EXTI_RTSR_TR22;
    NVIC_EnableIRQ_Reg(RTC_WKUP_IRQn);
    NVIC_SetPriority_Reg(RTC_WKUP_IRQn, 5);
}

void RTC_WKUP_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR22) {
        EXTI->PR = EXTI_PR_PR22;
        RTC->ISR &= ~RTC_ISR_WUTF;
        /* Không cần làm gì thêm ở đây - mục đích duy nhất của ngắt này là
         * đánh thức CPU khỏi WFI; logic xử lý tiếp theo nằm ngay sau lệnh
         * "wfi" trong PowerMgmt_PreSleep(), được Idle Task gọi tiếp qua
         * PowerMgmt_PostSleep(). */
    }
}