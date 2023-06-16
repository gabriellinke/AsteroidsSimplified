#include "hal_data.h"
#include <stdio.h>
#include "spaceship_control_thread.h"
extern void initialise_monitor_handles(void);

/* Spaceship Control Thread entry function */
void spaceship_control_thread_entry(void)
{
    initialise_monitor_handles(); //Used for printf outputs
    /* Initialize button interruption */
    button_interrupt.p_api->open(button_interrupt.p_ctrl,button_interrupt.p_cfg); // Open instance and initialize.
    ULONG message;

    while (1)
    {
        UINT status = tx_queue_receive(&button_queue, &message, TX_WAIT_FOREVER);
        if(!status) {
            printf("\nBotão apertado\n");
        } else {
            printf("Error: %d", status);
        }

    }
}

void button_callback(external_irq_callback_args_t *p_args) {
    ULONG parameter_message = 1;
    UINT status = tx_queue_send(&button_queue, &parameter_message, TX_NO_WAIT);
}
