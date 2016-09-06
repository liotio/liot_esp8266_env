#include "driver/tca6416a.h"

static uint8 TCA6416A_state_input[2];
static uint8 TCA6416A_state_output[2];
static uint8 TCA6416A_state_polinv[2];
static uint8 TCA6416A_state_config[2];

static uint8 _address;

static void TCA6416A_update_outputs();

uint16 TCA6416A_init(
        uint8 address)
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
    GPIO_OUTPUT_SET(0, 1);

    _address = address;

    os_printf("\nInit TCA6416A");

    TCA6416A_reset();

    TCA6416A_state_output[TCA6416A_P0] = 0xFF; // default value 1111 1111
    TCA6416A_state_output[TCA6416A_P1] = 0xFF; // default value 1111 1111
    TCA6416A_state_polinv[TCA6416A_P0] = 0x00; // default value 0000 0000
    TCA6416A_state_polinv[TCA6416A_P1] = 0x00; // default value 0000 0000
    TCA6416A_state_config[TCA6416A_P0] = 0xFF; // default value 1111 1111
    TCA6416A_state_config[TCA6416A_P1] = 0xFF; // default value 1111 1111

    // TCA6416A_state_input[TCA6416A_P0] =
    // TCA6416A_state_input[TCA6416A_P1] =
}

void TCA6416A_reset()
{
    os_delay_us(10);
    GPIO_OUTPUT_SET(0, 0);
    os_delay_us(10);
    GPIO_OUTPUT_SET(0, 1);
}

uint8 TCA6416A_get_input(
        TCA6416A_gpio gpio)
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

    data = I2C_read_single(_address, gpio);
    if (data == -1) {
        return -1;
    }

    return data & BIT(gpio);
}

uint16 TCA6416A_get_input_diff()
{
    uint8 input0, input1;
    uint16 result = 0;

    input0 = I2C_read_single(_address, TCA6416A_P0);
    input1 = I2C_read_single(_address, TCA6416A_P1);

    // determine difference between last and new input states
    result |= (input0 ^ TCA6416A_state_input[TCA6416A_P0]);
    result |= (input1 ^ TCA6416A_state_input[TCA6416A_P1]) << 8;

    // update input states
    TCA6416A_state_input[TCA6416A_P0] = input0;
    TCA6416A_state_input[TCA6416A_P1] = input1;

    return result;
}

void TCA6416A_set_outputs_low(
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

    TCA6416A_update_outputs();
}

void TCA6416A_set_outputs_high(
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

    TCA6416A_update_outputs(_address);
}

static void TCA6416A_update_outputs()
{
    I2C_write_single(_address, TCA6416A_REG_CONFIG_0,
            TCA6416A_state_config[TCA6416A_P0]);

    I2C_write_single(_address, TCA6416A_REG_CONFIG_1,
            TCA6416A_state_config[TCA6416A_P1]);

    I2C_write_single(_address, TCA6416A_REG_OUTPUT_0,
            TCA6416A_state_output[TCA6416A_P0]);

    I2C_write_single(_address, TCA6416A_REG_OUTPUT_1,
            TCA6416A_state_output[TCA6416A_P1]);

    I2C_stop();
}
