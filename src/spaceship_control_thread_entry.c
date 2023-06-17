#include "hal_data.h"
#include <stdio.h>
#include <math.h>
#include "spaceship_control_thread.h"
#include "game_engine_thread.h"

extern void initialise_monitor_handles(void);

void button_timer_callback();
ULONG TIMER_TICKS = ceil(100/6); // 1 second / 6
TX_TIMER button_timer;
int button_timer_expired = 1;

enum EventType {
    SHOT = 0,
    MOVE = 1
};

/* Spaceship Control Thread entry function */
void spaceship_control_thread_entry(void)
{
    ULONG message;

    initialise_monitor_handles(); //Used for printf outputs
    /* Initialize button interruption */
    button_interrupt.p_api->open(button_interrupt.p_ctrl,button_interrupt.p_cfg); // Open instance and initialize.

    UINT timer_status = tx_timer_create(&button_timer, "Button Timer", button_timer_callback, TX_NULL, TIMER_TICKS, 0, TX_NO_ACTIVATE);
    if(timer_status) {
        printf("Error creating timer: %d", timer_status);
    }

    while (1)
    {
        UINT status = tx_queue_receive(&button_queue, &message, TX_WAIT_FOREVER);
        if(!status) {
            printf("\nBotão apertado\n");
            if(button_timer_expired) {
                printf("\tDisparo válido\n");
                button_timer_expired = 0;
                timer_status = tx_timer_change(&button_timer, TIMER_TICKS, 0);
                if(timer_status) printf("\nError restarting timer: %d", timer_status);
                timer_status = tx_timer_activate(&button_timer);
                if(timer_status) printf("\nError restarting timer: %d", timer_status);
                // TODO: Send message in QUEUE
                message = SHOT;
                UINT status = tx_queue_send(&control_queue, &message, TX_NO_WAIT);
                if(status) printf("\nError sending shot message to control queue. Err %d", status);
            }
        } else {
            printf("\nError: %d", status);
        }

    }
}

void button_callback(external_irq_callback_args_t *p_args) {
    ULONG parameter_message = 1;
    UINT status = tx_queue_send(&button_queue, &parameter_message, TX_NO_WAIT);
}

void button_timer_callback() {
    button_timer_expired = 1;
}
