#ifndef USER_INIT_H
#define USER_INIT_H

#include "osapi.h"
#include "user_interface.h"

#include "user_config.h"
#include "wifi_config.h"

#include "driver/soft_i2c.h"
#include "driver/tca6416a.h"
#include "driver/cc1101.h"

#include "user/sound.h"
#include "user/task.h"

void SOFTAP_init_user();

void I2C_init_user();

void TCA6416A_init_user();

void BME280_init_user();

void BNO055_init_user();

void CC1101_init_user();

void HTTPD_init_user();

void TASK_init_user();

void TIMER_init_user();

#endif
