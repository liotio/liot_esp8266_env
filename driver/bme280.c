#include "driver/bme280.h"

// calibration values
uint16 _dig_T1;
sint16 _dig_T2;
sint16 _dig_T3;
uint16 _dig_P1;
sint16 _dig_P2;
sint16 _dig_P3;
sint16 _dig_P4;
sint16 _dig_P5;
sint16 _dig_P6;
sint16 _dig_P7;
sint16 _dig_P8;
sint16 _dig_P9;
uint8  _dig_H1;
sint16 _dig_H2;
uint8  _dig_H3;

// sensor address
uint8 _address;

void BME280_init(uint8 address)
{
    _address = address;

    _dig_T1 = (uint16) BME280_read_reg(BME280_REG_DIG_T1_LSB, 16);
    _dig_T2 = (sint16) BME280_read_reg(BME280_REG_DIG_T2_LSB, 16);
    _dig_T3 = (sint16) BME280_read_reg(BME280_REG_DIG_T3_LSB, 16);
    _dig_P1 = (uint16) BME280_read_reg(BME280_REG_DIG_P1_LSB, 16);
    _dig_P2 = (sint16) BME280_read_reg(BME280_REG_DIG_P2_LSB, 16);
    _dig_P3 = (sint16) BME280_read_reg(BME280_REG_DIG_P3_LSB, 16);
    _dig_P4 = (sint16) BME280_read_reg(BME280_REG_DIG_P4_LSB, 16);
    _dig_P5 = (sint16) BME280_read_reg(BME280_REG_DIG_P5_LSB, 16);
    _dig_P6 = (sint16) BME280_read_reg(BME280_REG_DIG_P6_LSB, 16);
    _dig_P7 = (sint16) BME280_read_reg(BME280_REG_DIG_P7_LSB, 16);
    _dig_P8 = (sint16) BME280_read_reg(BME280_REG_DIG_P8_LSB, 16);
    _dig_P9 = (sint16) BME280_read_reg(BME280_REG_DIG_P9_LSB, 16);
    _dig_H1 = (uint8)  BME280_read_reg(BME280_REG_DIG_H1_LSB,  8);
    _dig_H2 = (sint16) BME280_read_reg(BME280_REG_DIG_H2_LSB, 16);
    _dig_H3 = (uint8)  BME280_read_reg(BME280_REG_DIG_H3_LSB,  8);
}


uint64 BME280_read_reg(
        uint8 reg,
        uint8 bits)
{
    uint64 result = 0;

    if (I2C_start(_address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BME280");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write register to BME280");
        return -1;
    }

    if (I2C_start(_address, I2C_SLAVE_READ)) {
        os_printf("\nCannot init reading from BME280");
        return -1;
    }

    while (bits > 8) {
        result = (result << 8) | I2C_read_ack();
        bits -= 8;
    }

    result = (result << bits) | (I2C_read_nack() >> (8 - bits));

    I2C_stop();

    return result;
}

uint8 BME280_write_reg(
        uint8 reg,
        uint8 data)
{
    if (I2C_start(_address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BME280");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write register to BME280");
        return -1;
    }
    if (I2C_write(data)) {
        os_printf("\nCannot write data to BME280");
        return -1;
    }

    return 0;
}

int32 BME280_read_temp()
{
    uint32 data;
    sint32 adc_T, var1, var2, T;

    data = BME280_read_reg(BME280_REG_TEMP_MSB, 20);

    #ifdef DEBUG
    os_printf("\ndata:   %u", data);
    os_printf("\ndig_T!: %u", _dig_T1);
    os_printf("\ndig_T2: %d", _dig_T2);
    os_printf("\ndig_T3: %d", _dig_T3);
    #endif

    adc_T = (sint32) data;
    var1 = ((((adc_T>>3) - ((sint32)_dig_T1<<1))) * ((sint32)_dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((sint32)_dig_T1)) * ((adc_T>>4) - ((sint32)_dig_T1))) >> 12) * ((sint32)_dig_T3)) >> 14;

    return ((var1 + var2) * 5 + 128) >> 8;
}
