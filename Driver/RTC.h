#include "STM32_reg.h"

typedef struct 
{
    volatile uint32_t WPR;
    volatile uint32_t ISR;
    volatile uint32_t PRER;
    volatile uint32_t TR;
    volatile uint32_t DR;
    volatile uint32_t CR;
    volatile uint32_t WUTR;
}RTC_TypeDef;
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
#define RTC_PRER_PREDIV_S_Pos   0
#define RTC_WKUP_IRQn    3