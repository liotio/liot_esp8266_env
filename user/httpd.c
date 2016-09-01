#include "user/httpd.h"

static struct espconn httpdConn;
static esp_tcp httpdTcp;


void HTTPD_on_connection(
        void *arg);

void HTTPD_on_receive(
        void *arg,
        char *data,
        unsigned short len);


void HTTPD_init()
{
    httpdConn.type = ESPCONN_TCP;
    httpdConn.state = ESPCONN_NONE;
    httpdTcp.local_port = 80;
    httpdConn.proto.tcp = &httpdTcp;

    os_printf("\nInit HTTPD, conn=%p\n", &httpdConn);
    espconn_regist_connectcb(&httpdConn, HTTPD_on_connection);
    espconn_accept(&httpdConn);
}

void HTTPD_on_connection(
        void *arg)
{
    struct espconn *conn = arg;
    // only set recv callback, skip others
    espconn_regist_recvcb(conn, HTTPD_on_receive);
    espconn_set_opt(conn, ESPCONN_NODELAY);
}

void HTTPD_on_receive(
        void *arg,
        char *data,
        unsigned short len)
{
    struct espconn *conn = arg;

    char method[5];
    char location[256];

    http_request_method(data, method, 5);
    http_request_location(data, location, 256);

    if (strcmp(method,"GET") == 0) {

        if (strcmp(location,"/") == 0) {
            /*
            if (led_active == 0) {
                gpio_output_set(0, (1 << led_pin), 0, 0);
                led_active = 1;
                espconn_sent(conn, "LED is now ON", 13);
            } else {
                gpio_output_set((1 << led_pin), 0, 0, 0);
                led_active = 0;
                espconn_sent(conn, "LED is now OFF", 14);
            }
            */
            espconn_sent(conn, "OK", 2);
        }
    }

    espconn_disconnect(conn);
}
