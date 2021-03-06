/**
 * Software I2C for ESP8266
 */

#ifndef SOFT_I2C_H
#define SOFT_I2C_H

#include "c_types.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "gpio.h"
#include "osapi.h"

#define GPIO_INPUT      0
#define GPIO_OUTPUT     1

#define GPIO_LOW        0
#define GPIO_HIGH       1

#define I2C_SDA_PIN     2
#define I2C_SCK_PIN     4
#define I2C_SDA_MUX     PERIPHS_IO_MUX_GPIO2_U
#define I2C_SCL_MUX     PERIPHS_IO_MUX_GPIO4_U
#define I2C_SDA_FUNC    FUNC_GPIO2
#define I2C_SCL_FUNC    FUNC_GPIO4

#define I2C_SEND_ACK    0
#define I2C_SEND_NACK   1

#define I2C_SLAVE_WRITE 0
#define I2C_SLAVE_READ  1

#define I2C_SDA_IN      GPIO_DIS_OUTPUT(I2C_SDA_PIN)
#define I2C_SDA_HIGH    GPIO_OUTPUT_SET(I2C_SDA_PIN, GPIO_HIGH)
#define I2C_SDA_LOW     GPIO_OUTPUT_SET(I2C_SDA_PIN, GPIO_LOW)
#define I2C_SDA_READ    GPIO_INPUT_GET(I2C_SDA_PIN)

#define I2C_SCK_IN      GPIO_DIS_OUTPUT(I2C_SCK_PIN)
#define I2C_SCK_HIGH    GPIO_OUTPUT_SET(I2C_SCK_PIN, GPIO_HIGH)
#define I2C_SCK_HIGHSTR I2C_SCK_IN; I2C_DELAY; while (!I2C_SCK_READ) I2C_DELAY;
#define I2C_SCK_LOW     GPIO_OUTPUT_SET(I2C_SCK_PIN, GPIO_LOW)
#define I2C_SCK_READ    GPIO_INPUT_GET(I2C_SCK_PIN)

#define I2C_DELAY       os_delay_us(4); system_soft_wdt_feed();

#define I2C_TIMEOUT     50000

// set gpio's to open drain
void I2C_init();

// write start sequence and device address + read / write bit
uint8 I2C_start(
    uint8 slave_addr,
    uint8 readwrite);

// write stop sequence
void I2C_stop();

// transmit a byte and return (n)ack
uint8 I2C_write(
    uint8 data);

uint8 I2C_write_single(
        uint8 slave_addr,
        uint8 reg_addr,
        uint8 data);

uint8 I2C_write_buffer(
    uint8 slave_addr,
    uint8 reg_addr,
    uint8 length,
    uint8 const *data);

uint8 I2C_read_init(
		uint8 slave_addr,
		uint8 reg_addr);

// receive one byte and generate (n)ack
uint8 I2C_read(uint8 ack);

uint8 I2C_read_single(
        uint8 slave_addr,
        uint8 reg_addr);

uint64 I2C_read_multiple_msb(
        uint8 slave_addr,
        uint8 reg_addr,
        uint8 bits);

uint64 I2C_read_multiple_lsb(
        uint8 slave_addr,
        uint8 reg_addr,
        uint8 bits);

uint8 I2C_read_buffer(
    uint8 slave_addr,
    uint8 reg_addr,
    uint8 length,
    uint8 *data);

// receive one byte and generate ack
uint8 I2C_read_ack();
// receive one byte and generate nack
uint8 I2C_read_nack();

#endif
