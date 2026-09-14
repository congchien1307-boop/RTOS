#ifndef STM32_REG_H
#define STM32_REG_H

#include "SystemClock_config.h"
#define PERIPHERAL_BASE 0x40000000UL
#define AHB1_BASE (PERIPHERAL_BASE + 0x20000UL)
#define APB1_BASE (PERIPHERAL_BASE)
#define APB2_BASE (PERIPHERAL_BASE + 0x10000UL)
#define GPIOA_BASE (AHB1_BASE + 0x0000UL)
#define GPIOB_BASE (AHB1_BASE + 0x0400UL)
#define RCC_BASE (AHB1_BASE + 0x3800UL)
#define I2C1_BASE (APB1_BASE + 0x5400UL)
#define SPI1_BASE (APB2_BASE + 0x3000UL)
#define EXTI_BASE (APB2_BASE + 0x3C00UL)
#define SYSCFG_BASE (APB2_BASE + 0x3800UL)
#define PWR_BASE (APB1_BASE + 0x7000UL)
#define RTC_BASE (APB1_BASE + 0x2800UL)
#define SYSTICK_BASE   0xE000E010UL
#define ADC1_BASE (APB2_BASE + 0x2000UL)
#define Flash_Start_Address 0x40023C00UL
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef long unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef signed char       int8_t;
typedef signed short      int16_t;
typedef long signed int         int32_t;
typedef signed long long  int64_t;
#define __IO volatile
typedef struct 
{
    volatile uint32_t MODER;   // GPIO port mode register
    volatile uint32_t OTYPER;  // GPIO port output type register
    volatile uint32_t OSPEEDR; // GPIO port output speed register
    volatile uint32_t PUPDR;   // GPIO port pull-up/pull-down register
    volatile uint32_t IDR;     // GPIO port input data register
    volatile uint32_t ODR;     // GPIO port output data register
    volatile uint32_t BSRR;    // GPIO port bit set/reset register
    volatile uint32_t LCKR;    // GPIO port configuration lock register
    volatile uint32_t AFR[2];  // GPIO alternate function registers
} GPIOA_TypeDef;
typedef struct {
    __IO uint32_t CR, PLLCFGR, CFGR, CIR;
    __IO uint32_t AHB1RSTR, AHB2RSTR, AHB3RSTR, RESERVED0;
    __IO uint32_t APB1RSTR, APB2RSTR, RESERVED1[2];
    __IO uint32_t AHB1ENR, AHB2ENR, AHB3ENR, RESERVED2;
    __IO uint32_t APB1ENR, APB2ENR, RESERVED3[2];
    __IO uint32_t AHB1LPENR, AHB2LPENR, AHB3LPENR, RESERVED4;
    __IO uint32_t APB1LPENR, APB2LPENR, RESERVED5[2];
    __IO uint32_t BDCR, CSR;
} RCC_TypeDef;
typedef struct
{
    volatile uint32_t MODER;   // GPIO port mode register
    volatile uint32_t OTYPER;  // GPIO port output type register
    volatile uint32_t OSPEEDR; // GPIO port output speed register
    volatile uint32_t PUPDR;   // GPIO port pull-up/pull-down register
    volatile uint32_t IDR;     // GPIO port input data register
    volatile uint32_t ODR;     // GPIO port output data register
    volatile uint32_t BSRR;    // GPIO port bit set/reset register
    volatile uint32_t LCKR;    // GPIO port configuration lock register
    volatile uint32_t AFR[2];  // GPIO alternate function registers
} GPIOB_TypeDef;
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_TypeDef;
typedef struct {
    volatile uint32_t ACR;       // Flash control register
    volatile uint32_t KEYR;     // Flash key register
    volatile uint32_t OPTKEYR;  // Flash option key register
    volatile uint32_t SR;
    volatile uint32_t CR;       // Flash status register
    volatile uint32_t OPTCR;    // Flash option control register
} Flash_TypeDef;
typedef struct 
{
    volatile uint32_t CR;
    volatile uint32_t WPR;
}PWR_TypeDef;

#define FLASH ((Flash_TypeDef *) Flash_Start_Address)
#define GPIOA ((GPIOA_TypeDef *) GPIOA_BASE)
#define GPIOB ((GPIOB_TypeDef *) GPIOB_BASE)
#define RCC ((RCC_TypeDef *) RCC_BASE)
#define I2C1 ((I2C_TypeDef *) I2C1_BASE)
#define SPI1 ((SPI_TypeDef *) SPI1_BASE)
#define EXTI ((EXTI_TypeDef *) EXTI_BASE)
#define SYSCFG ((SYSCFG_TypeDef *) SYSCFG_BASE)
#define PWR ((PWR_TypeDef *) PWR_BASE)
#define RTC ((RTC_TypeDef *) RTC_BASE)
#define ADC1 ((ADC1_TypeDef *) ADC1_BASE)
#define RCC_AHB1_AHB1ENR_GPIOAEN   (1 << 0)
#define RCC_AHB1_AHB1ENR_GPIOBEN   (1 << 1)
#define RCC_APB2ENR_UART1EN   (1 << 4)
#define RCC_APB1ENR_I2C1EN    (1 << 21)
#define RCC_APB1RSTR_I2C1RST  (1 << 21)
#define RCC_APB2ENR_SPI1EN    (1 << 12)
#define RCC_APB2ENR_SYSCFGEN   (1 << 14)
#define RCC_APB2ENR_ADC1EN   (1 << 8)
#define NVIC_ISER0       (*(__IO uint32_t*)0xE000E100UL)
#define NVIC_IPR_BASE    0xE000E400UL
#define SCB_SCR          (*(__IO uint32_t*)0xE000ED10UL)
#define SCB_SCR_SLEEPDEEP   (1U << 2)
#define SCB_AIRCR        (*(__IO uint32_t*)0xE000ED0CUL)
#endif