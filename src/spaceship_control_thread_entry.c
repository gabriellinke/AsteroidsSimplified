#include "hal_data.h"
#include "constants.h"
#include "spaceship_control_thread.h"
#include "game_engine_thread.h"
#include <math.h>

void button_timer_callback();
ULONG TIMER_TICKS = ceil(100/6); // 1 second / 6
TX_TIMER button_timer;
int button_timer_expired = 1;

/* Spaceship Control Thread entry function */
void spaceship_control_thread_entry(void)
{
    UINT status_button, status_touch, timer_status, button_message, touch_message;
    /* Initialize button interruption */
    button_interrupt.p_api->open(button_interrupt.p_ctrl,button_interrupt.p_cfg); // Open instance and initialize.

    timer_status = tx_timer_create(&button_timer, "Button Timer", button_timer_callback, TX_NULL, TIMER_TICKS, 0, TX_NO_ACTIVATE);
    if(GX_SUCCESS != timer_status) __BKPT(0);

    while (1)
    {
        status_touch = tx_queue_receive(&touch_queue, &touch_message, 1);
        status_button = tx_queue_receive(&button_queue, &button_message, 1);
        if(!status_button) {
            if(button_timer_expired) {
                button_timer_expired = 0;
                timer_status = tx_timer_change(&button_timer, TIMER_TICKS, 0);
                if(GX_SUCCESS != timer_status) __BKPT(0);
                timer_status = tx_timer_activate(&button_timer);
                if(GX_SUCCESS != timer_status) __BKPT(0);
                UINT status = tx_queue_send(&control_queue, &button_message, TX_NO_WAIT);
                if(GX_SUCCESS != status) __BKPT(0);
            }
        }
        if(!status_touch) {
            UINT status = tx_queue_send(&control_queue, &touch_message, TX_NO_WAIT);
            if(GX_SUCCESS != status) __BKPT(0);
        }
    }
}

void button_callback(external_irq_callback_args_t *p_args) {
    ULONG parameter_message = SHOT_MESSAGE;
    UINT status = tx_queue_send(&button_queue, &parameter_message, TX_NO_WAIT);
    if(GX_SUCCESS != status) __BKPT(0);
}

void button_timer_callback() {
    button_timer_expired = 1;
}


