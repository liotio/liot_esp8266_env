#include "eagle_soc.h"
#include "c_types.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "user_interface.h"
#include "espconn.h"

#include "user_config.h"
#include "user_types.h"
#include "wifi_config.h"

#include "driver/uart.h"
#include "driver/soft_i2c.h"

#include "driver/bme280.h"
#include "driver/bno055.h"
#include "driver/cc1101.h"
#include "driver/tca6416a.h"

#include "user/httpd.h"
#include "user/init.h"
#include "user/interrupt.h"
#include "user/sound.h"
#include "user/timer.h"

void user_init();
void user_start();

void user_init()
{
    wifi_set_opmode(SOFTAP_MODE);
    UART_SetBaudrate(UART0, BIT_RATE_115200);
    os_printf("SDK version: %s", system_get_sdk_version());
    system_init_done_cb(user_start);
}

void user_start()
{
    SOFTAP_init_user();

    I2C_init_user();
    TCA6416A_init_user();
    BME280_init_user();
    BNO055_init_user();
    // CC1101_init_user();

    os_delay_us(1000000);

    TASK_init_user();
    // INTERRUPT_init_user();
    TIMER_init_user();
    HTTPD_init_user();
}
