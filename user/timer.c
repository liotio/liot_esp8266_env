#include "user/timer.h"

static os_timer_t TIMER_task_i2c_timer;
static os_timer_t TIMER_task_spi_timer;

void TIMER_task_i2c_init()
{
    // start timer every 10 seconds
    os_timer_setfn(&TIMER_task_i2c_timer, (os_timer_func_t *) TIMER_task_i2c, NULL);
    os_timer_arm(&TIMER_task_i2c_timer, 10000, 1);
}

void TIMER_task_i2c_stop()
{
    os_timer_disarm(&TIMER_task_i2c_timer);
}

void TIMER_task_i2c(void *arg)
{
    TASK_i2c_restart();
}

void TIMER_task_spi_init()
{
    os_timer_setfn(&TIMER_task_spi_timer, (os_timer_func_t *) TIMER_task_spi, NULL);
    os_timer_arm(&TIMER_task_spi_timer, 1000, 1);
}

void TIMER_task_spi_stop()
{
    os_timer_disarm(&TIMER_task_spi_timer);
}

void TIMER_task_spi(void *arg)
{
    uint8_t *data = "Hello World!";
    CC1101_send_packet(0, 1, data, sizeof(data));
    CC1101_send_packet(100, 1, data, sizeof(data));
    os_printf("\nSending: %s", data);
}

/*
void hw_test_timer_cb()
{
}

void hw_test_timer_init()
{
    // hw_timer_init(FRC1_SOURCE,1);
    // hw_timer_set_func(hw_test_timer_cb);
    // hw_timer_arm(100);
}
*/
