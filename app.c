#include "eagle_soc.h"
#include "c_types.h"

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"
#include "espconn.h"
#include "pwm.h"
#include "mem.h"

#include "uart.h"
#include "wifi_config.h"

static const int pin = 2;
static volatile os_timer_t some_timer;

void some_timerfunc(void *arg)
{
  if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & (1 << pin))
  {
    // set gpio low
    gpio_output_set(0, (1 << pin), 0, 0);
    // os_printf("\r\nAUS\r\n");
  }
  else
  {
    // set gpio high
    gpio_output_set((1 << pin), 0, 0, 0);
    // os_printf("\r\nAN\r\n");
  }
}

void systemInitDoneCB() {
    // configure UART TXD to be GPIO1, set as output
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1);
    gpio_output_set(0, 0, (1 << pin), 0);

    // setup timer (1000ms, repeating)
    os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);
    os_timer_arm(&some_timer, 1000, 1);

    // wifi_set_opmode(STATION_MODE);
    // struct station_config config = (struct station_config *)
    //          zalloc(sizeof(struct station_config));
    // sprintf(config->ssid, AP_SSID);
    // sprintf(config->password, AP_PASS);
    // wifi_station_set_config(config);
    // free(config);
    // wifi_station_connect();
}

void user_init()
{
  gpio_init();

  // uart_init_2(0, 115200);

  system_init_done_cb(systemInitDoneCB);
}
