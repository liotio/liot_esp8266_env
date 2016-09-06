#include "driver/bno055.h"

static uint8 _address;

void BNO055_init(
        uint8 address)
{
    _address = address;

    os_printf("\nInit BNO055");

    // start config mode
    I2C_write_single(_address, BNO055_REG_PAGE_ID, 0);
    os_delay_us(500);
    I2C_write_single(_address, BNO055_REG_OPR_MODE, BNO055_MODE_OP_CFG);
    os_delay_us(500);
    I2C_write_single(_address, BNO055_REG_SYS_TRIGGER, 0x80);
    os_delay_us(500);
    I2C_write_single(_address, BNO055_REG_UNIT_SEL, 0);
    os_delay_us(500);
    I2C_write_single(_address, BNO055_REG_PWR_MODE, BNO055_MODE_PWR_NORMAL);
    os_delay_us(500);
    I2C_write_single(_address, BNO055_REG_OPR_MODE, BNO055_MODE_OP_IMU);
    os_delay_us(500);

    #ifdef DEBUG
    os_printf("SELFTEST RES: %x\n", (uint8) I2C_read_single(address, BNO055_REG_SELFTEST_RES));
    os_delay_us(500);
    os_printf("SYS_STATUS:   %x\n", (uint8) I2C_read_single(address, BNO055_REG_SYS_STAT));
    os_delay_us(500);
    os_printf("SYS_ERROR:    %x\n", (uint8) I2C_read_single(address, BNO055_REG_SYS_ERR));
    #endif
}

uint64 BNO055_read_euler()
{
    // get data in right order, but first pitch, then roll, last head
    return I2C_read_multiple_lsb(_address, BNO055_REG_EUL_HEAD_L, 48);
}
