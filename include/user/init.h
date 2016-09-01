#ifndef USER_INIT_H
#define USER_INIT_H

void SOFTAP_init_user();

void I2C_init_user();

void TCA6416A_init_user(
        uint8 address);

void BME280_init_user(
        uint8 address);

void BNO055_init_user(
        uint8 address);

void CC1101_init_user();

void HTTPD_init_user();

void TASK_init_user();

void TIMER_init_user();

#endif
