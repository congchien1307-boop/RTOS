#include "FreeRTOS.h"
#include "semphr.h"
extern SemaphoreHandle_t xLoRaTxDoneSem;
#define REG_FIFO               0x00
#define REG_OP_MODE            0x01
#define REG_FRF_MSB             0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_FIFO_ADDR_PTR        0x0D
#define REG_FIFO_TX_BASE_ADDR     0x0E
#define REG_IRQ_FLAGS            0x12
#define REG_MODEM_CONFIG_1        0x1D
#define REG_MODEM_CONFIG_2         0x1E
#define REG_PAYLOAD_LENGTH        0x22
#define REG_DIO_MAPPING_1          0x40

#define MODE_LONG_RANGE   0x80   /* bit 7 = 1 -> bật chế độ LoRa (thay vì FSK/OOK mặc định) */
#define MODE_SLEEP        0x00
#define MODE_STDBY        0x01
#define MODE_TX            0x03
#define IRQ_TX_DONE_MASK   0x08
