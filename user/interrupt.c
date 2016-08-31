#include "user/interrupt.h"
#include "driver/tca6416a.h"

void gpio_intr_handler(int * dummy)
{
    uint8 input0, input1, i;
// clear gpio status. Say ESP8266EX SDK Programming Guide in  5.1.6. GPIO interrupt handler

    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

// if the interrupt was by GPIO0
    if (gpio_status & BIT(5))
    {
        input0 = TCA6416A_read_reg(TCA6416A_ADDR_LOW, TCA6416A_REG_INPUT_0);
        input1 = TCA6416A_read_reg(TCA6416A_ADDR_LOW, TCA6416A_REG_INPUT_1);

        if (input0 & 0x04) {
            os_printf("\nInput0: ");

            for (i = 8; i > 0; i--) {
                os_printf("%u", ((input0 >> (i-1)) & 1) ? 1 : 0);
            }

            os_printf(" - Input1: ");

            for (i = 8; i > 0; i--) {
                os_printf("%u", ((input1 >> (i-1)) & 1) ? 1 : 0);
            }

            CC1101_isr();
        }

        // CC1101_isr();

        // disable interrupt for GPIO0
        gpio_pin_intr_state_set(GPIO_ID_PIN(5), GPIO_PIN_INTR_DISABLE);
        //clear interrupt status for GPIO0
        GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(5));
        // Do something, for example, increment whatyouwant indirectly
        // (*dummy)++;
        // Reactivate interrupts for GPIO0
        gpio_pin_intr_state_set(GPIO_ID_PIN(5), GPIO_PIN_INTR_ANYEDGE);
    }
}
