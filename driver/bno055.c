#include "driver/bmp280.h"

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

uint32 BNO055_read_reg_20(
        uint8 address,
        uint8 reg)
{
    uint8 msb, lsb, xsb;

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

    msb = I2C_read_ack();
    lsb = I2C_read_ack();
    xsb = I2C_read_nack();
    I2C_stop();

    return msb << 12 | lsb << 4 | xsb >> 4;
}

uint32 BNO055_read_dig(
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
        os_printf("\nCannot find BNO055\n");
        return -1;
    }

    lsb = I2C_read_ack();
    msb = I2C_read_nack();
    I2C_stop();

    return msb << 8 | lsb;
}

uint8 BNO055_write_reg(
        uint8 address,
        uint8 reg,
        uint8 data)
{
    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BNO055\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write to BNO055\n");
        return -1;
    }
    if (I2C_write(data)) {
        os_printf("\nCannot write to BNO055\n");
        return -1;
    }

    os_printf("Writing to BNO055 successful");

    return 0;
}
