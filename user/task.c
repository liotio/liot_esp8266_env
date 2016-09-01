#include "user/task.h"

static os_event_t TASK_i2c_queue[TASK_i2c_queue_len];

void TASK_i2c(os_event_t *events)
{
    // system_soft_wdt_stop();
    sint16 head, roll, ptch;
    uint64 data;

    data = BNO055_read_reg_48(BNO055_ADDR, BNO055_REG_EUL_HEAD_L);
    ptch = (sint16) (data & 0xFFFF) / 16;
    data = data >> 16;
    roll = (sint16) (data & 0xFFFF) / 16;
    data = data >> 16;
    head = (sint16) (data & 0xFFFF) / 16;
    // system_soft_wdt_restart();

    os_printf("\nROLL:  %d", roll);
    os_printf("\nPITCH: %d", ptch);

    if ((ptch < -45 && ptch >= -180)
            || (ptch > 45 && ptch <= 180)
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

    /*
    BME280_get_temperature_int32();
    BME280_get_humidity_int32();
    BME280_get_pressure_int64();
    */
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
