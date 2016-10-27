#ifndef CC1101_H
#define CC1101_H

#include "c_types.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "gpio.h"
#include "osapi.h"

#include "driver/hspi.h"

#include "user/interrupt.h"

//=============================================================================
// zentrale Daten für den CC1101 Funkchip
//=============================================================================
// RxCC1101     Empfangsbuffer
// TxCC1101     Sendebuffer
// ID           Knoten Adresse
// channel      Funkkanal
// conf[39]     Konfigurationstabelle
//=============================================================================
extern uint8_t ID;            //Node ID von 1...255
extern uint8_t channel;       //Funkkanal von 1..10
extern uint8_t conf[39];               //Konfigurationstabelle
extern uint8_t paTableIndex;  //PA Tabelle


#define MAX_UID                   0xFF  // Maximum UID of a node is 255
#define MIN_UID                   0x01  // Minimum UID of a node is 1
#define BROADCAST_UID             0x00  // UID for broadcasts
#define MIN_CHANNEL               1     // Minimum channel number, set to 0 to activate Notfallkanal
#define MAX_CHANNEL               10    // Maximum channel number

#define CC1101_GDO0               GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) // read GDO0
#define CC1101_GDO1               GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) // read GDO1
#define CC1101_GDO2               GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) // read GDO2

#define NOBYTE                    0x00  // No command, for reading.
#define PACKET_LENGTH             255   // Packet length = 62 Bytes.
#define HEADER_SIZE               2     //size of header
#define CC1101_MAX_DATA_LENGTH    (PACKET_LENGTH - HEADER_SIZE)
//=============================================================================
#define PATABLE                   17    // Current PATABLE Index
#define LOW_POWER                 1     // Sendeleistung auf -15 dBm setzen
#define MED_POWER                 9     // Sendeleistung auf  +2 dBm setzen
#define HIGH_POWER                16    // Sendeleistung auf  +9 dBm setzen
//=============================================================================
// Bitmasks for reading out status register values
//=============================================================================
#define CRC_OK              (0x80)  // Bitmask (=10000000) for reading CRC_OK.
                                    // If CRC_OK == 1: CRC for received data OK (or CRC disabled).
                                    // If CRC_OK == 0: CRC error in received data.
#define LQI_EST             (0x7F)  // Bitmask (=01111111) for reading LQI_EST.
                                    // The Link Quality Indicator estimates how easily a received signal can be demodulated.
#define I_RSSI              (0x00)  // Index 0 contains RSSI information (from optionally appended packet status bytes).
#define I_LQI               (0x01)  // Index 1 contains LQI & CRC_OK information (from optionally appended packet status bytes).
#define MARC_STATE          (0x1F)  // Bitmask (=00011111) for reading MARC_STATE in MARCSTATE status register.
#define CS                  (0x40)  // Bitmask (=01000000) for reading CS (Carrier Sense) in PKTSTATUS status register.
#define PQT_REACHED         (0x20)  // Bitmask (=00100000) for reading PQT_REACHED (Preamble Quality reached) in PKTSTATUS status register.
#define CCA                 (0x10)  // Bitmask (=00010000) for reading CCA (clear channel assessment) in PKTSTATUS status register.
#define SFD                 (0x08)  // Bitmask (=00001000) for reading SFD (Sync word found) in PKTSTATUS status register.
#define GDO2                (0x04)  // Bitmask (=00000100) for reading GDO2 (current value on GDO2 pin) in PKTSTATUS status register.
#define GDO1                (0x02)  // Bitmask (=00000010) for reading GDO1 (current value on GDO1 pin) in PKTSTATUS status register.
#define GDO0                (0x01)  // Bitmask (=00000001) for reading GDO0 (current value on GDO0 pin) in PKTSTATUS status register.
#define TXFIFO_UNDERFLOW    (0x80)  // Bitmask (=10000000) for reading TXFIFO_UNDERFLOW in TXBYTES status register.
#define BYTES_IN_TXFIFO     (0x7F)  // Bitmask (=01111111) for reading NUM_TXBYTES in TXBYTES status register.
#define RXFIFO_OVERFLOW     (0x80)  // Bitmask (=10000000) for reading RXFIFO_OVERFLOW in RXBYTES status register.
#define BYTES_IN_RXFIFO     (0x7F)  // Bitmask (=01111111) for reading NUM_RXBYTES in RXBYTES status register.
//=============================================================================
// Bitmasks for reading out configuration register values
//=============================================================================
#define PKT_LENGTH_CONFIG   (0x03)  // Bitmask (=00000011) for reading LENGTH_CONFIG in PKTCTRL0 configuration register.
//=============================================================================

//=============================================================================
// Configuration Registers (47x)
//=============================================================================
#define CC1101_IOCFG2   (0x00) // GDO2 output pin configuration
#define CC1101_IOCFG1   (0x01) // GDO1 output pin configuration
#define CC1101_IOCFG0   (0x02) // GDO0 output pin configuration
#define CC1101_FIFOTHR  (0x03) // RX FIFO and TX FIFO thresholds
#define CC1101_SYNC1    (0x04) // Sync word, high byte
#define CC1101_SYNC0    (0x05) // Sync word, low byte
#define CC1101_PKTLEN   (0x06) // Packet length
#define CC1101_PKTCTRL1 (0x07) // Packet automation control
#define CC1101_PKTCTRL0 (0x08) // Packet automation control
#define CC1101_ADDR     (0x09) // Device address
#define CC1101_CHANNR   (0x0A) // Channel number
#define CC1101_FSCTRL1  (0x0B) // Frequency synthesizer control
#define CC1101_FSCTRL0  (0x0C) // Frequency synthesizer control
#define CC1101_FREQ2    (0x0D) // Frequency control word, high byte
#define CC1101_FREQ1    (0x0E) // Frequency control word, middle byte
#define CC1101_FREQ0    (0x0F) // Frequency control word, low byte
#define CC1101_MDMCFG4  (0x10) // Modem configuration
#define CC1101_MDMCFG3  (0x11) // Modem configuration
#define CC1101_MDMCFG2  (0x12) // Modem configuration
#define CC1101_MDMCFG1  (0x13) // Modem configuration
#define CC1101_MDMCFG0  (0x14) // Modem configuration
#define CC1101_DEVIATN  (0x15) // Modem deviation setting
#define CC1101_MCSM2    (0x16) // Main Radio Control State Machine configuration
#define CC1101_MCSM1    (0x17) // Main Radio Control State Machine configuration
#define CC1101_MCSM0    (0x18) // Main Radio Control State Machine configuration
#define CC1101_FOCCFG   (0x19) // Frequency Offset Compensation configuration
#define CC1101_BSCFG    (0x1A) // Bit Synchronization configuration
#define CC1101_AGCCTRL2 (0x1B) // AGC control
#define CC1101_AGCCTRL1 (0x1C) // AGC control
#define CC1101_AGCCTRL0 (0x1D) // AGC control
#define CC1101_WOREVT1  (0x1E) // High byte Event 0 timeout
#define CC1101_WOREVT0  (0x1F) // Low byte Event 0 timeout
#define CC1101_WORCTRL  (0x20) // Wake On Radio control
#define CC1101_FREND1   (0x21) // Front end RX configuration
#define CC1101_FREND0   (0x22) // Front end TX configuration
#define CC1101_FSCAL3   (0x23) // Frequency synthesizer calibration
#define CC1101_FSCAL2   (0x24) // Frequency synthesizer calibration
#define CC1101_FSCAL1   (0x25) // Frequency synthesizer calibration
#define CC1101_FSCAL0   (0x26) // Frequency synthesizer calibration
#define CC1101_RCCTRL1  (0x27) // RC oscillator configuration
#define CC1101_RCCTRL0  (0x28) // RC oscillator configuration
#define CC1101_FSTEST   (0x29) // Frequency synthesizer calibration control
#define CC1101_PTEST    (0x2A) // Production test
#define CC1101_AGCTEST  (0x2B) // AGC test
#define CC1101_TEST2    (0x2C) // Various test settings
#define CC1101_TEST1    (0x2D) // Various test settings
#define CC1101_TEST0    (0x2E) // Various test settings
//=============================================================================

//=============================================================================
// Strobe commands (14x)
//=============================================================================
#define CC1101_SRES    (0x30)  // Reset chip.
#define CC1101_SFSTXON (0x31)  // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                               // If in RX/TX: Go to a wait state where only the synthesizer is
                               // running (for quick RX / TX turnaround).
#define CC1101_SXOFF   (0x32)  // Turn off crystal oscillator.
#define CC1101_SCAL    (0x33)  // Calibrate frequency synthesizer and turn it off
                               // (enables quick start).
#define CC1101_SRX     (0x34)  // Enable RX. Perform calibration first if coming from IDLE and
                               // MCSM0.FS_AUTOCAL=1.
#define CC1101_STX     (0x35)  // In IDLE state: Enable TX. Perform calibration first if
                               // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                               // Only go to TX if channel is clear.
#define CC1101_SIDLE   (0x36)  // Exit RX / TX, turn off frequency synthesizer and exit
                               // Wake-On-Radio mode if applicable.
#define CC1101_SAFC    (0x37)  // Perform AFC adjustment of the frequency synthesizer
#define CC1101_SWOR    (0x38)  // Start automatic RX polling sequence (Wake-on-Radio)
#define CC1101_SPWD    (0x39)  // Enter power down mode when CSn goes high.
#define CC1101_SFRX    (0x3A)  // Flush the RX FIFO buffer (CC1101 should be in IDLE state).
#define CC1101_SFTX    (0x3B)  // Flush the TX FIFO buffer (CC1101 should be in IDLE state).
#define CC1101_SWORRST (0x3C)  // Reset real time clock.
#define CC1101_SNOP    (0x3D)  // No operation. May be used to pad strobe commands to two
                               // bytes for simpler software.
//=============================================================================

//=============================================================================
// Status registers (12x)
//=============================================================================
#define CC1101_PARTNUM      (0x30)  // Part number of CC1101.
#define CC1101_VERSION      (0x31)  // Current version number.
#define CC1101_FREQEST      (0x32)  // Frequency Offset Estimate.
#define CC1101_LQI          (0x33)  // Demodulator estimate for Link Quality.
#define CC1101_RSSI         (0x34)  // Received signal strength indication.
#define CC1101_MARCSTATE    (0x35)  // Control state machine state.
#define CC1101_WORTIME1     (0x36)  // High byte of WOR timer.
#define CC1101_WORTIME0     (0x37)  // Low byte of WOR timer.
#define CC1101_PKTSTATUS    (0x38)  // Current GDOx status and packet status.
#define CC1101_VCO_VC_DAC   (0x39)  // Current setting from PLL calibration module.
#define CC1101_TXBYTES      (0x3A)  // Underflow and number of bytes in the TX FIFO.
#define CC1101_RXBYTES      (0x3B)  // Overflow and number of bytes in the RX FIFO.
//=============================================================================

//=============================================================================
// Multi byte registers
//=============================================================================
#define CC1101_PATABLE      (0x3E)  // Register for eight user selected output power settings.
#define CC1101_TXFIFO       (0x3F)  // TX FIFO: Write operations write to the TX FIFO (SB: +0x00; BURST: +0x40)
#define CC1101_RXFIFO       (0x3F)  // RX FIFO: Read operations read from the RX FIFO (SB: +0x80; BURST: +0xC0)
//=============================================================================

//=============================================================================
// Definitions to support burst/single access
//=============================================================================
#define CC1101_WRITE_SINGLE (0x00)  // Offset for burst write.
#define CC1101_WRITE_BURST  (0x40)  // Offset for burst write.
#define CC1101_READ_SINGLE  (0x80)  // Offset for read single byte.
#define CC1101_READ_BURST   (0xC0)  // Offset for read burst.
//=============================================================================

#define false 0
#define true 1

//=============================================================================
// CC1101 Funk Empfangspuffer RxCC1101
//=============================================================================
typedef struct RCC1101
    {
    uint8_t length;   // Längenbyte muß <= 62 sein
    uint8_t dest;     // Zieladresse des empfangenen Packetes
    uint8_t source;   // Quelladresse des empfangenen Packetes
    uint8_t data[CC1101_MAX_DATA_LENGTH]; // RxCC1101.data[0] Zieladresse
                            // RxCC1101.data[1] Quelladresse
                            // RxCC1101.data[2..59] Daten
    uint8_t RSSI;     // The RSSI value of last received packet
    uint8_t CRC_RX;   // The CRC status of last received packet (1 = OK, 0 = not OK)
    }
    CC1101_Rx;
extern volatile  CC1101_Rx RxCC1101;
//=============================================================================


//=============================================================================
// CC1101 Funk Sendepuffer TxCC1101 für variable Datenlängen
//=============================================================================
typedef struct TCC1101
    {
    uint8_t length;   // Länge Daten  (max 59)
    uint8_t dest;     // Zieladresse  (0..255)
    uint8_t source;   // Quelladresse (0..255)
    uint8_t data[CC1101_MAX_DATA_LENGTH]; // Daten        (max 60)
    }
    CC1101_Tx;
extern volatile  CC1101_Tx TxCC1101;    //
//=============================================================================


void CC1101_test();

void CC1101_init();

void CC1101_poweronreset();

void CC1101_spi_init();

void CC1101_init_idle();

void CC1101_init_powerdown();

void CC1101_init_interrupt();

void CC1101_isr();

void CC1101_reset();

void CC1101_set_id(
        uint8_t id);

uint8_t CC1101_set_power(
        uint8_t pa_index);

void CC1101_set_channel(
        uint8_t channel);

void CC1101_spi_trx_init();

void CC1101_spi_write(
        uint8_t addr,
        uint8_t value);

void CC1101_spi_write_burst(
        uint8_t addr,
        uint8_t *buffer,
        uint8_t count);

uint8_t CC1101_spi_read(
        uint8_t addr);

void CC1101_spi_read_burst(
        uint8_t addr,
        uint8_t *buffer,
        uint8_t count);

void CC1101_spi_strobe(
        uint8_t strobe);

void CC1101_send_packet(
        uint8_t dest,
        uint8_t source,
        uint8_t *data,
        uint8_t length);

uint8_t CC1101_receive_packet();

void CC1101_print_packet();

#endif
