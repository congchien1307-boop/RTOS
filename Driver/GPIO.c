#include "STM32_reg.h"
#include "GPIO.h"
 void GPIO_Init(void){
    // GPIO for I2C
    RCC->AHB1ENR |= (RCC_AHB1_AHB1ENR_GPIOAEN | RCC_AHB1_AHB1ENR_GPIOBEN);
    GPIOB->MODER &= ~((3 << GPIO_MODER_6_POS) | (3 << GPIO_MODER_7_POS));
    GPIOB->MODER |= ((2 << GPIO_MODER_6_POS) | (GPIO_MODER_7_POS));
    GPIOB->OTYPER |= (GPIO_OTYPER_6 | GPIO_OTYPER_7);
    GPIOB->OSPEEDR |= ((2 << 12) | (2<< 14));
    GPIOB->AFR[0] &= ~((0xFU << (4*6)) | (0xFU << (4*7)));
    GPIOB->AFR[0] |= ((4 << 4*6) | (4 << 4*7));
    // GPIO for SPI
    GPIOA->MODER   &= ~((3 << GPIO_MODER_5_POS) | (3 << GPIO_MODER_6_POS)| (3 << GPIO_MODER_7_POS));
    GPIOA->MODER   |=  (2U << GPIO_MODER_5_POS) | (2U << GPIO_MODER_6_POS) | (2U << GPIO_MODER_7_POS);
    GPIOA->OSPEEDR |=  (3U << GPIO_OSPEEDR_5_POS) | (3U << GPIO_OSPEEDR_6_POS) | (3U << GPIO_OSPEEDR_7_POS);
    GPIOA->AFR[0]  &= ~((0xFU << (4*5)) | (0xFU << (4*6)) | (0xFU << (4*7)));
    GPIOA->AFR[0]  |=  (5 << (4*5)) | (5 << (4*6)) | (5 << (4*7));   /* AF5 = SPI1 trên PA5/6/7 */
   // GPIO for SX1278
   GPIOA->MODER &= ~(3 << GPIO_MODER_4_POS | 3 << GPIO_MODER_3_POS);
   GPIOA->MODER |=  (1U << GPIO_MODER_4_POS) | (1U << GPIO_MODER_3_POS); /* output mode */
   GPIOA->OTYPER &= ~(GPIO_OTYPER_4 | GPIO_OTYPER_3);   /* push-pull */
   GPIOA->BSRR = GPIO_BSRR_BS4;   /* NSS mặc định mức cao (chưa chọn chip) */
   GPIOA->BSRR = GPIO_BSRR_BR4;
 }
