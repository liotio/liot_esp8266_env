PROGRAM       = liot_esp8266_env
FLASH_SIZE    = 32m
ESPBAUD       = 921600
# ESPPORT     = set in projects local.mk file

EXTRA_SRCS     = $(ROOT)/sdk/driver_lib

include $(ESP_EASY_SDK)/common.mk
