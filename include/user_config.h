#ifndef USER_CONFIG_H
#define USER_CONFIG_H

// #define DEBUG 1

#define USE_OPTIMIZE_PRINTF

#include "mqtt_config.h"

#include "driver/bme280.h"
#include "driver/bno055.h"
#include "driver/tca6416a.h"

#define BME280_ADDR     BME280_ADDR_HIGH
#define BNO055_ADDR     BNO055_ADDR_HIGH
#define TCA6416A_ADDR   TCA6416A_ADDR_LOW

#endif
