#ifndef BNO055_H
#define BNO055_H

#include "driver/soft_i2c.h"

#define BNO055_ADDR_LOW         0x28
#define BNO055_ADDR_HIGH        0x29

#define BNO055_REG_CHIP_ID      0x00
#define BNO055_REG_GYR_ID       0x01
#define BNO055_REG_MAG_ID       0x02
#define BNO055_REG_ACC_ID       0x03
#define BNO055_REG_UNIQUE_ID    0x03

// power modes
#define BNO055_MODE_PWR_NORMAL  0x00  // all sensors always on
#define BNO055_MODE_PWR_LOW     0x01  // only acc after 5 sec without motion
#define BNO055_MODE_PWR_SUSPEND 0x02  // all sensors and mcu in sleep mode

// operation non-fusion modes
#define BNO055_MODE_OP_CFG      0x00
#define BNO055_MODE_OP_ACCONLY  0x01
#define BNO055_MODE_OP_MAGONLY  0x02
#define BNO055_MODE_OP_GYROONLY 0x03
#define BNO055_MODE_OP_ACCMAG   0x04
#define BNO055_MODE_OP_ACCGYRO  0x05
#define BNO055_MODE_OP_MAGGYRO  0x06
#define BNO055_MODE_OP_AMG      0x07

// opartion fusion modes
#define BNO055_MODE_OP_IMU      0x08
#define BNO055_MODE_OP_COMPASS  0x09
#define BNO055_MODE_OP_M4G      0x0A
#define BNO055_MODE_OP_NDOF_OFF 0x0B
#define BNO055_MODE_OP_NDOF     0x0C



/*
#define BNO055_REG_ID           0xD0
#define BNO055_REG_RESET        0xE0
#define BNO055_REG_STATUS       0xF3
#define BNO055_REG_CTRL_MEAS    0xF4
#define BNO055_REG_CONFIG       0xF5

#define BNO055_REG_PRESS_MSB    0xF7
#define BNO055_REG_PRESS_LSB    0xF8
#define BNO055_REG_PRESS_XLSB   0xF9

#define BNO055_REG_TEMP_MSB     0xFA
#define BNO055_REG_TEMP_LSB     0xFB
#define BNO055_REG_TEMP_XLSB    0xFC

#define BNO055_REG_DIG_T1_LSB   0x88
#define BNO055_REG_DIG_T2_LSB   0x8A
#define BNO055_REG_DIG_T3_LSB   0x8C
#define BNO055_REG_DIG_P1_LSB   0x8E
#define BNO055_REG_DIG_P2_LSB   0x90
#define BNO055_REG_DIG_P3_LSB   0x92
#define BNO055_REG_DIG_P4_LSB   0x94
#define BNO055_REG_DIG_P5_LSB   0x96
#define BNO055_REG_DIG_P6_LSB   0x98
#define BNO055_REG_DIG_P7_LSB   0x9A
#define BNO055_REG_DIG_P8_LSB   0x9C
#define BNO055_REG_DIG_P9_LSB   0x9E
*/

// ...
uint16 BNO055_read_reg(
        uint8 address,
        uint8 reg);

// ...
uint32 BNO055_read_reg_20(
        uint8 address,
        uint8 reg);

// ...
uint32 BNO055_read_dig(
        uint8 address,
        uint8 reg);

// ...
uint8 BNO055_write_reg(
        uint8 address,
        uint8 reg,
        uint8 data);

#endif
