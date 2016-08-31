#ifndef INTERRUPT_H
#define INTERRUPT_H


#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"


volatile int var_intr;

// gpio interrupt handler. See below
void gpio_intr_handler(int * dummy);

#endif
