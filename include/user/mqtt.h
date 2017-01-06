#ifndef USER_MQTT_H
#define USER_MQTT_H

#include "mqtt_config.h"
#include "lib/mqtt/mqtt.h"
#include "mem.h"

MQTT_Client mqttClient;

void ICACHE_FLASH_ATTR WIFI_connect_cb(
        uint8_t status);

void ICACHE_FLASH_ATTR MQTT_init();

void ICACHE_FLASH_ATTR MQTT_publish(
        const char* topic,
        const char* data,
        int data_length,
        int qos,
        int retain);

static ICACHE_FLASH_ATTR void MQTT_connected_cb(
        uint32_t *args);

static ICACHE_FLASH_ATTR void MQTT_disconnected_cb(
        uint32_t *args);

static ICACHE_FLASH_ATTR void MQTT_published_cb(
        uint32_t *args);

static ICACHE_FLASH_ATTR void MQTT_data_cb(
        uint32_t *args,
        const char* topic,
        uint32_t topic_len,
        const char *data,
        uint32_t data_len);

#endif
