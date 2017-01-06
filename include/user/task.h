#ifndef USER_TASK_H
#define USER_TASK_H

#include "os_type.h"
#include "user_config.h"

#include "driver/bno055.h"

#define TASK_i2c_prio             0
#define TASK_i2c_queue_len        1

void ICACHE_FLASH_ATTR TASK_i2c(
        os_event_t *events);

void ICACHE_FLASH_ATTR TASK_i2c_init();

void ICACHE_FLASH_ATTR TASK_i2c_restart();

#endif
