#ifndef WIFI_H
#define WIFI_H

#include "user_interface.h"
#include "osapi.h"
#include "espconn.h"
#include "os_type.h"
#include "mem.h"

#include "wifi_config.h"

#include "lib/mqtt/mqtt_msg.h"
#include "lib/mqtt/debug.h"

#include "user_config.h"

typedef void (*wifi_callback_t)(uint8_t);

void ICACHE_FLASH_ATTR WIFI_connect(
        wifi_callback_t callback);

#endif
