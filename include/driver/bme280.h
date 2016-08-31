#ifndef DRIVER_BME280_H
#define DRIVER_BME280_H

/**
 * For informations and the datasheet for BME280, visit
 * https://www.bosch-sensortec.com/bst/products/all_products/bme280
 */

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
#define BME280_REG_DIG_H1_MSB   0xA1
#define BME280_REG_DIG_H2_LSB   0xE1
#define BME280_REG_DIG_H3_MSB   0xE3
#define BME280_REG_DIG_H4_MSB   0xE4
#define BME280_REG_DIG_H5_LSB   0xE5
#define BME280_REG_DIG_H6_MSB   0xE7

/**
 * Initializes BME280 and stores address.
 */
void BME280_init(
        uint8 address);

/**
 * Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
 * t_fine carries fine temperature as global value
 */
sint32 BME280_get_temperature_int32();

/**
 * Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
 * Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
 */
uint32 BME280_get_pressure_int64();

/**
 * Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
 * Output value of “47445” represents 47445/1024 = 46.333 %RH
 */
uint32 BME280_get_humidity_int32();

/**
 * Returns temperature in DegC, double precision.
 * Output value of “51.23” equals 51.23 DegC.
 * t_fine carries fine temperature as global value
 */
double BME280_get_temperature();

/**
 * Returns pressure in Pa as double.
 * Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa
 */
double BME280_get_pressure();

/**
 * Returns humidity in %rH as as double.
 * Output value of “46.332” represents 46.332 %rH
 */
double BME280_get_humidity();

/**
 * Read up to 64 bits from register starting with MSB.
 */
uint64 BME280_read_reg_msb(
        uint8 reg,
        uint8 bits);

/**
 * Read up to 64 bits from register starting with LSB.
 */
uint64 BME280_read_reg_lsb(
        uint8 reg,
        uint8 bits);

/**
 * Write one byte to register.
 */
uint8 BME280_write_reg(
        uint8 reg,
        uint8 data);

#endif
