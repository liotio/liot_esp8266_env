#include "user/init.h"

void ICACHE_FLASH_ATTR MQTT_init_user()
{
    MQTT_init();
}

void ICACHE_FLASH_ATTR WIFI_init_user()
{
    WIFI_STATION_init_user();
    WIFI_SOFTAP_init_user();
}

void ICACHE_FLASH_ATTR WIFI_STATION_init_user()
{
    WIFI_connect(WIFI_connect_cb);
}

void ICACHE_FLASH_ATTR WIFI_SOFTAP_init_user()
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

void ICACHE_FLASH_ATTR I2C_init_user()
{
    // set I2C pins as Open Collector
    I2C_init();
    // turn LED on SDA line off
    I2C_SDA_HIGH;
    os_delay_us(100);
}

void ICACHE_FLASH_ATTR TCA6416A_init_user()
{
    os_printf("\nInit TCA6416A");
    TCA6416A_init(TCA6416A_ADDR);

    if (TCA6416A_initialized()) {
        // turn on green and orange LED (P0.5 and P0.6) and buzzer (P0.4)
        TCA6416A_set_outputs(TCA6416A_P0_5 | TCA6416A_P0_6, 0);
        // proof that I/O port expander is working
        play_sound(400, 150, 1.0);
        // turn off orange LED (P0.6) and buzzer (P0.4) again
        TCA6416A_set_outputs(TCA6416A_P0_6, 1);
    }
}

void ICACHE_FLASH_ATTR BME280_init_user()
{
    os_printf("\nInit BME280");
    BME280_init(BME280_ADDR);
}

void ICACHE_FLASH_ATTR BNO055_init_user()
{
    os_printf("\nInit BNO055");
    BNO055_init(BNO055_ADDR);
}

void ICACHE_FLASH_ATTR CC1101_init_user()
{
    os_printf("\nInit CC1101");
    CC1101_init();

    uint8_t i;
    uint8_t buf[3];
    CC1101_spi_read_burst(0x00, buf, 3);

    for (i = 0; i < 3; i++) {
        os_printf("\nGDO%u: %x", 2-i, buf[i]);
    }

    CC1101_set_channel(1);
    CC1101_set_id(1); // system_get_chip_id() % 10
}

void ICACHE_FLASH_ATTR TASK_init_user()
{
    // I2C task
    TASK_i2c_init();
}

void ICACHE_FLASH_ATTR TIMER_init_user()
{
    TIMER_task_i2c_init();
    // TIMER_task_spi_init();
}

void ICACHE_FLASH_ATTR HTTPD_init_user()
{
    os_printf("\nInit HTTPD");
    HTTPD_init();
}
