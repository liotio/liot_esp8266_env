#include "driver/tca6416a.h"

uint16 ICACHE_FLASH_ATTR TCA6416A_read_reg(
        uint8 address,
        uint8 reg)
{
    uint8 data;

    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("Cannot init writing to TCA6416A\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("Cannot write to TCA6416A\n");
        return -1;
    }

    // restart and read register data
    if (I2C_start(address, I2C_SLAVE_READ)) {
        os_printf("Cannot find TCA6416A\n");
        return -1;
    }

    data = I2C_read_nack();
    I2C_stop();

    return data;
}

uint8 ICACHE_FLASH_ATTR TCA6416A_write_reg(
        uint8 address,
        uint8 reg,
        uint8 data)
{
    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("Cannot init writing to TCA6416A\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("Cannot write register to TCA6416A\n");
        return -1;
    }
    if (I2C_write(data)) {
        os_printf("Cannot write data to TCA6416A\n");
        return -1;
    }

    I2C_stop();

    os_printf("Writing to TCA6416A successful\n");

    return 0;
}

uint8 ICACHE_FLASH_ATTR TCA6416A_input_get(
        uint8 address,
        uint8 gpio)
{
    uint8 reg;
    uint16 data;

    // determine register to read from
    if (gpio > 0x00 && gpio <= 0x07) {
        reg = TCA6416A_REG_INPUT_0;
    } else if (gpio > 0x07 && gpio <= 0x0F) {
        reg = TCA6416A_REG_INPUT_1;
        gpio -= 0x08;
    } else {
        return -1;
    }

    data = TCA6416A_read_reg(address, gpio);
    if (data == -1) {
        return -1;
    }

    return (data >> gpio) & 1;
}
