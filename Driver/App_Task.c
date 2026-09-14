#include "App_Task.h"
#include <string.h>
#include "SHT30.h"
#include "ADC.h"
/* Hàm từ các driver register-level đã viết ở các bước trước */
extern SensorStatus_t Sensor_SingleShotRead_Reg(float *temperature_c, float *humidity_pct);
extern void LoRa_Init_Reg(void);
extern void LoRa_Send_Reg(const uint8_t *data, uint16_t len);
extern void LoRa_Sleep_Reg(void);
extern void LoRa_Wakeup_Reg(void);

QueueHandle_t     xSensorDataQueue = NULL;
SemaphoreHandle_t xLoRaTxDoneSem   = NULL;

static TaskHandle_t hAcqTask, hCommsTask, hPowerTask;

BaseType_t AppTasks_Init(void)
{
    xSensorDataQueue = xQueueCreate(SENSOR_QUEUE_LEN, sizeof(SensorData_t));
    xLoRaTxDoneSem   = xSemaphoreCreateBinary();

    if (xSensorDataQueue == NULL || xLoRaTxDoneSem == NULL) {
        return pdFAIL;
    }

    BaseType_t ok = pdPASS;
    ok &= xTaskCreate(Acquisition_Task, "Acq", 256, NULL, TASK_PRIO_ACQUISITION, &hAcqTask);
    ok &= xTaskCreate(Comms_Task, "Comms", 384, NULL, TASK_PRIO_COMMS, &hCommsTask);
    ok &= xTaskCreate(PowerManagement_Task, "Pwr", 128, NULL, TASK_PRIO_POWERMGMT, &hPowerTask);
    return ok;
}

void Acquisition_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWake = xTaskGetTickCount();
    SensorData_t sample;

    for (;;) {
        memset(&sample, 0, sizeof(sample));

        if (Sensor_SingleShotRead_Reg(&sample.temperature_c, &sample.humidity_pct) == SENSOR_OK) {
            sample.battery_mv  = ADC1_ReadBatteryVoltage_mV();
            sample.timestamp_s = 0;   /* TODO: đọc RTC register-level, chưa viết */

            if (xQueueSend(xSensorDataQueue, &sample, 0) != pdPASS) {
                SensorData_t dump;
                xQueueReceive(xSensorDataQueue, &dump, 0);
                xQueueSend(xSensorDataQueue, &sample, 0);
            }
        }

        vTaskDelayUntil(&xLastWake, pdMS_TO_TICKS(ACQ_PERIOD_MS));
    }
}

void Comms_Task(void *pvParameters)
{
    (void)pvParameters;
    SensorData_t batch[COMMS_BATCH_SIZE];
    uint8_t txBuf[64];
    uint8_t count = 0;

    LoRa_Init_Reg();
    LoRa_Sleep_Reg();

    for (;;) {
        if (xQueueReceive(xSensorDataQueue, &batch[count], portMAX_DELAY) == pdPASS) {
            count++;
            if (count >= COMMS_BATCH_SIZE) {
                /* TODO: thay bằng DataPack_EncodeBatch() khi bạn cần CBOR,
                 * tạm thời copy thô để test luồng dữ liệu trước */
                uint16_t len = (uint16_t)(count * sizeof(SensorData_t));
                memcpy(txBuf, batch, len);

                LoRa_Wakeup_Reg();
                LoRa_Send_Reg(txBuf, len);
                xSemaphoreTake(xLoRaTxDoneSem, pdMS_TO_TICKS(2000));
                LoRa_Sleep_Reg();
                count = 0;
            }
        }
    }
}

void PowerManagement_Task(void *pvParameters)
{
    (void)pvParameters;
    for (;;) {
        /* TODO: đọc pin qua ADC register-level khi đã viết */
        vTaskDelay(pdMS_TO_TICKS(60000));
    }
}