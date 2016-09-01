#ifndef USER_TASK_H
#define USER_TASK_H

#include "os_type.h"

#define TASK_i2c_prio             0
#define TASK_i2c_queue_len        1
os_event_t TASK_i2c_queue[TASK_i2c_queue_len];

void TASK_i2c(os_event_t *events);

#endif
