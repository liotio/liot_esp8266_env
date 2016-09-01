#include "driver/soft_i2c.h"

/*
 * Based on:
 *
 * http://www.robot-electronics.co.uk/i2c-tutorial
 *
 */

void I2C_init()
{
    // avoid being interrupted
    ETS_GPIO_INTR_DISABLE();

    // use pins as gpio
    PIN_FUNC_SELECT(I2C_SDA_MUX, I2C_SDA_FUNC);
    PIN_FUNC_SELECT(I2C_SCL_MUX, I2C_SCL_FUNC);

    // configure as open drain
    GPIO_REG_WRITE(GPIO_PIN_ADDR(I2C_SDA_PIN), GPIO_REG_READ(GPIO_PIN_ADDR(I2C_SDA_PIN)) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE));
    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_SDA_PIN));
    GPIO_REG_WRITE(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SCK_PIN)), GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SCK_PIN))) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE));
    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_SCK_PIN));

    ETS_GPIO_INTR_ENABLE();
}

uint8 I2C_restart()
{
    // generate start sequence
    I2C_DELAY;
    I2C_SDA_HIGH;
    I2C_DELAY;
    I2C_SCK_HIGH;
    I2C_DELAY;

    I2C_SDA_LOW;
    I2C_DELAY;

    I2C_SCK_LOW;
    I2C_DELAY;
}

uint8 I2C_start(
    uint8 address,
    uint8 readwrite)
{
    uint8 data;

    // set gpio's to open drain
    // I2C_gpio_init();

    // generate start sequence
    I2C_restart();

    // shift address one position to left, take
    // readwrite bit to least significant bit and
    // write this data out to the slave device(s)
    data = (address << 1) | (readwrite & 1);
    return I2C_write(data);
}

void I2C_stop()
{
    //TODO Remove next 2?
    I2C_SDA_LOW;
    I2C_SCK_LOW;
    I2C_DELAY;

    I2C_SCK_HIGH;
    I2C_DELAY;

    I2C_SDA_HIGH;
    I2C_DELAY;
}

void I2C_await_clk_strech()
{
    // wait while slave pulls low clock line (clock streching)
    I2C_SCK_IN;
    I2C_DELAY;
    while (I2C_SCK_READ == 0) {
        I2C_DELAY;
    }
}

uint8 I2C_await_ack()
{
    uint8 ack, sck;
    uint32 timeout;

    #ifdef DEBUG
    os_printf("\r\n");
    #endif

    // read ack value

    // sck in should behave same as sck high (external pull-up)
    // but sck in can be used for clock stretching
    I2C_SDA_IN;
    I2C_DELAY;

    I2C_await_clk_strech();

    timeout = I2C_TIMEOUT;
    while (timeout) {
        ack = I2C_SDA_READ;
        if (ack == GPIO_LOW) {
            break;
        }
        #ifdef DEBUG
        os_printf("Wait for I2C ack\n");
        #endif
        I2C_DELAY;
        timeout--;
    }

    #ifdef DEBUG
    os_printf("\r\n");
    #endif

    I2C_SCK_LOW;
    I2C_DELAY;

    return ack;
}

uint8 I2C_write(
    uint8 data)
{
    uint8 i;
    uint8 ack;

    #ifdef DEBUG
    os_printf("\r\n");
    #endif

    // shift out the data bits starting with
    // the most significant bit
    for (i = 8; i > 0; i--) {
        if ((data >> (i-1)) & 1) {
            I2C_SDA_HIGH;
            #ifdef DEBUG
            os_printf("1");
            #endif
        } else {
            I2C_SDA_LOW;
            #ifdef DEBUG
            os_printf("0");
            #endif
        }

        I2C_SCK_HIGH;
        I2C_DELAY;

        I2C_SCK_LOW;
        I2C_DELAY;
    }

    ack = I2C_await_ack();

    #ifdef DEBUG
    os_printf("\r\n");
    #endif

    I2C_DELAY;

    if (ack) {
        I2C_stop();
    }

    return ack;
}

uint8 I2C_write_single(
        uint8 slave_addr,
        uint8 reg_addr,
        uint8 data)
{
    if (I2C_start(slave_addr, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to slave");
        return -1;
    }
    if (I2C_write(reg_addr)) {
        os_printf("\nCannot write register to slave");
        return -1;
    }
    if (I2C_write(data)) {
        os_printf("\nCannot write data to slave");
        return -1;
    }

    return 0;
}

uint8 I2C_write_buffer(
    uint8 slave_addr,
    uint8 reg_addr,
    uint8 length,
    uint8 const *data)
{
    if (I2C_start(slave_addr, I2C_SLAVE_WRITE)) {
        return 1;
    }
    if (I2C_write(reg_addr)) {
        return 1;
    }
    while (length--) {
        I2C_write(*data++);
    }

    I2C_stop();
    return 0;
}

uint8 I2C_read(uint8 ack)
{
    uint8 i;
    uint8 data = 0;

    #ifdef DEBUG
    os_printf("\r\n");
    #endif

    // I2C_SDA_HIGH;
    // I2C_SCK_HIGH;

    I2C_SDA_IN;
    I2C_await_clk_strech();

    for (i = 8; i > 0; i--) {
        I2C_SCK_HIGH;
        I2C_DELAY;

        data = data | (I2C_SDA_READ << (i-1));
        #ifdef DEBUG
        os_printf("%u", I2C_SDA_READ);
        #endif

        I2C_SCK_LOW;
        I2C_DELAY;
    }

    // send (n)ack

    if (ack == I2C_SEND_ACK) {
        I2C_SDA_LOW;
    } else {
        I2C_SDA_HIGH;
    }

    I2C_SCK_HIGH;
    I2C_DELAY;

    I2C_SCK_LOW;
    // TODO useless?
    // I2C_SDA_HIGH;
    I2C_DELAY;

    #ifdef DEBUG
    os_printf("\r\n");
    #endif

    return data;
}

uint8 I2C_read_ack()
{
    return I2C_read(I2C_SEND_ACK);
}

uint8 I2C_read_nack()
{
    return I2C_read(I2C_SEND_NACK);
}

uint8 I2C_read_single(
        uint8 slave_addr,
        uint8 reg_addr)
{
    return (uint8) I2C_read_multiple_msb(slave_addr, reg_addr, 8);
}

uint64 I2C_read_multiple_msb(
        uint8 slave_addr,
        uint8 reg_addr,
        uint8 bits)
{
    uint64 result = 0;

    if (I2C_start(slave_addr, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to slave");
        return -1;
    }
    if (I2C_write(reg_addr)) {
        os_printf("\nCannot write register to slave");
        return -1;
    }

    if (I2C_start(slave_addr, I2C_SLAVE_READ)) {
        os_printf("\nCannot init reading from slave");
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

uint64 I2C_read_multiple_lsb(
        uint8 slave_addr,
        uint8 reg_addr,
        uint8 bits)
{
    uint64 result = 0;
    uint8 offset = 0;

    if (I2C_start(slave_addr, I2C_SLAVE_WRITE)) {
        os_printf("\nCannot init writing to slave");
        return -1;
    }
    if (I2C_write(reg_addr)) {
        os_printf("\nCannot write register to slave");
        return -1;
    }

    if (I2C_start(slave_addr, I2C_SLAVE_READ)) {
        os_printf("\nCannot init reading from slave");
        return -1;
    }

    while (bits > 8) {
        result = result | I2C_read_ack() << offset;
        bits -= 8;
        offset += 8;
    }

    result = result | (I2C_read_nack() << offset);

    I2C_stop();

    return result;
}

uint8 I2C_read_buffer(
    uint8 slave_addr,
    uint8 reg_addr,
    uint8 length,
    uint8 *data)
{
    if (I2C_start(slave_addr, I2C_SLAVE_WRITE)) {
        return 1;
    }
    if (I2C_write(reg_addr)) {
        return 1;
    }
    if (I2C_start(slave_addr, I2C_SLAVE_READ)) {
        return 1;
    }
    while ((length--) > 1)
    {
        *data = I2C_read_ack();
        data++;
    }
    *data = I2C_read_nack();

    I2C_stop();
    return 0;
}
