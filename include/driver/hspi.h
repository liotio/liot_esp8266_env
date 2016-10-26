/**
 * Hardware-supported HSPI driver for ESP8266
 *
 * HSPI is an acronym used by Espressif for * "Hardware Serial Peripheral
 * Interface" and stands for a the second SPI of the ESP8266 that can be used
 * to connect user-hardware as the first SPI is used for external serial flash.
 */

// https://github.com/MetalPhreak/ESP8266_SPI_Driver/blob/master/include/driver/spi.h
// https://github.com/MetalPhreak/ESP8266_SPI_Driver/blob/master/driver/spi.c

#ifndef HSPI_H
#define HSPI_H

#include "c_types.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "gpio.h"
#include "osapi.h"

#include "driver/spi_interface.h"
#include "driver/spi_register.h"


#define GPIO_INPUT                0
#define GPIO_OUTPUT               1

#define GPIO_LOW                  0
#define GPIO_HIGH                 1

#define HSPI                      1  // other SPI has number 0

#define FUNC_HSPI                 2  // HSPI is always func 2 on multiplexed GPIO
#define FUNC_GPIO_HSPI_CLK        FUNC_GPIO14
#define FUNC_GPIO_HSPI_MISO       FUNC_GPIO12
#define FUNC_GPIO_HSPI_MOSI       FUNC_GPIO13
#define FUNC_GPIO_HSPI_CS         FUNC_GPIO15

// define values for different byte orders
#define HSPI_MSB_FIRST            1
#define HSPI_LSB_FIRST            0

#define HSPI_CLK_PIN              9
#define HSPI_MISO_PIN             10
#define HSPI_MOSI_PIN             12
#define HSPI_CS_PIN               13

#define HSPI_MISO_GPIO_IN         GPIO_DIS_OUTPUT(HSPI_MISO_PIN)
#define HSPI_MISO_GPIO_READ       GPIO_INPUT_GET(HSPI_MISO_PIN)

#define HSPI_CS_GPIO_LOW          GPIO_OUTPUT_SET(HSPI_CS_PIN, GPIO_LOW)
#define HSPI_CS_GPIO_HIGH         GPIO_OUTPUT_SET(HSPI_CS_PIN, GPIO_HIGH)

#define HSPI_CLK_USE_DIV          0
#define HSPI_CLK_80MHZ_NODIV      1

// give multiplexed peripherals better names than names of SDIO interface
#define HSPI_MISO_MUX             PERIPHS_IO_MUX_MTDI_U
#define HSPI_MOSI_MUX             PERIPHS_IO_MUX_MTCK_U
#define HSPI_CLK_MUX              PERIPHS_IO_MUX_MTMS_U
#define HSPI_CS_MUX               PERIPHS_IO_MUX_MTDO_U

// define prediv and countdiv, set HSPI clock frequency to 4 MHz
#define HSPI_CLK_CNTDIV           2
#define HSPI_CLK_PREDIV           10
#define HSPI_CLK_FREQ             APB_CLK_FREQ / (SPI_CLK_PREDIV * SPI_CLK_CNTDIV)

// macro
#define HSPI_IS_BUSY              READ_PERI_REG(SPI_CMD(HSPI)) & SPI_USR

void HSPI_init();

void HSPI_init_gpio(
        uint8 sysclk_as_spiclk);

void HSPI_set_mode(
        uint8 spi_cpha,
        uint8 spi_cpol);

void HSPI_set_clock(
        uint16 prediv,
        uint8 cntdiv);

void HSPI_set_tx_byte_order(
        uint8 byte_order);

void HSPI_set_rx_byte_order(
        uint8 byte_order);

uint32 HSPI_transaction(
        uint8 cmd_bits,
        uint16 cmd_data,
        uint32 addr_bits,
        uint32 addr_data,
        uint32 dout_bits,
        uint32 dout_data,
        uint32 din_bits,
        uint32 dummy_bits);

#define HSPI_txd(bits, data) HSPI_transaction(0, 0, 0, 0, bits, (uint32) data, 0, 0)
#define HSPI_tx8(data)       HSPI_transaction(0, 0, 0, 0, 8,    (uint32) data, 0, 0)
#define HSPI_tx16(data)      HSPI_transaction(0, 0, 0, 0, 16,   (uint32) data, 0, 0)
#define HSPI_tx32(data)      HSPI_transaction(0, 0, 0, 0, 32,   (uint32) data, 0, 0)

#define HSPI_rxd(bits)       HSPI_transaction(0, 0, 0, 0, 0, 0, bits, 0)
#define HSPI_rx8()           HSPI_transaction(0, 0, 0, 0, 0, 0, 8,    0)
#define HSPI_rx16()          HSPI_transaction(0, 0, 0, 0, 0, 0, 16,   0)
#define HSPI_rx32()          HSPI_transaction(0, 0, 0, 0, 0, 0, 32,   0)

#define HSPI_trx8(data)      HSPI_transaction(0, 0, 0, 0, 8,    (uint32) data, 8, 0)

#endif
