#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include "user_interface.h"

// #define DEBUG 1

#define USE_OPTIMIZE_PRINTF

#define DEVICE_ID (system_get_chip_id() % 10)

#include "mqtt_config.h"

#include "driver/bme280.h"
#include "driver/bno055.h"
#include "driver/tca6416a.h"

#define BME280_ADDR     BME280_ADDR_HIGH
#define BNO055_ADDR     BNO055_ADDR_HIGH
#define TCA6416A_ADDR   TCA6416A_ADDR_LOW

#endif
