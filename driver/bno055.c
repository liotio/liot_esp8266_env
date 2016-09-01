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
    os_printf("SELFTEST RES: %x\n", (uint8) BNO055_read_reg_16(address, BNO055_REG_SELFTEST_RES));
    os_delay_us(500);
    os_printf("SYS_STATUS:   %x\n", (uint8) BNO055_read_reg_16(address, BNO055_REG_SYS_STAT));
    os_delay_us(500);
    os_printf("SYS_ERROR:    %x\n", (uint8) BNO055_read_reg_16(address, BNO055_REG_SYS_ERR));
    #endif
}

uint16 BNO055_read_reg(
        uint8 address,
        uint8 reg)
{
    uint8 msb;

    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BNO055\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write to BNO055\n");
        return -1;
    }

    // restart and read register data
    if (I2C_start(address, I2C_SLAVE_READ)) {
        os_printf("\nCannot init reading from BNO055\n");
        return -1;
    }

    msb = I2C_read_nack();
    I2C_stop();

    return msb;
}

uint32 BNO055_read_reg_16(
        uint8 address,
        uint8 reg)
{
    uint8 msb, lsb;

    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BNO055\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write to BNO055\n");
        return -1;
    }

    // restart and read register data
    if (I2C_start(address, I2C_SLAVE_READ)) {
        os_printf("\nCannot init reading from BNO055\n");
        return -1;
    }

    lsb = I2C_read_ack();
    msb = I2C_read_nack();
    I2C_stop();

    return msb << 8 | lsb;
}

uint64 BNO055_read_reg_48(
        uint8 address,
        uint8 reg)
{
    uint8 lsb, msb;
    uint64 result = 0;

    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BNO055\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write to BNO055\n");
        return -1;
    }

    os_delay_us(500);

    // restart and read register data
    if (I2C_start(address, I2C_SLAVE_READ)) {
        os_printf("\nCannot init reading from BNO055\n");
        return -1;
    }

    lsb = I2C_read_ack();
    msb = I2C_read_ack();

    result = (msb << 8) | lsb;

    lsb = I2C_read_ack();
    msb = I2C_read_ack();

    result = (result << 16) | (msb << 8) | lsb;

    lsb = I2C_read_ack();
    msb = I2C_read_nack();

    result = (result << 16) | (msb << 8) | lsb;

    I2C_stop();

    return result;
}
