#include "user/sound.h"

void play_sound(
        uint16 tone,
        uint32 time_ms,
        double vol_pct)
{
    uint8 state;
    uint16 high, low;
    uint32 i, num;

    high = (tone >> 1) * vol_pct;
    low = tone - high;

    num = (time_ms << 11) / tone;

    for (i = 0; i < num; i++) {
        if (state) {
            state = 0;
            I2C_SCK_LOW;
            os_delay_us(low);
        } else {
            state = 1;
            I2C_SCK_HIGH;
            os_delay_us(high);
        }
        system_soft_wdt_feed();
    }
}
