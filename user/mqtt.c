#include "user/mqtt.h"

void ICACHE_FLASH_ATTR WIFI_connect_cb(
        uint8_t status)
 {
    if (status == STATION_GOT_IP) {
        os_printf("MQTT_Connect\n");
        MQTT_Connect(&mqttClient);
    } else {
        os_printf("MQTT_Disconnect\n");
        MQTT_Disconnect(&mqttClient);
    }
}

void ICACHE_FLASH_ATTR MQTT_init()
{
    MQTT_InitConnection(&mqttClient, MQTT_HOST, MQTT_PORT, DEFAULT_SECURITY);

    MQTT_InitClient(&mqttClient, MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS,
            MQTT_KEEPALIVE, MQTT_CLEAN_SESSION);
    MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
    MQTT_OnConnected(&mqttClient, MQTT_connected_cb);
    MQTT_OnDisconnected(&mqttClient, MQTT_disconnected_cb);
    MQTT_OnPublished(&mqttClient, MQTT_published_cb);
    MQTT_OnData(&mqttClient, MQTT_data_cb);
}

void ICACHE_FLASH_ATTR MQTT_publish(
        const char* topic,
        const char* data,
        int data_length,
        int qos,
        int retain)
{
    MQTT_Publish(&mqttClient, topic, data, data_length, qos, retain);
}

static ICACHE_FLASH_ATTR void MQTT_connected_cb(
        uint32_t *args)
{
    MQTT_Client* client = (MQTT_Client*) args;
    os_printf("\nMQTT: Connected");

    // MQTT_Subscribe(client, "led", 0);

    // MQTT_Subscribe(client, "mqtt/topic/1", 1);
    // MQTT_Subscribe(client, "mqtt/topic/2", 2);

    // MQTT_Publish(client, "mqtt/topic/0", "hello0", 6, 0, 0);
    // MQTT_Publish(client, "mqtt/topic/1", "hello1", 6, 1, 0);
    // MQTT_Publish(client, "mqtt/topic/2", "hello2", 6, 2, 0);
}

static ICACHE_FLASH_ATTR void MQTT_disconnected_cb(
        uint32_t *args)
{
    MQTT_Client* client = (MQTT_Client*) args;
    os_printf("\nMQTT: Disconnected");
}

static ICACHE_FLASH_ATTR void MQTT_published_cb(
        uint32_t *args)
{
    MQTT_Client* client = (MQTT_Client*) args;
    os_printf("\nMQTT: Published");
}

static ICACHE_FLASH_ATTR void MQTT_data_cb(
        uint32_t *args,
        const char* topic,
        uint32_t topic_len,
        const char *data,
        uint32_t data_len)
{
    char *topicBuf = (char*) os_zalloc(topic_len + 1);
    char *dataBuf = (char*) os_zalloc(data_len + 1);

    MQTT_Client* client = (MQTT_Client*) args;
    os_memcpy(topicBuf, topic, topic_len);
    topicBuf[topic_len] = 0;
    os_memcpy(dataBuf, data, data_len);
    dataBuf[data_len] = 0;
    os_printf("\nReceive topic: %s, data: %s\n", topicBuf, dataBuf);

    if (strcmp(topicBuf, "led") == 0) {

        uint8_t i = 0;
        uint8_t color = 0;
        uint8_t grb[3] = {0, 0, 0};
        uint8_t leds[12];

        while (i < data_len) {
            if (dataBuf[i] > 47 && dataBuf[i] < 58) {
                grb[color] = grb[color] * 10 + (dataBuf[i] - 48);
            } else if (dataBuf[i] == ';') {
                color++;
            }
            i++;
        }

        os_memcpy(&leds[0], grb, sizeof(grb));
        os_memcpy(&leds[3], grb, sizeof(grb));
        os_memcpy(&leds[6], grb, sizeof(grb));
        os_memcpy(&leds[9], grb, sizeof(grb));

        // WS2812_write(leds, sizeof(leds));
    }

    os_free(topicBuf);
    os_free(dataBuf);
}
