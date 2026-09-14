#include "STM32_reg.h"
typedef uint8_t SensorStatus_t;
#define SENSOR_OK 0
#define SENSOR_ERR_I2C 1
#define SENSOR_ERR_CRC 2
#define SENSOR_ERR_TIMEOUT 3