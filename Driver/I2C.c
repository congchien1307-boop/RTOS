#include "STM32_reg.h"
#include "I2C.h"
void I2C1_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

    I2C1->CR2 = 42U;                       
    I2C1->CCR = 210U;                      
    I2C1->TRISE = 43U;                     
    I2C1->CR1 |= I2C_CR1_PE;               
}
extern void I2C1_GenerateStart(void)
{
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB)) { }   /* wait Start condition */
    /** 
    * @param addr7
    * @param ReadNotWrite 0= read, 1=write
    * @return 1 nếu slave ack, 0 nếu NACK  
    */
}   
extern void I2C_SendAddress(uint8_t addr7, uint8_t ReadNotWrite){
    I2C1->DR = (uint8_t)((addr7 << 1) | (ReadNotWrite ? 1U : 0U));

    /* Chờ 1 trong 2 khả năng: ADDR (slave ACK) hoặc AF (slave NACK) */
    while (!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF))) { }

    if (I2C1->SR1 & I2C_SR1_AF) {
        I2C1->SR1 &= ~I2C_SR1_AF;   /* xoá cờ lỗi */
        I2C1->CR1 |= I2C_CR1_STOP; /* huỷ giao dịch */
    }
}
extern void I2C1_WriteByte(uint8_t data)
{
    while (!(I2C1->SR1 & I2C_SR1_TXE)) { }
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF)) { }
}
extern void I2C1_GenerateStop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;
    while (I2C1->SR2 & I2C_SR2_BUSY) { }     /* chờ bus rảnh hẳn trước khi dùng tiếp */
} 
extern void I2C1_ReadBytes(uint8_t *buf, uint8_t len)
{
    I2C1->CR1 |= I2C_CR1_ACK;   /* bật ACK để tiếp tục nhận nhiều byte */

    for (uint8_t i = 0; i < len; i++) {
        if (i == (uint8_t)(len - 1)) {
            I2C1->CR1 &= ~I2C_CR1_ACK;   /* byte cuối: KHÔNG ack (báo slave dừng gửi) */
            I2C1->CR1 |= I2C_CR1_STOP;
        }
        while (!(I2C1->SR1 & I2C_SR1_RXNE)) { }
        buf[i] = (uint8_t)I2C1->DR;
    }
}   