#include "STM32_reg.h"
#include "GPIO.h"
typedef struct 
{
    volatile uint32_t CR1;
    volatile uint32_t SR;
    volatile uint32_t DR;
}SPI_TypeDef;
static void NSS_Low(void){ GPIOA->BSRR = GPIO_BSRR_BR4;}
static void NSS_High(void){ GPIOA->BSRR = GPIO_BSRR_BS4;}
#define SPI_CR1_MSTR  (1 << 2)
#define SPI_CR1_SSM   (1 << 9)
#define SPI_CR1_SSI   (1 << 8)
#define SPI_CR1_BR_POS  3
#define SPI_CR1_SPE   (1 << 6)
#define SPI_SR_TXE    (1 << 1)
#define SPI_SR_RXNE   (1 << 0)
#define SPI_SR_BSY    (1 << 7)
void SPI1_Init(void);
uint16_t SPI1_TransferByte(uint8_t txData);
void WriteReg(uint8_t addr, uint8_t val);
uint16_t ReadReg(uint8_t addr);