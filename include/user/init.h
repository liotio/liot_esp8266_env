#ifndef USER_INIT_H
#define USER_INIT_H

#include "osapi.h"
#include "user_interface.h"

#include "user_config.h"
#include "wifi_config.h"

#include "driver/soft_i2c.h"
#include "driver/tca6416a.h"
#include "driver/cc1101.h"

#include "lib/wifi.h"

#include "user/mqtt.h"
#include "user/sound.h"
#include "user/task.h"

void ICACHE_FLASH_ATTR MQTT_init_user();

void ICACHE_FLASH_ATTR WIFI_init_user();

void ICACHE_FLASH_ATTR WIFI_STATION_init_user();

void ICACHE_FLASH_ATTR WIFI_SOFTAP_init_user();

void ICACHE_FLASH_ATTR I2C_init_user();

void ICACHE_FLASH_ATTR TCA6416A_init_user();

void ICACHE_FLASH_ATTR BME280_init_user();

void ICACHE_FLASH_ATTR BNO055_init_user();

void ICACHE_FLASH_ATTR CC1101_init_user();

void ICACHE_FLASH_ATTR HTTPD_init_user();

void ICACHE_FLASH_ATTR TASK_init_user();

void ICACHE_FLASH_ATTR TIMER_init_user();

#endif
