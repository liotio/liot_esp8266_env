#ifndef DRIVER_BME280_H
#define DRIVER_BME280_H


#include "driver/soft_i2c.h"


#define BME280_ADDR_LOW         0x76
#define BME280_ADDR_HIGH        0x77

#define BME280_REG_ID           0xD0
#define BME280_REG_RESET        0xE0
#define BME280_REG_STATUS       0xF3
#define BME280_REG_CTRL_MEAS    0xF4
#define BME280_REG_CONFIG       0xF5

#define BME280_REG_HUM_MSB      0xFD
#define BME280_REG_HUM_LSB      0xFE

#define BME280_REG_PRESS_MSB    0xF7
#define BME280_REG_PRESS_LSB    0xF8
#define BME280_REG_PRESS_XLSB   0xF9

#define BME280_REG_TEMP_MSB     0xFA
#define BME280_REG_TEMP_LSB     0xFB
#define BME280_REG_TEMP_XLSB    0xFC

#define BME280_REG_DIG_T1_LSB   0x88
#define BME280_REG_DIG_T2_LSB   0x8A
#define BME280_REG_DIG_T3_LSB   0x8C
#define BME280_REG_DIG_P1_LSB   0x8E
#define BME280_REG_DIG_P2_LSB   0x90
#define BME280_REG_DIG_P3_LSB   0x92
#define BME280_REG_DIG_P4_LSB   0x94
#define BME280_REG_DIG_P5_LSB   0x96
#define BME280_REG_DIG_P6_LSB   0x98
#define BME280_REG_DIG_P7_LSB   0x9A
#define BME280_REG_DIG_P8_LSB   0x9C
#define BME280_REG_DIG_P9_LSB   0x9E
#define BME280_REG_DIG_H1_LSB   0xA1
#define BME280_REG_DIG_H2_LSB   0xE1
#define BME280_REG_DIG_H3_LSB   0xE3


uint64 BME280_read_reg(
        uint8 reg,
        uint8 bits);

uint8 BME280_write_reg(
        uint8 reg,
        uint8 data);

int32 BME280_read_temp();


#endif
