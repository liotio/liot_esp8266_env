#ifndef USER_MQTT_H
#define USER_MQTT_H

#include "mqtt_config.h"

#include "lib/mqtt/mqtt.h"

MQTT_Client mqttClient;

void MQTT_init();

static void MQTT_connected_cb(
        uint32_t *args);

static void MQTT_disconnected_cb(
        uint32_t *args);

static void MQTT_published_cb(
        uint32_t *args);

static void MQTT_data_cb(
        uint32_t *args,
        const char* topic,
        uint32_t topic_len,
        const char *data,
        uint32_t data_len);

#endif
