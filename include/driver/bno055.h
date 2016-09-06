#ifndef BNO055_H
#define BNO055_H

#include "driver/soft_i2c.h"

#define BNO055_ADDR_LOW         0x28
#define BNO055_ADDR_HIGH        0x29

#define BNO055_REG_CHIP_ID      0x00
#define BNO055_REG_GYR_ID       0x01
#define BNO055_REG_MAG_ID       0x02
#define BNO055_REG_ACC_ID       0x03

#define BNO055_REG_PAGE_ID      0x07

#define BNO055_REG_SELFTEST_RES 0x36
#define BNO055_REG_SYS_CLK_STAT 0x38
#define BNO055_REG_SYS_STAT     0x39
#define BNO055_REG_SYS_ERR      0x3A
#define BNO055_REG_UNIT_SEL     0x3B

#define BNO055_REG_TEMP         0x34

#define BNO055_REG_GYR_DAT_X_L  0x14
#define BNO055_REG_GYR_DAT_X_M  0x15
#define BNO055_REG_GYR_DAT_Y_L  0x16
#define BNO055_REG_GYR_DAT_Y_M  0x17
#define BNO055_REG_GYR_DAT_Z_L  0x18
#define BNO055_REG_GYR_DAT_Z_M  0x19

#define BNO055_REG_EUL_HEAD_L   0x1A
#define BNO055_REG_EUL_HEAD_M   0x1B
#define BNO055_REG_EUL_ROLL_L   0x1C
#define BNO055_REG_EUL_ROLL_M   0x1D
#define BNO055_REG_EUL_PITCH_L  0x1E
#define BNO055_REG_EUL_PITCH_M  0x1F

#define BNO055_REG_UNIT_SEL     0x3B
#define BNO055_REG_OPR_MODE     0x3D
#define BNO055_REG_PWR_MODE     0x3E
#define BNO055_REG_SYS_TRIGGER  0x3F

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

#define BNO055_UNIT_ACC_MPS2    0x00
#define BNO055_UNIT_ACC_MG      0x01
#define BNO055_UNIT_AN_RT_DPS   0x00
#define BNO055_UNIT_AN_RT_RPS   0x02
#define BNO055_UNIT_EUL_AN_DEG  0x00
#define BNO055_UNIT_EUL_AN_RAD  0x04
#define BNO055_UNIT_TEMP_C      0x00
#define BNO055_UNIT_TEMP_F      0x10
#define BNO055_UNIT_DAT_FMT_WIN 0x00
#define BNO055_UNIT_DAT_FMT_AND 0x80

void BNO055_init(
        uint8 address);

uint64 BNO055_read_euler();

#endif
