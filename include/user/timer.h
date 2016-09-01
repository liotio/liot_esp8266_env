#ifndef USER_TIMER_H
#define USER_TIMER_H

#include "osapi.h"
#include "os_type.h"

#include "driver/soft_i2c.h"

void TIMER_task_i2c_init();

void TIMER_task_i2c_stop();

void TIMER_task_i2c(void *arg);

#endif
