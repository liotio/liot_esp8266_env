#ifndef USER_TIMER_H
#define USER_TIMER_H

#include "osapi.h"
#include "os_type.h"

#include "driver/soft_i2c.h"
#include "driver/cc1101.h"

void TIMER_task_i2c_init();

void TIMER_task_i2c_stop();

void TIMER_task_i2c(void *arg);

void TIMER_task_spi_init();

void TIMER_task_spi_stop();

void TIMER_task_spi(void *arg);

#endif
