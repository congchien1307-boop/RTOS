#include "I2C.h"
#include "SHT30.h"
#define SHT30_ADDR7   0x44   /* ADDR pin nối GND */
extern uint8_t CRC8_SHT(const uint8_t *data, uint8_t len){
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++) {
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}
SensorStatus_t Sensor_SingleShotRead_Reg(float *temperature_c, float *humidity_pct)
{
    uint8_t raw[6];

    /* --- Bước 1: gửi lệnh đo single-shot high-repeatability (0x2400) --- */
    I2C1_GenerateStart();
    if (!I2C1_SendAddress(SHT30_ADDR7, 0)) {
        return SENSOR_ERR_I2C;
    }
    I2C1_WriteByte(0x24);
    I2C1_WriteByte(0x00);
    I2C1_GenerateStop();

    /* --- Bước 2: chờ sensor đo xong (~15ms), dùng vTaskDelay nếu FreeRTOS
     * đã chạy, tạm thời busy-wait nếu bạn đang test đứng riêng (chưa RTOS) --- */
    for (volatile uint32_t d = 0; d < 200000; d++) { }   /* placeholder, thay bằng vTaskDelay(15) sau */

    /* --- Bước 3: repeated START, đọc 6 byte kết quả --- */
    I2C1_GenerateStart();
    if (!I2C1_SendAddress(SHT30_ADDR7, 1)) {
        return SENSOR_ERR_I2C;
    }
    I2C1_ReadBytes(raw, 6);
    
    if (CRC8_SHT(&raw[0], 2) != raw[2] || CRC8_SHT(&raw[3], 2) != raw[5]) {
    return SENSOR_ERR_CRC;
    }
    /* raw[0..1]=temp, raw[2]=CRC temp, raw[3..4]=hum, raw[5]=CRC hum
     * (CRC check giữ nguyên hàm crc8_sht() đã viết trước đó) */
    uint16_t rawTemp = (uint16_t)((raw[0] << 8) | raw[1]);
    uint16_t rawHum  = (uint16_t)((raw[3] << 8) | raw[4]);
    *temperature_c = -45.0f + 175.0f * ((float)rawTemp / 65535.0f);
    *humidity_pct  = 100.0f * ((float)rawHum / 65535.0f);

    return SENSOR_OK;
}
