#include "user/task.h"

static os_event_t TASK_i2c_queue[TASK_i2c_queue_len];

void ICACHE_FLASH_ATTR TASK_i2c(os_event_t *events)
{
    if (BNO055_initialized()) {
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
            TCA6416A_set_outputs(TCA6416A_P0_4, 0);
            os_delay_us(500);

            play_sound(400, 200, 0.1);

            os_delay_us(500);
            TCA6416A_set_outputs(TCA6416A_P0_4, 1);
        }

        // if orientation did not changed for 5 minutes,
        // stop timer as sensor goes to sleep mode
        // TIMER_task_i2c_stop
        // restart timer when interrupt from BNO055 received
    }

    if (BME280_initialized()) {
        os_printf("\n\nBME280");

        sint32 temp = BME280_get_temperature_int32();
        sint16 temp_pre = temp / 100;
        if (temp < 0) temp *= -1;
        uint16 temp_post = temp % 100;

        while (temp_post > 10) {
            temp_post /= 10;
        }

        os_printf("\nTEMP:  %d.%u degC", temp_pre, temp_post);

        uint32 hum = BME280_get_humidity_int32();
        uint16 hum_pre = hum / 1024;
        uint16 hum_post = hum % 1024;

        while (hum_post > 10) {
            hum_post /= 10;
        }

        os_printf("\nHUM:   %u.%u pctRH", hum_pre, hum_post);

        uint32 press = BME280_get_pressure_int64() / 256;
        uint16 press_pre = press / 100;
        uint16 press_post = press % 100;

        while (press_post > 10) {
            press_post /= 10;
        }

        os_printf("\nPRESS: %u.%u hPa", press_pre, press_post);

        char topic[32];
        char payload[96];

        os_sprintf(topic, "liot_esp8266_env/%u/environment", DEVICE_ID);
        os_sprintf(payload, "{ \"temperature\": %d.%u, \"humidity\": %u.%u, \"pressure\": %u.%u }",
                temp_pre, temp_post, hum_pre, hum_post, press_pre, press_post);

        MQTT_publish(topic, payload, strlen(payload), 0, 0);
    }

    // check battery status
    {
        uint16 adc;
        float adc_volt;
        uint8 bat_pct;
        char topic[32];
        char payload[96];

        os_delay_us(500);
        TCA6416A_set_outputs(TCA6416A_P0_7, 1);
        os_delay_us(500);

        adc = system_adc_read();
        os_printf("\nADC: %u", adc);

        adc_volt = adc / 1024.0;

        if (adc_volt > 0.87) {
            bat_pct = 100;
        } else if (adc_volt <  0.74) {
            bat_pct = 0;
        } else {
            bat_pct = ((adc_volt - 0.74) / 0.13) * 100;
        }

        os_delay_us(500);
        TCA6416A_set_outputs(TCA6416A_P0_7, 0);
        os_delay_us(500);

        os_sprintf(topic, "liot_esp8266_env/%u/battery", DEVICE_ID);
        os_sprintf(payload, "{ \"battery\": %u, \"adc\": %u }", bat_pct, adc);

        MQTT_publish(topic, payload, strlen(payload), 0, 0);
    }
}

void ICACHE_FLASH_ATTR TASK_i2c_init()
{
    system_os_task(TASK_i2c, TASK_i2c_prio, TASK_i2c_queue, TASK_i2c_queue_len);
    TASK_i2c_restart();
}

void ICACHE_FLASH_ATTR TASK_i2c_restart()
{
    system_os_post(TASK_i2c_prio, 0, 0);
}
