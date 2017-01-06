#include "lib/wifi.h"

static os_timer_t WIFI_timer;
static wifi_callback_t WIFI_callback = NULL;
static uint8_t WIFI_status = STATION_IDLE;
static uint8_t WIFI_last_status = STATION_IDLE;

static void ICACHE_FLASH_ATTR WIFI_check_ip(
        void *arg);

void ICACHE_FLASH_ATTR WIFI_connect(
        wifi_callback_t callback)
{
    struct station_config station_conf;
    os_memset(&station_conf, 0, sizeof(struct station_config));
    WIFI_callback = callback;

    // write SSID / PASS to station config
    os_sprintf(station_conf.ssid, "%s", STA_SSID);
    os_sprintf(station_conf.password, "%s", STA_PASS);
    wifi_station_set_config(&station_conf);

    // set a timer to check whether got ip from router succeed or not.
    os_timer_disarm(&WIFI_timer);
    os_timer_setfn(&WIFI_timer, (os_timer_func_t *) WIFI_check_ip, NULL);
    os_timer_arm(&WIFI_timer, 100, 0);

    wifi_station_connect();
}

static ICACHE_FLASH_ATTR void WIFI_check_ip(
        void *arg)
{
    struct ip_info ip_config;

    os_timer_disarm(&WIFI_timer);
    wifi_get_ip_info(STATION_IF, &ip_config);
    WIFI_status = wifi_station_get_connect_status();

    if (WIFI_status == STATION_GOT_IP && ip_config.ip.addr != 0) {
        // if connected, keep checking if still connected every 10 seconds
        os_timer_arm(&WIFI_timer, 10000, 0);
        // show only one, annoying otherwise
        // os_printf("STATION_GOT_IP\n");
    } else {
        if (wifi_station_get_connect_status() == STATION_WRONG_PASSWORD) {
            os_printf("STATION_WRONG_PASSWORD\n");
            wifi_station_connect();
        } else if (wifi_station_get_connect_status() == STATION_NO_AP_FOUND) {
            os_printf("STATION_NO_AP_FOUND\n");
            wifi_station_connect();
        } else if (wifi_station_get_connect_status() == STATION_CONNECT_FAIL) {
            os_printf("STATION_CONNECT_FAIL\n");
            wifi_station_connect();
        } else {
            os_printf("STATION_IDLE\n");
        }
        os_timer_arm(&WIFI_timer, 500, 0);
    }

    // call Wi-Fi callback method if state has changed
    if (WIFI_status != WIFI_last_status) {
        WIFI_last_status = WIFI_status;
        if (WIFI_callback) {
            WIFI_callback(WIFI_status);
        }
    }
}
