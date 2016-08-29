#include "eagle_soc.h"
#include "c_types.h"

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "user_interface.h"
#include "espconn.h"
#include "pwm.h"

#include "driver/uart.h"
#include "driver/soft_i2c.h"
// #include "driver/bh1750.h"
// #include "driver/sht1x.h"

#include "driver/bmp280.h"
#include "driver/bno055.h"
#include "driver/tca6416a.h"
#include "libs/sounds.h"

#define DEBUG

#define REG_READ(_r) (*(volatile uint32 *)(_r))
#define WDEV_NOW() REG_READ(0x3ff20c00)
uint32 tick_now2 = 0;

// #include "wifi_config.h"

#define TCA6416A_ADDR   TCA6416A_ADDR_LOW
#define BNO055_ADDR     BNO055_ADDR_HIGH

typedef enum {
    FRC1_SOURCE = 0,
    NMI_SOURCE = 1,
} FRC1_TIMER_SOURCE_TYPE;

static os_timer_t timer;

static os_timer_t BNO055_timer;

static uint8 state;

void hw_test_timer_cb()
{
    if (state) {
        state--;
        I2C_SDA_HIGH;
    } else {
        state = 100;
        I2C_SDA_LOW;
    }

    /*
    static uint16 j = 0;
    j++;


    if( (WDEV_NOW() - tick_now2) >= 1000000 )
    {
        static u32 idx = 1;

        tick_now2 = WDEV_NOW();
        os_printf("b%u:%d\n",idx++,j);
        j = 0;
    }
    */
}

/*
#define WDEV_NOW (*(volatile uint32 *)(0x3ff20c00);
uint32 tick_now2 = 0;

void hw_test_timer_cb(void)
{
    static uint16 j = 0;
    j++;

    if((WDEV_NOW - tick_now2) >= 1000000 )
    {
        static u32 idx = 1;
        tick_now2 = WDEV_NOW;
        os_printf("b%u:%d\n",idx++,j);
        j = 0;
    }
}
*/

void toggle(void *arg)
{
    os_printf("%d\n", BMP280_read_temp(BMP280_ADDR_LOW));
    /*
    data: 531028
    dig_T!: 27981
    dig_T2: 26967
    dig_T3: -1000
    2671
     */

    // os_printf("%u\n", BMP280_read_reg(BMP280_ADDR_LOW, BMP280_REG_RESET));

    /*
    if (state) {
        state = 0;
        I2C_SCK_LOW;
        // TCA6416A_write_reg(TCA6416A_ADDR, TCA6416A_REG_OUTPUT_0, ~(0));
    } else {
        state = 1;
        I2C_SCK_HIGH;
        // TCA6416A_write_reg(TCA6416A_ADDR, TCA6416A_REG_OUTPUT_0, ~(1<<5));
    }
    */
}

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

    /*
    data: 531028
    dig_T!: 27981
    dig_T2: 26967
    dig_T3: -1000
    2671
     */

    // os_printf("%u\n", BMP280_read_reg(BMP280_ADDR_LOW, BMP280_REG_RESET));

    /*
    if (state) {
        state = 0;
        I2C_SCK_LOW;
        // TCA6416A_write_reg(TCA6416A_ADDR, TCA6416A_REG_OUTPUT_0, ~(0));
    } else {
        state = 1;
        I2C_SCK_HIGH;
        // TCA6416A_write_reg(TCA6416A_ADDR, TCA6416A_REG_OUTPUT_0, ~(1<<5));
    }
    */
}

void sys_init_done_cb() {
    os_printf("SDK version:%s\n", system_get_sdk_version());
    wifi_set_opmode(NULL_MODE);

    // GPIO_DIS_OUTPUT(5);

    // I2C pins as open collector
    I2C_gpio_init();
    // LED on SDA off
    I2C_SDA_HIGH;

    os_printf("...\n");

    if (BNO055_init(BNO055_ADDR_HIGH)) {
        os_printf("\n--- BNO055 init failed ---\n");
    }
    os_delay_us(100);


    // PIN_FUNC_SELECT(I2C_SDA_MUX, I2C_SDA_FUNC);

    // configure as open drain
    // GPIO_REG_WRITE(GPIO_PIN_ADDR(I2C_SDA_PIN), GPIO_REG_READ(GPIO_PIN_ADDR(I2C_SDA_PIN)) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE));
    // GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_SDA_PIN));

    // I2C_SDA_LOW;

    // bei multiplexed notwendig auf gpio zu setzen
    // PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
    // GPIO_OUTPUT_SET(12, 0);

    // state = 0;

    uint16 data;
    data = BNO055_read_reg(BNO055_ADDR_HIGH, BNO055_REG_CHIP_ID);
    if (data == (uint16) -1) {
        os_printf("Error\n");
    } else {
        os_printf("Chip ID: %u\n", data);
    }

    // os_delay_us(100);

    TCA6416A_init();
    // TCA6416A_set_outputs_low(TCA6416A_ADDR_LOW, TCA6416A_P0_4 | TCA6416A_P0_5 | TCA6416A_P0_6);
    TCA6416A_set_outputs_low(TCA6416A_ADDR_LOW, TCA6416A_P0_5);

    /*
    hw_timer_init(FRC1_SOURCE,1);
    hw_timer_set_func(hw_test_timer_cb);
    hw_timer_arm(100);
    */

    /*
    play_sound( 400, 200, 1.0);
    os_delay_us(200000);
    play_sound( 400, 200, 1.0);
    os_delay_us(200000);
    play_sound( 400, 200, 1.0);
    */

    /*
    play_sound( 200, 2000, 1.0);
    play_sound( 400, 2000, 1.0);
    play_sound( 600, 2000, 1.0);
    play_sound(1000, 2000, 1.0);
    play_sound(2000, 2000, 1.0);
    play_sound(3000, 2000, 1.0);
    play_sound(4000, 2000, 1.0);
    play_sound(5000, 2000, 1.0);
    play_sound(6000, 2000, 1.0);
    play_sound(7000, 2000, 1.0);
    play_sound(8000, 2000, 1.0);
    play_sound(9000, 2000, 1.0);
    */

    // TCA6416A_set_outputs_high(TCA6416A_ADDR_LOW, TCA6416A_P0_4 | TCA6416A_P0_6);

    os_timer_setfn(&BNO055_timer, (os_timer_func_t *) trigger, NULL);
    os_timer_arm(&BNO055_timer, 250, 1);
}

void user_init()
{
    UART_SetBaudrate(UART0, BIT_RATE_115200);
    // system_init_done_cb(sys_init_done_cb);

    I2C_gpio_init();
    I2C_SDA_HIGH;

    hw_timer_init(FRC1_SOURCE,1);
    hw_timer_set_func(hw_test_timer_cb);
    hw_timer_arm(100);
}
