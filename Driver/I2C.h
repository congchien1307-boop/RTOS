#include "STM32_reg.h"

typedef struct 
{
    volatile uint32_t  CR1;
    volatile uint32_t  CCR;
    volatile uint32_t  TRISE;
    volatile uint32_t  CR2;
    volatile uint32_t  SR1;
    volatile uint32_t  DR;
    volatile uint32_t  SR2;
}I2C_TypeDef;

#define I2C_CR1_PE (1 << 0)
#define I2C_CR1_START (1 << 8)
#define I2C_SR1_SB (1 << 0)
#define I2C_SR1_AF (1 << 10)
#define I2C_SR1_ADDR (1 << 1)
#define I2C_CR1_STOP (1 << 9)
#define I2C_SR1_TXE  (1 << 7)
#define I2C_SR1_BTF  (1 << 2)
#define I2C_SR2_BUSY (1 << 1)
#define I2C_CR1_ACK  (1 << 10)
#define I2C_SR1_RXNE (1 << 6)


void I2C1_Init(void);
extern void I2C1_GenerateStart(void);
extern uint8_t I2C1_SendAddress(uint8_t addr7, uint8_t ReadNotWrite);
extern void I2C1_WriteByte(uint8_t data);
extern void I2C1_GenerateStop(void);
extern void I2C1_ReadBytes(uint8_t *buf, uint8_t len);
