#include "driver/tca6416a.h"

uint16 TCA6416A_init()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
    GPIO_OUTPUT_SET(0, 1);
    os_delay_us(10);
    GPIO_OUTPUT_SET(0, 0);
    os_delay_us(10);
    GPIO_OUTPUT_SET(0, 1);

    TCA6416A_state_output[TCA6416A_P0] = 0xFF; // default value 1111 1111
    TCA6416A_state_output[TCA6416A_P1] = 0xFF; // default value 1111 1111
    TCA6416A_state_polinv[TCA6416A_P0] = 0x00; // default value 0000 0000
    TCA6416A_state_polinv[TCA6416A_P1] = 0x00; // default value 0000 0000
    TCA6416A_state_config[TCA6416A_P0] = 0xFF; // default value 1111 1111
    TCA6416A_state_config[TCA6416A_P1] = 0xFF; // default value 1111 1111
}

uint8 TCA6416A_update_outputs(
        uint8 address)
{
    os_delay_us(20);

    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("Cannot init writing to TCA6416A\n");
        return -1;
    }
    if (I2C_write(TCA6416A_REG_CONFIG_0)) {
        os_printf("Cannot write register to TCA6416A\n");
        return -1;
    }
    if (I2C_write(TCA6416A_state_config[TCA6416A_P0])) {
        os_printf("Cannot write config 0 data to TCA6416A\n");
        return -1;
    }
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("Cannot init writing to TCA6416A\n");
        return -1;
    }
    if (I2C_write(TCA6416A_REG_CONFIG_1)) {
        os_printf("Cannot write register to TCA6416A\n");
        return -1;
    }
    if (I2C_write(TCA6416A_state_config[TCA6416A_P1])) {
        os_printf("Cannot write config 1 data to TCA6416A\n");
        return -1;
    }
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("Cannot init writing to TCA6416A\n");
        return -1;
    }
    if (I2C_write(TCA6416A_REG_OUTPUT_0)) {
        os_printf("Cannot write register to TCA6416A\n");
        return -1;
    }
    if (I2C_write(TCA6416A_state_output[TCA6416A_P0])) {
        os_printf("Cannot write output 0 data to TCA6416A\n");
        return -1;
    }
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("Cannot init writing to TCA6416A\n");
        return -1;
    }
    if (I2C_write(TCA6416A_REG_OUTPUT_1)) {
        os_printf("Cannot write register to TCA6416A\n");
        return -1;
    }
    if (I2C_write(TCA6416A_state_output[TCA6416A_P1])) {
        os_printf("Cannot write output 1 data to TCA6416A\n");
        return -1;
    }

    I2C_stop();
    os_printf("Updating TCA6416A successful\n");

    return 0;
}

uint16 TCA6416A_read_reg(
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

uint8 TCA6416A_write_reg(
        uint8 address,
        uint8 reg,
        uint8 data)
{
    os_delay_us(20);

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

uint8 TCA6416A_input_get(
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

void TCA6416A_set_outputs_low(
        uint8 address,
        uint16 pins)
{
    // in conf register, 1 is input, 0 is output
    // set as outputs as follows:
    // - invert pins variable, then AND with TCA6416A_state_config
    // existing outputs will stay outputs, new will become outputs with AND
    TCA6416A_state_config[TCA6416A_P0] &= ~(pins);
    TCA6416A_state_config[TCA6416A_P1] &= ~(pins >> 8);

    // in output register, 1 is high, 0 is low
    // set low as follows:
    // - inverts pins variable, then AND with TCA6416A_state_output
    // low will stay low, high will change if it should
    TCA6416A_state_output[TCA6416A_P0] &= ~(pins);
    TCA6416A_state_output[TCA6416A_P1] &= ~(pins >> 8);

    TCA6416A_update_outputs(address);
}

void TCA6416A_set_outputs_high(
        uint8 address,
        uint16 pins)
{
    // in conf register, 1 is input, 0 is output
    // set as outputs as follows:
    // - invert pins variable, then AND with TCA6416A_state_config
    // existing outputs will stay outputs, new will become outputs with AND
    TCA6416A_state_config[TCA6416A_P0] &= ~(pins);
    TCA6416A_state_config[TCA6416A_P1] &= ~(pins >> 8);

    // in output register, 1 is high, 0 is low
    // set high as follows:
    // - simply OR with TCA6416A_state_output
    // high will stay high, low will change if it should
    TCA6416A_state_output[TCA6416A_P0] |= (pins);
    TCA6416A_state_output[TCA6416A_P1] |= (pins >> 8);

    TCA6416A_update_outputs(address);
}
