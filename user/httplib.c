#include "user/httplib.h"

#include "osapi.h"

void ICACHE_FLASH_ATTR
http_request_method(const char * http_request, char * method, unsigned short length)
{
    unsigned short i = 0;

    do {
        method[i] = http_request[i];

        i++;
    } while (*(http_request+i) != ' ' && i < length);

    method[i] = '\0';
}

void ICACHE_FLASH_ATTR
http_request_location(const char * http_request, char * location, unsigned short length)
{
    unsigned short i = 0;
    unsigned short j = 0;

    do {
        i++;
    } while (*(http_request+i) != ' ');

    i++;

    do {
        location[j] = http_request[i];

        i++;
        j++;
    } while (*(http_request+i) != ' ' && j < length);

    location[i] = '\0';
}
