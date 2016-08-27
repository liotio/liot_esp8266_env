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
#define TCA6416A_REG_CONF_0       0x06
#define TCA6416A_REG_CONF_1       0x07

#define TCA6416A_GPIO0            0x00
#define TCA6416A_GPIO1            0x01
#define TCA6416A_GPIO2            0x02
#define TCA6416A_GPIO3            0x03
#define TCA6416A_GPIO4            0x04
#define TCA6416A_GPIO5            0x05
#define TCA6416A_GPIO6            0x06
#define TCA6416A_GPIO7            0x07
#define TCA6416A_GPIO8            0x08
#define TCA6416A_GPIO9            0x09
#define TCA6416A_GPIO10           0x0A
#define TCA6416A_GPIO11           0x0B
#define TCA6416A_GPIO12           0x0C
#define TCA6416A_GPIO13           0x0D
#define TCA6416A_GPIO14           0x0E
#define TCA6416A_GPIO15           0x0F

#define TCA6416A_GPIO_IN          0x01
#define TCA6416A_GPIO_OUT         0x00

uint8 state_reg_input_0;
uint8 state_reg_input_1;
uint8 state_reg_output_0;
uint8 state_reg_output_1;
uint8 state_reg_pol_inv_0;
uint8 state_reg_pol_inv_1;
uint8 state_reg_conf_0;
uint8 state_reg_conf_1;

// ...
uint16 TCA6416A_read_reg(
        uint8 address,
        uint8 reg);

// ...
uint8 TCA6416A_write_reg(
        uint8 address,
        uint8 reg,
        uint8 data);

// ...
uint8 TCA6416A_input_get(
        uint8 address,
        uint8 gpio);

// ...

#endif
