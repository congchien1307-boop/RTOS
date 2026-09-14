#ifndef APP_TASKS_H
#define APP_TASKS_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define ACQ_PERIOD_MS           10000U
#define COMMS_BATCH_SIZE        3U
#define SENSOR_QUEUE_LEN        8U

#define TASK_PRIO_ACQUISITION   (tskIDLE_PRIORITY + 2)
#define TASK_PRIO_COMMS         (tskIDLE_PRIORITY + 2)
#define TASK_PRIO_POWERMGMT     (tskIDLE_PRIORITY + 1)

typedef struct {
    float    temperature_c;
    float    humidity_pct;
    uint16_t battery_mv;
    uint32_t timestamp_s;
} SensorData_t;

extern QueueHandle_t     xSensorDataQueue;
extern SemaphoreHandle_t xLoRaTxDoneSem;

BaseType_t AppTasks_Init(void);
void Acquisition_Task(void *pvParameters);
void Comms_Task(void *pvParameters);
void PowerManagement_Task(void *pvParameters);

#endif