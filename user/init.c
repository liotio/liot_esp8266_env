#include "user/init.h"

void SOFTAP_init_user()
{
    char buffer[64];
    struct softap_config config;
    wifi_softap_get_config(&config);

    os_memset(config.ssid, 0, 32);
    os_memset(config.password, 0, 64);
    os_sprintf(buffer, "%s_%u", AP_SSID, system_get_chip_id() % 10);
    os_memcpy(config.ssid, buffer, os_strlen(buffer));
    os_memcpy(config.password, AP_PASS, os_strlen(AP_PASS));
    config.authmode = AUTH_WPA2_PSK;
    config.ssid_len = 0;
    config.max_connection = 4;

    wifi_softap_set_config(&config);
}

void I2C_init_user()
{
    // set I2C pins as Open Collector
    I2C_init();
    // turn LED on SDA line off
    I2C_SDA_HIGH;
    os_delay_us(100);
}

void TCA6416A_init_user()
{
    TCA6416A_init(TCA6416A_ADDR);

    // turn on green and orange LED (P0.5 and P0.6) and Buzzer (P0.4)
    TCA6416A_set_outputs_low(TCA6416A_P0_4 | TCA6416A_P0_5 | TCA6416A_P0_6);
    // Proof that I/O Port Expander is working
    play_sound(400, 150, 1.0);
    // turn off orange LED (P0.6) and Buzzer (P0.4) again
    TCA6416A_set_outputs_high(TCA6416A_P0_4 | TCA6416A_P0_6);
}

void BME280_init_user()
{
    BME280_init(BME280_ADDR);
}

void BNO055_init_user()
{
    BNO055_init(BNO055_ADDR);

    /*
    uint16 data;
    data = BNO055_read_reg(BNO055_ADDR_HIGH, BNO055_REG_CHIP_ID);
    if (data == (uint16) -1) {
        os_printf("Error\n");
    } else {
        os_printf("Chip ID: %u\n", data);
    }
    */
}

void CC1101_init_user()
{
    // CC1101_init();

    CC1101_init_spi();
    powerUpReset();

    spiInitTrx();
    HSPI_transaction(0,0,8,0x02 | CC1101_WRITE_SINGLE,8,0x0E,0,0);

    os_delay_us(1000);
    spiInitTrx();
    os_printf("GDO0: %x\n", (uint8) HSPI_transaction(0,0,8,0x02 | CC1101_READ_SINGLE,0,0,8,0));
    spiInitTrx();
    os_printf("GDO1: %x\n", (uint8) HSPI_transaction(0,0,8,0x01 | CC1101_READ_SINGLE,0,0,8,0));
    spiInitTrx();
    os_printf("GDO2: %x\n", (uint8) HSPI_transaction(0,0,8,0x00 | CC1101_READ_SINGLE,0,0,8,0));

    /*
    if (!CC1101_set_channel(1)) {
        os_printf("Error setting channel");
    }
    if (!CC1101_set_id(1)) {
        os_printf("Error setting ID");
    }
    */
}

void HTTPD_init_user()
{
    HTTPD_init();
}

void TASK_init_user()
{
    // I2C task
    TASK_i2c_init();
}

void TIMER_init_user()
{
    TIMER_task_i2c_init();
}
