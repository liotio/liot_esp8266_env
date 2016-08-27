PROGRAM       = esprogram
FLASH_SIZE    = 32m
ESPBAUD       = 921600
ESPPORT       = /dev/ttyUSB0

EXTRA_SRCS     = $(ROOT)/sdk/driver_lib

include $(ESP_OPEN_SDK)/common.mk
