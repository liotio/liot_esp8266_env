#include "user/httpd.h"

static struct espconn httpdConn;
static esp_tcp httpdTcp;


void ICACHE_FLASH_ATTR HTTPD_on_connection(
        void *arg);

void ICACHE_FLASH_ATTR HTTPD_on_receive(
        void *arg,
        char *data,
        unsigned short len);


void ICACHE_FLASH_ATTR HTTPD_init()
{
    httpdConn.type = ESPCONN_TCP;
    httpdConn.state = ESPCONN_NONE;
    httpdTcp.local_port = 80;
    httpdConn.proto.tcp = &httpdTcp;

    espconn_regist_connectcb(&httpdConn, HTTPD_on_connection);
    espconn_accept(&httpdConn);
}

void ICACHE_FLASH_ATTR HTTPD_on_connection(
        void *arg)
{
    struct espconn *conn = arg;
    // only set recv callback, skip others
    espconn_regist_recvcb(conn, HTTPD_on_receive);
    espconn_set_opt(conn, ESPCONN_NODELAY);
}

void ICACHE_FLASH_ATTR HTTPD_on_receive(
        void *arg,
        char *data,
        unsigned short len)
{
    struct espconn *conn = arg;

    char method[5];
    char location[256];
    char buffer[256];

    http_request_method(data, method, 5);
    http_request_location(data, location, 256);

    if (strcmp(method,"GET") == 0) {

        if (strcmp(location,"/") == 0) {

            sint32 temp = BME280_temp_sint;
            sint32 temp_pre = temp / 100;
            if (temp < 0) temp *= -1;
            sint32 temp_post = temp % 100;

            uint32 hum = BME280_hum_uint;
            uint32 hum_pre = hum / 1024;
            uint32 hum_post = hum % 1024;

            uint32 press = BME280_press_uint / 256;
            uint32 press_pre = press / 100;
            uint32 press_post = press % 100;

            os_sprintf(buffer, "\nTEMP: %d.%u degC\nHUM: %u.%u pctRH\nPRESS: %u.%u hPa", temp_pre, temp_post, hum_pre, hum_post, press_pre, press_post);

            espconn_sent(conn, buffer, os_strlen(buffer));
        }
    }

    espconn_disconnect(conn);
}
