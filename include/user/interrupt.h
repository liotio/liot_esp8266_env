#ifndef USER_INTERRUPT_H
#define USER_INTERRUPT_H

#include "ets_sys.h"
#include "eagle_soc.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"

#include "driver/tca6416a.h"

#include "user/task.h"

#define INTERRUPT_TYPE            GPIO_PIN_INTR_NEGEDGE

#define INTERRUPT_BNO055          TCA6416A_P0_0
#define INTERRUPT_CC1101_GDO0     TCA6416A_P0_1
#define INTERRUPT_CC1101_GDO2     TCA6416A_P0_2
#define INTERRUPT_CC1101          (INTERRUPT_CC1101_GDO0 | INTERRUPT_CC1101_GDO2)

void INTERRUPT_init_user();

void INTERRUPT_handler(
        int * var);

#endif
