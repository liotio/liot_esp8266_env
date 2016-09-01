#include "user/timer.h"


static uint8 state;

/*
void hw_test_timer_cb()
{
    if (state) {
        state--;
        I2C_SDA_HIGH;
    } else {
        state = 100;
        I2C_SDA_LOW;
    }
}
*/

/*
void toggle(void *arg)
{
    uint8 input0, input1, i;

    input0 = TCA6416A_read_reg(TCA6416A_ADDR_LOW, TCA6416A_REG_INPUT_0);
    input1 = TCA6416A_read_reg(TCA6416A_ADDR_LOW, TCA6416A_REG_INPUT_1);

    os_printf("\nInput0: ");

    for (i = 8; i > 0; i--) {
        os_printf("%u", ((input0 >> (i-1)) & 1) ? 1 : 0);
    }

    os_printf(" - Input1: ");

    for (i = 8; i > 0; i--) {
        os_printf("%u", ((input1 >> (i-1)) & 1) ? 1 : 0);
    }

    // os_printf("%d\n", BMP280_read_temp(BMP280_ADDR_LOW));
    // data: 531028
    // dig_T!: 27981
    // dig_T2: 26967
    // dig_T3: -1000
    // 2671

    // os_printf("%u\n", BMP280_read_reg(BMP280_ADDR_LOW, BMP280_REG_RESET));

    if (state) {
        state = 0;
        I2C_SCK_LOW;
        // TCA6416A_write_reg(TCA6416A_ADDR, TCA6416A_REG_OUTPUT_0, ~(0));
    } else {
        state = 1;
        I2C_SCK_HIGH;
        // TCA6416A_write_reg(TCA6416A_ADDR, TCA6416A_REG_OUTPUT_0, ~(1<<5));
    }
}
*/

/*
void trigger(void *arg)
{
    // system_soft_wdt_stop();
    sint16 head, roll, ptch;
    uint64 data;

    data = BNO055_read_reg_48(BNO055_ADDR, BNO055_REG_EUL_HEAD_L);
    ptch = (sint16) (data & 0xFFFF) / 16;
    data = data >> 16;
    roll = (sint16) (data & 0xFFFF) / 16;
    data = data >> 16;
    head = (sint16) (data & 0xFFFF) / 16;
    // system_soft_wdt_restart();

    os_printf("\nROLL:  %d\n", roll);
    os_printf("PITCH: %d\n", ptch);

    if ((ptch < -45 && ptch >= -180)
            || (ptch > 45 && ptch <= 180)
            || (roll < -45 && roll >= -180)
            || (roll > 45 && roll <= 180)) {

        os_delay_us(500);
        TCA6416A_set_outputs_low(TCA6416A_ADDR, TCA6416A_P0_4);
        os_delay_us(500);

        play_sound( 400, 200, 0.06);

        os_delay_us(500);
        TCA6416A_set_outputs_high(TCA6416A_ADDR, TCA6416A_P0_4);
    }

    // data: 531028
    // dig_T!: 27981
    // dig_T2: 26967
    // dig_T3: -1000
    // 2671

    // os_printf("%u\n", BMP280_read_reg(BMP280_ADDR_LOW, BMP280_REG_RESET));

    if (state) {
        state = 0;
        I2C_SCK_LOW;
        // TCA6416A_write_reg(TCA6416A_ADDR, TCA6416A_REG_OUTPUT_0, ~(0));
    } else {
        state = 1;
        I2C_SCK_HIGH;
        // TCA6416A_write_reg(TCA6416A_ADDR, TCA6416A_REG_OUTPUT_0, ~(1<<5));
    }
}
*/
