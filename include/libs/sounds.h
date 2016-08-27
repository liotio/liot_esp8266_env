#include "driver/soft_i2c.h"

#define TONE_A_HASH     1080 // 454,6 Hz
#define TONE_X          1100 //  74,4 Hz
#define TONE_D          5000 // 147,5 Hz
#define TONE_E          6000 //  82,0 Hz
#define TONE_TEST       7000

void play_sound(
        uint16 tone,
        uint32 time_ms,
        double vol_pct);