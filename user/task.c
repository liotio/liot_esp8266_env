#include "user/task.h"

static os_event_t TASK_i2c_queue[TASK_i2c_queue_len];

void TASK_i2c(os_event_t *events)
{
    os_printf("\n\nBNO055");

    sint16 head, roll, pitch;
    uint64 data;
    data = BNO055_read_euler();

    head  = (sint16) (data & 0xFFFF) / 16;
    data  = data >> 16;
    roll  = (sint16) (data & 0xFFFF) / 16;
    data  = data >> 16;
    pitch = (sint16) (data & 0xFFFF) / 16;

    os_printf("\nHEAD:  %d", head);
    os_printf("\nROLL:  %d", roll);
    os_printf("\nPITCH: %d", pitch);

    if ((pitch < -45 && pitch >= -180)
            || (pitch > 45 && pitch <= 180)
            || (roll < -45 && roll >= -180)
            || (roll > 45 && roll <= 180)) {

        os_delay_us(500);
        TCA6416A_set_outputs_low(TCA6416A_P0_4);
        os_delay_us(500);

        play_sound( 400, 200, 0.06);

        os_delay_us(500);
        TCA6416A_set_outputs_high(TCA6416A_P0_4);
    }

    // if orientation did not changed for 5 minutes,
    // stop timer as sensor goes to sleep mode
    // TIMER_task_i2c_stop
    // restart timer when interrupt from BNO055 received

    os_printf("\n\nBME280");
    os_printf("\n\nTEMP:  %d", BME280_get_temperature_int32());
    os_printf("\n\nHUM:   %d", BME280_get_humidity_int32());
    os_printf("\n\nPRESS: %d", BME280_get_pressure_int64());
}

void TASK_i2c_init()
{
    system_os_task(TASK_i2c, TASK_i2c_prio, TASK_i2c_queue, TASK_i2c_queue_len);
    TASK_i2c_restart();
}

void TASK_i2c_restart()
{
    system_os_post(TASK_i2c_prio, 0, 0);
}
