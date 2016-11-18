#include "user/interrupt.h"

static int INTERRUPT_var;

void INTERRUPT_init_user()
{
    // set GPIO 5 Pin as GPIO Input
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
    PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO5_U);
    GPIO_DIS_OUTPUT(5);

    // avoid getting disturbed by an Interrupt
    ETS_GPIO_INTR_DISABLE();

    ETS_GPIO_INTR_ATTACH(INTERRUPT_handler, &INTERRUPT_var);

    // clear Register
    gpio_register_set(GPIO_PIN_ADDR(5),
            GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE) |
            GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE) |
            GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(5));

    // assign Interrupt
    gpio_pin_intr_state_set(GPIO_ID_PIN(5), INTERRUPT_TYPE);
    ETS_GPIO_INTR_ENABLE();
}

void INTERRUPT_handler(int * var)
{
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

    // if the Interrupt was by GPIO 5
    if (gpio_status & BIT(5)) {

        // see which inputs from port expander have changed since the interrupt
        uint16 input_diff = TCA6416A_get_input_diff();

        if (input_diff & INTERRUPT_BNO055) {
            os_printf("\nInterrupt from BNO055");
            TASK_i2c_restart();
        }

        if (input_diff & INTERRUPT_CC1101_GDO0) {
            // os_printf("\nInterrupt from CC1101 GDO0");
            // CC1101_isr();
        }

        if (input_diff & INTERRUPT_CC1101_GDO2) {
            // os_printf("\nInterrupt from CC1101 GDO2");
            CC1101_isr();
        }

        // disable interrupt for GPIO 5
        gpio_pin_intr_state_set(GPIO_ID_PIN(5), GPIO_PIN_INTR_DISABLE);
        // clear interrupt status for GPIO 5
        GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(5));
        // reactivate interrupts for GPIO 5
        gpio_pin_intr_state_set(GPIO_ID_PIN(5), INTERRUPT_TYPE);
    }
}
