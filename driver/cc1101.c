#include "driver/cc1101.h"

//=========================================================================
// Anschlussbelegung des CC1101 ISM Transceivers (SPI1)
//=========================================================================
//  GDO0            PC4
//  GDO2            PC5
//  SPI-CS          PB12    SS1
//  SPI-CLK         PA5     SCK1
//  SPI-MISO        PA6     MISO1 GDO1
//  SPI-MOSI        PA7     MOSI1
//=========================================================================

//=============================================================================
// Rx Tx Buffer
//=============================================================================
volatile  CC1101_Rx RxCC1101;
volatile  CC1101_Tx TxCC1101;
//=============================================================================
// Knoten ID und Funkkanal
//=============================================================================
uint8 ID = 255;
uint8 channel = 0;


//=============================================================================
// PATABLE sollte überarbeitet werden
//=============================================================================
uint8 paTableIndex = PATABLE;   // Current PATABLE Index
uint8 paTable[] = {
        0x00,        // -52 dBm  0
        0x23,        // -15 dBm  1
        0x33,        // -12 dBm  2
        0x34,        // -10 dBm  3
        0x28,        // -8 dBm   4
        0x2B,        // -6 dBm   5
        0x57,        // -4 dBm   6
        0x54,        // -2 dBm   7
        0x3F,        // 0 dBm    8
        0x8C,        // +2 dBm   9
        0x8A,        // +3 dBm   10
        0x87,        // +4 dBm   11
        0x84,        // +5 dBm   12
        0xCE,        // +6 dBm   13
        0xCC,        // +7 dBm   14
        0xC9,        // +8 dBm   15
        0xC6,        // +9 dBm   16
        0xC3         // +10 dBm  17
};
//=============================================================================


//=============================================================================
// Function switchchannel - 11 fest definierte Kanalfrequenzen
//=============================================================================

uint8 FREQ2[] = {0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21};
uint8 FREQ1[] = {0x71,0x6E,0x6B,0x68,0x65,0x41,0x3F,0x3C,0x39,0x36,0x33};
uint8 FREQ0[] = {0x7A,0x85,0x91,0x9D,0xA9,0xF8,0x03,0x0F,0x1B,0x27,0x33};

//Index       frequency     Channel
//-------------------------------------------
//channel=0,  f=869.525MHz  Emergency Channel
//channel=1,  f=869.225MHz  Channel 1
//channel=2,  f=868.925MHz  Channel 2
//channel=3,  f=868.625MHz  Channel 3
//channel=4,  f=868.325MHz  Channel 4
//channel=5,  f=864.700MHz  Channel 5
//channel=6,  f=864.400MHz  Channel 6
//channel=7,  f=864.100MHz  Channel 7
//channel=8,  f=863.800MHz  Channel 8
//channel=9,  f=863.500MHz  Channel 9
//channel=10, f=863.200MHz  Channel 10


//=============================================================================
// 400 KBit/s, 869.525 MHz, MSK, Quartz: 26 MHz, Addresse=255
//=============================================================================
char conf[] = {
        0x06, // IOCFG2   GDO2 Signal Konfigurierung Table 34 Packet,CRC,FIFO
        0x2E, // IOCFG1   GDO1 Signal Konfigurierung Table 34 Tristate
        0x0E, // IOCFG0   GDO0 Signal Konfigurierung Table 34 Carrier Sense
        0x0F, // FIFOTHR  Bytes in FIFO
        0x9B, // SYNC1    Sync Word HighByte 0x9B
        0xAD, // SYNC0    Sync Word Low Byte 0xAD
        0x3F, // PKTLEN   Packetlänge = 63 bei variabler Packetlänge
        0x06, // PKTCTRL1 2 Status Bytes anfügen; Adresscheck ON Broadcastadresse 0
        0x45, // PKTCTRL0 variable Packetlänge ON; whitening ON,
        0xFF, // ADDR     Addresse für Packetfilterung (Knotenadresse)
        0x00, // CHANNR
        0x0B, // FSCTRL1
        0x00, // FSCTRL0
        0x21, // FREQ2    Frequenz Control Word High Byte
        0x71, // FREQ1    Frequenz Control Word Middle Byte
        0x7A, // FREQ0    Frequenz Control Word Low Byte
        0x2D, // MDMCFG4
        0xF8, // MDMCFG3
        0x73, // MDMCFG2  Modulationsformat MSK
        0x42, // MDMCFG1  8 Präambel Bytes,
        0xF8, // MDMCFG0
        0x00, // DEVIATN
        0x07, // MCSM2    (RX_TIME = until end of packet)
        0x03, // MCSM1    (TX_OFFMODE = RX)
        0x18, // MCSM0    (autom. Frequenzkalibrierung bei IDLE->Tx und IDLE->Rx ON)
        0x1D, // FOCCFG
        0x1C, // BSCFG
        0xC0, // AGCCTRL2 AGC Control Gain
        0x49, // AGCCTRL1 AGC Control Carrier Sense Level
        0xB2, // AGCCTRL0 AGC Control
        0x87, // WOREVT1
        0x6B, // WOREVT0
        0xF8, // WORCTRL  WOR Control
        0xB6, // FREND1   Front End Rx
        0x10, // FREND0   Front End TX - PA Power settings
        0xEA, // FSCAL3
        0x2A, // FSCAL2
        0x00, // FSCAL1
        0x1F  // FSCAL0
};

void spiInitTrx()
{
    PIN_FUNC_SELECT(HSPI_MISO_MUX, FUNC_GPIO_HSPI_MISO);  // MISO as GPIO
    PIN_FUNC_SELECT(HSPI_CS_MUX, FUNC_GPIO_HSPI_CS);
    HSPI_MISO_GPIO_IN;                                    // MISO as Input from GDO1
    HSPI_CS_GPIO_LOW;

    while (HSPI_MISO_GPIO_READ) { ; }                     // Wait until CC1101 is ready

    PIN_FUNC_SELECT(HSPI_MISO_MUX, FUNC_HSPI);            // MOSI as HSPI port again
    PIN_FUNC_SELECT(HSPI_CS_MUX, FUNC_HSPI);
}

void spiWriteReg(unsigned char addr, unsigned char value)
{
    spiInitTrx();

    HSPI_transaction(0,0,8,addr,8,value,0,0);

    // HSPI_CS_GPIO_HIGH;
}

unsigned char spiReadReg(unsigned char addr)
{
    unsigned char x;            // Variable Rückgabewert
    spiInitTrx();               // Init SPI CS = 0 warten bis bereit

    // TODO Maybe use HSPI_trx8(...) instead
    x = HSPI_transaction(0,0,8,addr | CC1101_READ_SINGLE,0,0,8,0);
    // HSPI_trx8(addr | CC1101_READ_SINGLE); // Kommando schreiben
    // x = HSPI_rx8();             // Wert lesen

    // HSPI_CS_GPIO_HIGH;

    return x;
}

// TODO Maybe whole function is not needed
void spiStrobe(unsigned char strobe)
{
    spiInitTrx();               // Init SPI CS = 0 warten bis bereit
    HSPI_transaction(0,0,8,strobe,0,0,0,0);
    // HSPI_tx8(strobe);

    // HSPI_CS_GPIO_HIGH;
}

void spiWriteBurstReg(unsigned char addr, char *buffer, unsigned char count)
{
    unsigned char i;
    spiInitTrx();               // Init SPI CS = 0 warten bis bereit

    // HSPI_tx8(addr | CC1101_WRITE_BURST);

    for (i = 0; i < count; i++) {
        // HSPI_tx8(buffer[i]);
        HSPI_transaction(0,0,8,((addr+i) | CC1101_WRITE_SINGLE),8,buffer[i],0,0);
    }

    // HSPI_CS_GPIO_HIGH;
}

void spiReadBurstReg(unsigned char addr, char *buffer, unsigned char count)
{
    unsigned char i;
    spiInitTrx();               // Init SPI CS = 0 warten bis bereit

    // HSPI_tx8(addr | CC1101_READ_BURST);

    for (i = 0; i < count; i++) {
        // buffer[i] = HSPI_rx8();
        buffer[i] = HSPI_transaction(0,0,8,((addr+i) | CC1101_READ_SINGLE),0,0,8,0);
    }

    // HSPI_CS_GPIO_HIGH;
}

unsigned char spiReadStatus(unsigned char addr)
{
    unsigned char x;    // Variable
    spiInitTrx();       // Init SPI CS = 0 warten bis bereit
    HSPI_tx8(addr | CC1101_READ_BURST);
    x = HSPI_rx8();

    HSPI_CS_GPIO_HIGH;

    return x;           // Chip Status Byte - Tabelle 17 im CC1101 Data Sheet
}

void CC1101_reset()
{
    spiInitTrx();               // Init SPI CS = 0 warten bis bereit
    // HSPI_tx8(CC1101_SRES);       // Strobe Kommando Reset
    HSPI_transaction(0,0,8,CC1101_SRES,0,0,0,0);
    spiInitTrx();               // Init SPI CS = 0 warten bis bereit

    // HSPI_CS_GPIO_HIGH;
}

void powerUpReset()
{
    PIN_FUNC_SELECT(HSPI_CS_MUX, FUNC_GPIO_HSPI_CS);
    HSPI_CS_GPIO_HIGH;
    os_delay_us(1);

    HSPI_CS_GPIO_LOW;
    os_delay_us(1);

    HSPI_CS_GPIO_HIGH;
    os_delay_us(45);

    PIN_FUNC_SELECT(HSPI_CS_MUX, FUNC_HSPI);

    CC1101_reset();
}

void CC1101_set_channel(uint8 channel)
{
    if (channel > 10) {
        return;
    }

    spiStrobe(CC1101_SIDLE);

    spiWriteReg(CC1101_FREQ2, (uint8) FREQ2[channel]);
    spiWriteReg(CC1101_FREQ1, (uint8) FREQ1[channel]);
    spiWriteReg(CC1101_FREQ0, (uint8) FREQ0[channel]);

    spiStrobe(CC1101_SRX);
    spiStrobe(CC1101_SCAL);
}

//=============================================================================
// Funktion initalisiert CC1101 und setzt CC1101 in den RX Mode
//=============================================================================
void CC1101_init()
{
    CC1101_init_spi();
    //unsigned int i_enable = 1;
    memset((void *) TxCC1101.data, 0, CC1101_MAX_DATA_LENGTH);
    memset((void *) RxCC1101.data, 0, CC1101_MAX_DATA_LENGTH);
    // Power up Reset CC1101
    powerUpReset();
    // Konfigurationsregister schreiben
    spiWriteBurstReg(0x00, conf, sizeof(conf));
    // aktuelle Sendeleistung des CC1101 setzen
    spiWriteReg(CC1101_PATABLE, paTable[paTableIndex]);
    // Initialisieren der RSSI und CRC Werte im RxCC1101 Empfangspuffer
    RxCC1101.RSSI = 0x00;
    RxCC1101.CRC_RX = false;
    // CC1101 GDO2 Interrupt auf PC5 erlauben
    CC1101_init_interrupt();
    // in den RX Mode schalten
    spiStrobe(CC1101_SRX);
    // warten
    os_delay_us(120); // 120uSek
}

void CC1101_init_spi()
{
    // Init all HSPI Pins
    HSPI_init();
    HSPI_set_clock(4, 2);

    // Reset CS to GPIO in order to set it manually
    // PIN_FUNC_SELECT(HSPI_CS_MUX, FUNC_GPIO_HSPI_CS);
    // HSPI_CS_GPIO_HIGH;
}

void CC1101_init_interrupt()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5); // Pin GPIO 5 as GPIO
    PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO5_U);              // Disable internal pull up
    GPIO_DIS_OUTPUT(5);                                  // GPIO 5 as Input

    ETS_GPIO_INTR_DISABLE();                             // don't get disturbed by interrupt

    ETS_GPIO_INTR_ATTACH(gpio_intr_handler, &var_intr);

    //    void gpio_register_set(uint32 reg_id, uint32 value);
    //
    // From include file
    //   Set the specified GPIO register to the specified value.
    //   This is a very general and powerful interface that is not
    //   expected to be used during normal operation.  It is intended
    //   mainly for debug, or for unusual requirements.
    //
    // All people repeat this mantra but I don't know what it means
    //
    gpio_register_set(GPIO_PIN_ADDR(5),
            GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE) |
            GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE) |
            GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(5));

    // enable interrupt for his GPIO
    //     GPIO_PIN_INTR_... defined in gpio.h

    gpio_pin_intr_state_set(GPIO_ID_PIN(5), GPIO_PIN_INTR_NEGEDGE);

    ETS_GPIO_INTR_ENABLE();
}

void CC1101_init_idle()
{
    // Power up Reset CC1101
    powerUpReset();
    // Konfigurationsregister schreiben
    spiWriteBurstReg(0x00, conf, sizeof(conf));
    // aktuelle Sendeleistung des CC1101 setzen
    spiWriteReg(CC1101_PATABLE, paTable[paTableIndex]);
    // Initialisieren der RSSI und CRC Werte im RxCC1101 Empfangspuffer
    RxCC1101.RSSI = 0x00;
    RxCC1101.CRC_RX = false;
    spiStrobe(CC1101_SIDLE);    // in den IDLE Mode setzen
    os_delay_us(120); // 120uSek
}

void CC1101_init_powerdown()
{
    spiStrobe(CC1101_SPWD);     // in den PowerDown Mode setzen
    os_delay_us(120);
}

void CC1101_isr()
{
    char crc;
    crc = receive_Packet();
    if (crc) {
//        EXTI_ClearITPendingBit(EXTI_Line5);
//        EXTI_ClearFlag(EXTI_Line5);
        print_Packet();
    }

    spiStrobe(CC1101_SIDLE); // Switch to IDLE
    spiStrobe(CC1101_SFRX);  // Flush the RX FIFO
    spiStrobe(CC1101_SRX);   // Rx Mode



    uint32 gpio_status;

    gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

    //clear interrupt status

    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
//    EXTI_ClearITPendingBit(EXTI_Line5);
//    EXTI_ClearFlag(EXTI_Line5);
}

void CC1101_set_id(uint8 id)
{
    if (id > MAX_UID || id < MIN_UID) {
        return;
    }

    spiStrobe(CC1101_SIDLE);                  // in den IDLE Mode setzen
    spiWriteReg(CC1101_ADDR, id);             // Adressregister schreiben
    spiStrobe(CC1101_SRX);                    // in den RX Mode schalten
    os_delay_us(120);
}

uint8 CC1101_set_power(uint8 pa_index)
{
    if (pa_index < 18) {
        spiStrobe(CC1101_SIDLE);    // CC1101 in den IDLE Mode setzen
        paTableIndex = pa_index;       // PA Index setzen
        spiWriteReg(CC1101_PATABLE, paTable[paTableIndex]); // PA Wert schreiben
        spiStrobe(CC1101_SRX);      // CC1101 in den RX Mode setzen
        os_delay_us(120);
        return true;
    }

    return false;
}

void send_Packet(unsigned char ziel,unsigned char quelle,  char *data, unsigned char length)
{
    //  -----------------------------
    //  | 0 | 1 | 2 | 3  |....| 62  |  Datenpaket aus 63 Byte
    //  -----------------------------
    //  | L | Z | Q | D1 |....| D59 |
    //    L--------------------------- Länge
    //        Z----------------------- Ziel
    //            Q------------------- Quelle
    //                D1........D59--- 59 Datenbytes

    unsigned char i;

    if (length > CC1101_MAX_DATA_LENGTH-1 ) { // max 59 Byte
        length = CC1101_MAX_DATA_LENGTH-1 ;   // zu große Packete werden auf 59 Byte begrenzt
    }

    // Packetlänge = 1 Byte (Zieladdresse) + data length
    TxCC1101.length = 2 + length;
    // Zieladresse eintragen
    TxCC1101.dest = ziel;
    TxCC1101.source = quelle;

    // Quelladresse eintragen
    //Sendepuffer füllen
    for (i = 0; i < length; i++) {
        TxCC1101.data[i] = data[i];
    }

    /*
    // Interrupt auf GDO2 verbieten
    EXTI_ClearITPendingBit(EXTI_Line5);
    EXTI_ClearFlag(EXTI_Line5);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
    */

    // setzt CC1101 in den IDLE Mode
    spiStrobe(CC1101_SIDLE);
    // löscht den TX FIFO des CC1101
    spiStrobe(CC1101_SFTX);
    // Packet in TX FIFO schreiben +Länge+Ziel+Quelle
    spiWriteBurstReg(CC1101_TXFIFO, (char *) &TxCC1101, length+3);
    // setzt CC1101 in den Tx Mode
    spiStrobe(CC1101_STX);

    // warten GDO2=1 - sync transmitted
    // while (!CC1101_GDO2);
    // warten GDO2=0 - Packet Ende
    // while (CC1101_GDO2);

    /*
    // Interrupt auf GDO2 erlauben
    EXTI_ClearITPendingBit(EXTI_Line5);
    EXTI_ClearFlag(EXTI_Line5);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    */
}

char receive_Packet()
{
    // für RSSI und CRC Status vorbereiten
    char status[2];
    // mit 0 initialisieren um aktuelle Länge zu speichern
    unsigned char packetLength = 0;
    // Wenn Bytes im RX FIFO vorhanden sind dann...
    if ((spiReadStatus(CC1101_RXBYTES) & BYTES_IN_RXFIFO)) {
        // Längenbyte des aktuellen Packetes aus dem RX FIFO lesen (erstes Byte)
        packetLength = spiReadReg(CC1101_RXFIFO); //Das erste Byte ist LängenByte

        // Wenn Packetlänge OK dann...
        if (packetLength <= PACKET_LENGTH) {
            // Längenbyte in den RxCC1101 Puffer schreiben
            RxCC1101.length = packetLength;// packetLength;
            // Den Rest des Packetes in RxCC1101 mit aktueller Länge schreiben
            spiReadBurstReg(CC1101_RXFIFO,(char *)RxCC1101.data, packetLength);
            // Lesen der zwei Status Bytes (status[0] = RSSI, status[1] = LQI)
            spiReadBurstReg(CC1101_RXFIFO, status, 2);
            // RSSI Werte in den RxCC1101 Puffer schreiben
            RxCC1101.RSSI = status[I_RSSI];
            // CRC Wert in den RxCC1101 Puffer schreiben
            RxCC1101.CRC_RX = (status[I_LQI] & CRC_OK) >> 7;
            // Zieladresse in den RxCC1101 Puffer schreiben
            RxCC1101.dest = RxCC1101.data[0];
            // Quelladresse in den RxCC1101 Puffer schreiben
            RxCC1101.source = RxCC1101.data[1];
            // Rückgabewert CRC true or false
            return RxCC1101.CRC_RX;

        } else {
            // ...CC1101 in den IDLE Mode setzen um...
            spiStrobe(CC1101_SIDLE);
            // ...den RX FIFO zu löschen...
            spiStrobe(CC1101_SFRX);
        }
    }

    return false;
}

void print_Packet()
{
    unsigned int i;
    char daten[65];

    os_printf("\nQuelladresse= %u", RxCC1101.source);
    os_printf("\nZieladresse = %u", RxCC1101.dest);
    os_printf("\nRSSI = %u", RxCC1101.RSSI);
    os_printf("\nPaketlänge = %u", RxCC1101.length);
    os_printf("\n");
    os_printf("\nDatenpaket = ");

    for (i=0;i<((RxCC1101.length-2));i++) {
        daten[i] = RxCC1101.data[2+i];
    }

    daten[i] = '0';

    os_printf(daten);
    os_printf("\n");
}
