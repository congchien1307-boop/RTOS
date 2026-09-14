#include "SPI.h"
#include "SX128.h"
void LoRa_Send_Reg(const uint8_t *data, uint16_t len)
{
    WriteReg(REG_OP_MODE, MODE_LONG_RANGE | MODE_STDBY);
    WriteReg(REG_FIFO_ADDR_PTR, 0x00);

    NSS_Low();
    SPI1_TransferByte(REG_FIFO | 0x80);   /* choose FIFO register, write mode */
    for (uint16_t i = 0; i < len; i++) {
        SPI1_TransferByte(data[i]);       
    }
    NSS_High();

    WriteReg(REG_PAYLOAD_LENGTH, (uint8_t)len);
    WriteReg(REG_IRQ_FLAGS, 0xFF);         /* clear IRQ flag */
    WriteReg(REG_OP_MODE, MODE_LONG_RANGE | MODE_TX);
    /* wait  DIO0 (TxDone) */
  
}
void LoRa_Init_Reg(void)
{
    /* Reset phần cứng SX1278 qua chân RESET (PA3) */
    GPIOA->BSRR = GPIO_BSRR_BR3;              /* kéo RESET xuống thấp */
    for (volatile uint32_t d = 0; d < 20000; d++) { }   /* ~2ms, thay bằng vTaskDelay khi có RTOS */
    GPIOA->BSRR = GPIO_BSRR_BS3;              /* thả RESET lên cao */
    for (volatile uint32_t d = 0; d < 60000; d++) { }   /* ~6ms chờ chip khởi động xong */

    WriteReg(REG_OP_MODE, MODE_LONG_RANGE | MODE_SLEEP);   /* phải ở Sleep mới đổi được LoRa/FSK mode */
    for (volatile uint32_t d = 0; d < 20000; d++) { }

    /* Tần số 433MHz: Frf = f * 2^19 / 32MHz (32MHz = tần số thạch anh gốc của SX1278, KHÔNG liên quan SYSCLK STM32) */
    uint32_t frf = (uint32_t)(433000000.0 * 524288.0 / 32000000.0);
    WriteReg(REG_FRF_MSB, (uint8_t)(frf >> 16));
    WriteReg(REG_FRF_MID, (uint8_t)(frf >> 8));
    WriteReg(REG_FRF_LSB, (uint8_t)(frf >> 0));

    WriteReg(REG_MODEM_CONFIG_1, 0x72);   /* BW=125kHz, CR=4/5, explicit header */
    WriteReg(REG_MODEM_CONFIG_2, 0x94);   /* SF9, CRC on */
    WriteReg(REG_PA_CONFIG, 0x8F);        /* PA_BOOST, ~15dBm */
    WriteReg(REG_FIFO_TX_BASE_ADDR, 0x00);
    WriteReg(REG_DIO_MAPPING_1, 0x40);    /* DIO0 = 00 -> báo TxDone khi đang ở chế độ TX */

    WriteReg(REG_OP_MODE, MODE_LONG_RANGE | MODE_STDBY);
}

void LoRa_Sleep_Reg(void)  { WriteReg(REG_OP_MODE, MODE_LONG_RANGE | MODE_SLEEP); }
void LoRa_Wakeup_Reg(void) {
    WriteReg(REG_OP_MODE, MODE_LONG_RANGE | MODE_STDBY);
    for (volatile uint32_t d = 0; d < 10000; d++) { }   /* chờ oscillator ổn định */
}