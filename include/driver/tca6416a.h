#ifndef TCA6416A_H
#define TCA6416A_H

#include "driver/soft_i2c.h"

#define TCA6416A_ADDR_LOW         0x20
#define TCA6416A_ADDR_HIGH        0x21

#define TCA6416A_REG_INPUT_0      0x00
#define TCA6416A_REG_INPUT_1      0x01
#define TCA6416A_REG_OUTPUT_0     0x02
#define TCA6416A_REG_OUTPUT_1     0x03
#define TCA6416A_REG_POL_INV_0    0x04
#define TCA6416A_REG_POL_INV_1    0x05
#define TCA6416A_REG_CONFIG_0     0x06
#define TCA6416A_REG_CONFIG_1     0x07

#define TCA6416A_GPIO_IN          0x01
#define TCA6416A_GPIO_OUT         0x00

typedef enum {
    TCA6416A_P0 = 0x0,
    TCA6416A_P1 = 0x1
} TCA6416A_port;

typedef enum {
    TCA6416A_P0_0 = 0x0001,
    TCA6416A_P0_1 = 0x0002,
    TCA6416A_P0_2 = 0x0004,
    TCA6416A_P0_3 = 0x0008,
    TCA6416A_P0_4 = 0x0010,
    TCA6416A_P0_5 = 0x0020,
    TCA6416A_P0_6 = 0x0040,
    TCA6416A_P0_7 = 0x0080,
    TCA6416A_P1_0 = 0x0100,
    TCA6416A_P1_1 = 0x0200,
    TCA6416A_P1_2 = 0x0400,
    TCA6416A_P1_3 = 0x0800,
    TCA6416A_P1_4 = 0x1000,
    TCA6416A_P1_5 = 0x2000,
    TCA6416A_P1_6 = 0x4000,
    TCA6416A_P1_7 = 0x8000
} TCA6416A_gpio;

uint8 TCA6416A_state_input[2];

void TCA6416A_init(
        uint8 address);

uint8 TCA6416A_initialized();

void TCA6416A_reset();

uint8 TCA6416A_get_input(
        TCA6416A_gpio gpio);

uint16 TCA6416A_get_input_diff();

void TCA6416A_set_outputs(
        uint16 pins,
		uint8 levels);

#endif
