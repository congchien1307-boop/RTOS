#include "STM32_reg.h"
#include "SPI.h"
#include <stdint.h>
void SPI1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    SPI1->CR1 = SPI_CR1_MSTR              /* master mode */
              | SPI_CR1_SSM | SPI_CR1_SSI /* NSS quản lý bằng phần mềm, không để SPI tự kéo chân */
              | (4U << SPI_CR1_BR_POS);   /* BR=100 -> chia 32 -> ~2.6MHz, tăng dần sau khi debug ổn */
    SPI1->CR1 |= SPI_CR1_SPE;             /* enable */
}
extern uint16_t SPI1_TransferByte(uint8_t txData)
{
    while (!(SPI1->SR & SPI_SR_TXE)) { }   /* chờ buffer truyền rảnh */
    SPI1->DR = txData;                      /* ghi byte cần gửi */

    while (!(SPI1->SR & SPI_SR_RXNE)) { }  /* chờ byte nhận về đã sẵn sàng */
    uint8_t rxData = (uint8_t)SPI1->DR;     /* đọc byte nhận được (đồng thời clear cờ RXNE) */

    while (SPI1->SR & SPI_SR_BSY) { }      /* chờ peripheral thực sự rảnh trước khi thao tác tiếp */
    return rxData;
}
extern void WriteReg(uint8_t addr, uint8_t val)
{
    NSS_Low();
    SPI1_TransferByte((uint8_t)(addr | 0x80));   /* bit 7 = 1 -> write */
    SPI1_TransferByte(val);
    NSS_High();
    
}

extern uint16_t ReadReg(uint8_t addr)
{
    NSS_Low();
    SPI1_TransferByte((uint8_t)(addr & 0x7F));   /* bit 7 = 0 -> read */
    uint8_t val = SPI1_TransferByte(0x00);        
    NSS_High();
    return val;
}