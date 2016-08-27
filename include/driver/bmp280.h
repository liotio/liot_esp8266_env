#ifndef BMP280_H
#define BMP280_H

#include "driver/soft_i2c.h"

#define BMP280_ADDR_LOW         0x76
#define BMP280_ADDR_HIGH        0x77

#define BMP280_REG_ID           0xD0
#define BMP280_REG_RESET        0xE0
#define BMP280_REG_STATUS       0xF3
#define BMP280_REG_CTRL_MEAS    0xF4
#define BMP280_REG_CONFIG       0xF5

#define BMP280_REG_PRESS_MSB    0xF7
#define BMP280_REG_PRESS_LSB    0xF8
#define BMP280_REG_PRESS_XLSB   0xF9

#define BMP280_REG_TEMP_MSB     0xFA
#define BMP280_REG_TEMP_LSB     0xFB
#define BMP280_REG_TEMP_XLSB    0xFC

#define BMP280_REG_DIG_T1_LSB   0x88
#define BMP280_REG_DIG_T2_LSB   0x8A
#define BMP280_REG_DIG_T3_LSB   0x8C
#define BMP280_REG_DIG_P1_LSB   0x8E
#define BMP280_REG_DIG_P2_LSB   0x90
#define BMP280_REG_DIG_P3_LSB   0x92
#define BMP280_REG_DIG_P4_LSB   0x94
#define BMP280_REG_DIG_P5_LSB   0x96
#define BMP280_REG_DIG_P6_LSB   0x98
#define BMP280_REG_DIG_P7_LSB   0x9A
#define BMP280_REG_DIG_P8_LSB   0x9C
#define BMP280_REG_DIG_P9_LSB   0x9E

// ...
uint16 BMP280_read_reg(
        uint8 address,
        uint8 reg);

// ...
uint32 BMP280_read_reg_20(
        uint8 address,
        uint8 reg);

// ...
uint32 BMP280_read_dig(
        uint8 address,
        uint8 reg);

// ...
uint8 BMP280_write_reg(
        uint8 address,
        uint8 reg,
        uint8 data);

// ...
int32 BMP280_read_temp(
        uint8 address);

#endif
