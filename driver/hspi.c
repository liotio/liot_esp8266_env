#include "driver/hspi.h"

void HSPI_init()
{
    HSPI_init_gpio(HSPI_CLK_USE_DIV);
    HSPI_set_clock(HSPI_CLK_PREDIV, HSPI_CLK_CNTDIV);
    HSPI_set_tx_byte_order(HSPI_MSB_FIRST);
    HSPI_set_tx_byte_order(HSPI_MSB_FIRST);

    SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_CS_SETUP | SPI_CS_HOLD);
    CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_FLASH_MODE);
}

void HSPI_init_gpio(
        uint8 sysclk_as_spiclk)
{
    uint32 clock_div_flag = 0;
    if (sysclk_as_spiclk) {
        clock_div_flag = 0x0001;
    }

    //Set bit 9 if 80MHz sysclock required
    WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105 | (clock_div_flag << 9));

    PIN_FUNC_SELECT(HSPI_MISO_MUX, FUNC_HSPI);
    PIN_FUNC_SELECT(HSPI_MOSI_MUX, FUNC_HSPI);
    PIN_FUNC_SELECT(HSPI_CLK_MUX, FUNC_HSPI);
    PIN_FUNC_SELECT(HSPI_CS_MUX, FUNC_HSPI);
}

void HSPI_set_mode(
        uint8 spi_cpha,
        uint8 spi_cpol)
{
    if (spi_cpha) {
        CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_CK_OUT_EDGE);
    } else {
        SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_CK_OUT_EDGE);
    }

    if (spi_cpol) {
        SET_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);
    } else {
        CLEAR_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);
    }
}


void HSPI_set_clock(
        uint16 prediv,
        uint8 cntdiv)
{
    if ((prediv==0) | (cntdiv==0)) {
        WRITE_PERI_REG(SPI_CLOCK(HSPI), SPI_CLK_EQU_SYSCLK);
    } else {
        WRITE_PERI_REG(SPI_CLOCK(HSPI),
                (((prediv-1)&SPI_CLKDIV_PRE)<<SPI_CLKDIV_PRE_S)|
                (((cntdiv-1)&SPI_CLKCNT_N)<<SPI_CLKCNT_N_S)|
                (((cntdiv>>1)&SPI_CLKCNT_H)<<SPI_CLKCNT_H_S)|
                ((0&SPI_CLKCNT_L)<<SPI_CLKCNT_L_S));
    }
}

void HSPI_set_tx_byte_order(
        uint8 byte_order)
{
    if (byte_order) {
        SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_WR_BYTE_ORDER);
    } else {
        CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_WR_BYTE_ORDER);
    }
}

void HSPI_set_rx_byte_order(
        uint8 byte_order)
{
    if (byte_order){
        SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_RD_BYTE_ORDER);
    } else {
        CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_RD_BYTE_ORDER);
    }
}

/*
void spi_mast_byte_write(
        uint8 data)
{
    uint32 regvalue;

    while (HSPI_IS_BUSY);
    CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_MOSI | SPI_USR_MISO);

    //SPI_FLASH_USER2 bit28-31 is cmd length,cmd bit length is value(0-15)+1,
    // bit15-0 is cmd value.
    WRITE_PERI_REG(SPI_USER2(HSPI), ((7&SPI_USR_COMMAND_BITLEN)<<SPI_USR_COMMAND_BITLEN_S)|((uint32)data));
    SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);
    while (HSPI_IS_BUSY);
}
*/

uint32 HSPI_transaction(
        uint8 cmd_bits,
        uint16 cmd_data,
        uint32 addr_bits,
        uint32 addr_data,
        uint32 dout_bits,
        uint32 dout_data,
        uint32 din_bits,
        uint32 dummy_bits)
{

    while(HSPI_IS_BUSY); //wait for SPI to be ready

    //
    CLEAR_PERI_REG_MASK(SPI_USER(HSPI),
            SPI_USR_MOSI | SPI_USR_MISO | SPI_USR_COMMAND | SPI_USR_ADDR | SPI_USR_DUMMY);

    //enable functions based on number of bits. 0 bits = disabled.
    //This is rather inefficient but allows for a very generic function.
    //CMD ADDR and MOSI are set below to save on an extra if statement.
    if(din_bits) SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_MISO);
    if(dummy_bits) SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_DUMMY);

    WRITE_PERI_REG(SPI_USER1(HSPI),
            ((addr_bits-1)&SPI_USR_ADDR_BITLEN)<<SPI_USR_ADDR_BITLEN_S | //Number of bits in Address
            ((dout_bits-1)&SPI_USR_MOSI_BITLEN)<<SPI_USR_MOSI_BITLEN_S | //Number of bits to Send
            ((din_bits-1)&SPI_USR_MISO_BITLEN)<<SPI_USR_MISO_BITLEN_S |  //Number of bits to receive
            ((dummy_bits-1)&SPI_USR_DUMMY_CYCLELEN)<<SPI_USR_DUMMY_CYCLELEN_S); //Number of Dummy bits to insert

    if(cmd_bits) {
        SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_COMMAND); //enable COMMAND function in SPI module
        uint16 command = cmd_data << (16-cmd_bits); //align command data to high bits
        command = ((command>>8)&0xff) | ((command<<8)&0xff00); //swap byte order
        WRITE_PERI_REG(SPI_USER2(HSPI), ((((cmd_bits-1)&SPI_USR_COMMAND_BITLEN)<<SPI_USR_COMMAND_BITLEN_S) | command&SPI_USR_COMMAND_VALUE));
    }

    if(addr_bits){
        SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_ADDR); //enable ADDRess function in SPI module
        WRITE_PERI_REG(SPI_ADDR(HSPI), addr_data<<(32-addr_bits)); //align address data to high bits
    }

    if(dout_bits) {
        SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_MOSI); //enable MOSI function in SPI module
        //copy data to W0
        if (READ_PERI_REG(SPI_USER(HSPI)) & SPI_WR_BYTE_ORDER) {
            WRITE_PERI_REG(SPI_W0(HSPI), dout_data<<(32-dout_bits));
        } else {

            uint8 dout_extra_bits = dout_bits%8;

            if(dout_extra_bits){
                //if your data isn't a byte multiple (8/16/24/32 bits)and you don't have SPI_WR_BYTE_ORDER set, you need this to move the non-8bit remainder to the MSBs
                //not sure if there's even a use case for this, but it's here if you need it...
                //for example, 0xDA4 12 bits without SPI_WR_BYTE_ORDER would usually be output as if it were 0x0DA4,
                //of which 0xA4, and then 0x0 would be shifted out (first 8 bits of low byte, then 4 MSB bits of high byte - ie reverse byte order).
                //The code below shifts it out as 0xA4 followed by 0xD as you might require.
                WRITE_PERI_REG(SPI_W0(HSPI), ((0xFFFFFFFF<<(dout_bits - dout_extra_bits)&dout_data)<<(8-dout_extra_bits) | (0xFFFFFFFF>>(32-(dout_bits - dout_extra_bits)))&dout_data));
            } else {
                WRITE_PERI_REG(SPI_W0(HSPI), dout_data);
            }
        }
    }

    // begin HSPI transaction
    SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);

    // return data in
    if(din_bits) {
        while(HSPI_IS_BUSY);    //wait for SPI transaction to complete

        if(READ_PERI_REG(SPI_USER(HSPI))&SPI_RD_BYTE_ORDER) {
            return READ_PERI_REG(SPI_W0(HSPI)) >> (32-din_bits); //Assuming data in is written to MSB. TBC
        } else {
            return READ_PERI_REG(SPI_W0(HSPI)); //Read in the same way as DOUT is sent. Note existing contents of SPI_W0 remain unless overwritten!
        }

        return 0; //something went wrong
    }

    return 1;
}
