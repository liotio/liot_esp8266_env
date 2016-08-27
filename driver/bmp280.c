#include "driver/bmp280.h"

uint16 BMP280_read_reg(
        uint8 address,
        uint8 reg)
{
    uint8 msb;

    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BMP280\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write to BMP280\n");
        return -1;
    }

    // restart and read register data
    if (I2C_start(address, I2C_SLAVE_READ)) {
        os_printf("\nCannot init reading from BMP280\n");
        return -1;
    }

    msb = I2C_read_nack();
    I2C_stop();

    return msb;
}

uint32 BMP280_read_reg_20(
        uint8 address,
        uint8 reg)
{
    uint8 msb, lsb, xsb;

    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BMP280\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write to BMP280\n");
        return -1;
    }

    // restart and read register data
    if (I2C_start(address, I2C_SLAVE_READ)) {
        os_printf("\nCannot init reading from BMP280\n");
        return -1;
    }

    msb = I2C_read_ack();
    lsb = I2C_read_ack();
    xsb = I2C_read_nack();
    I2C_stop();

    return msb << 12 | lsb << 4 | xsb >> 4;
}

uint32 BMP280_read_dig(
        uint8 address,
        uint8 reg)
{
    uint8 msb, lsb;

    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BMP280\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write to BMP280\n");
        return -1;
    }

    // restart and read register data
    if (I2C_start(address, I2C_SLAVE_READ)) {
        os_printf("\nCannot find BMP280\n");
        return -1;
    }

    lsb = I2C_read_ack();
    msb = I2C_read_nack();
    I2C_stop();

    return msb << 8 | lsb;
}

uint8 BMP280_write_reg(
        uint8 address,
        uint8 reg,
        uint8 data)
{
    // write register to read from
    if (I2C_start(address, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to BMP280\n");
        return -1;
    }
    if (I2C_write(reg)) {
        os_printf("\nCannot write to BMP280\n");
        return -1;
    }
    if (I2C_write(data)) {
        os_printf("\nCannot write to BMP280\n");
        return -1;
    }

    os_printf("Writing to BMP280 successful");

    return 0;
}

int32 BMP280_read_temp(
        uint8 address)
{
    uint32 data;
    sint32 adc_T, var1, var2, T;
    uint16 dig_T1;
    sint16 dig_T2, dig_T3;

    data = BMP280_read_reg_20(address, BMP280_REG_TEMP_MSB);
    os_printf("\ndata: %u\n", data);

    if (data == -1) {
        os_printf("\nError reading temperature from BMP280\n");
    }

    adc_T = (sint32) data;

    // assume retrieving other values without errors
    dig_T1 = (uint16) BMP280_read_dig(address, BMP280_REG_DIG_T1_LSB);
    dig_T2 = (sint16) BMP280_read_dig(address, BMP280_REG_DIG_T2_LSB);
    dig_T3 = (sint16) BMP280_read_dig(address, BMP280_REG_DIG_T3_LSB);

    os_printf("dig_T!: %u\n", dig_T1);
    os_printf("dig_T2: %d\n", dig_T2);
    os_printf("dig_T3: %d\n", dig_T3);

    var1 = ((((adc_T>>3) - ((sint32)dig_T1<<1))) * ((sint32)dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((sint32)dig_T1)) * ((adc_T>>4) - ((sint32)dig_T1))) >> 12) * ((sint32)dig_T3)) >> 14;
    return ((var1 + var2) * 5 + 128) >> 8;
}
