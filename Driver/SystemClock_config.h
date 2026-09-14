#ifndef SYSTEMCLOCK_CONFIG_H
#define SYSTEMCLOCK_CONFIG_H
#define RCC_CR_HSION   (1 << 0)
#define RCC_CR_HSIRDY  (1 << 1)
#define RCC_CR_HSEON        (1 << 16)
#define RCC_CR_HSERDY       (1 << 17)
#define RCC_CR_PLLON        (1 << 24)
#define RCC_CR_PLLRDY       (1 << 25)
#define RCC_PLLCFGR_PLLM_Pos (16 << 0)
#define RCC_PLLCFGR_PLLN_Pos (336 << 6)
#define RCC_PLLCFGR_PLLP_Pos (1 << 16)
#define RCC_PLLCFGR_PLLSRC_Pos (1 << 22)
#define RCC_CFGR_PPRE2    (0 << 13)
#define RCC_CFGR_SW       (3 << 0)
#define RCC_CFGR_SW_PLL   (2 << 0)
#define RCC_CFGR_SWS  (3 << 2)
#define RCC_CFGR_SWS_PLL (2 << 2)
#define RCC_APB1ENR_PWREN     (1 << 28)
#define RCC_BDCR_LSEON        (1 << 0)
#define RCC_BDCR_LSERDY       (1 << 1)
#define RCC_BDCR_RTCSEL_Pos   8
#define RCC_BDCR_RTCSEL_LSE   (1U << RCC_BDCR_RTCSEL_Pos)   /* 01 = LSE làm nguồn RTCCLK */
#define RCC_BDCR_RTCEN        (1 << 15)

#define FLASH_ARC_DCEN  (1 << 10)
#define FLASH_ARC_ICEN  (1 << 9)
#define FLASH_ARC_PRFTEN  (1 << 8)
#define FLASH_ARC_LATECY_2WS  (2 << 0)

#define RTC_CR_WUCKSEL_Pos    0    /* bits [2:0] */
#define RTC_CR_WUCKSEL_DIV16  (0U << RTC_CR_WUCKSEL_Pos)   /* 000 = RTCCLK/16 */
#define RTC_CR_WUTIE          (1 << 14)
#define RTC_CR_WUTE           (1 << 10)
#define RTC_CR_FMT            (1 << 6)   /* 0 = 24h format */
#define RTC_ISR_WUTWF   (1 << 2)    /* Wakeup timer registers cho phép ghi (chỉ đúng khi WUTE=0) */
#define RTC_ISR_INITF   (1 << 6)
#define RTC_ISR_INIT    (1 << 7)
#define RTC_ISR_RSF     (1 << 5)
#define RTC_ISR_WUTF    (1 << 10)   /* cờ báo Wakeup Timer đã đếm xong */
#define RTC_PRER_PREDIV_A_Pos   16   /* async prescaler, bits [22:16] */
#define RTC_PRER_PREDIV_S_Pos   0    /* sync prescaler, bits [14:0] */

#define PWR_CR_LPDS      (1 << 0)   
#define PWR_CR_PDDS      (1 << 1)   
#define PWR_CR_DBP       (1 << 8)

#define SCB_SCR          (*(volatile uint32_t*)0xE000ED10)
#define SCB_SCR_SLEEPDEEP   (1 << 2)
extern void SystemClock_config(void);
#endif 