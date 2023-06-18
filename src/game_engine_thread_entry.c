#include "game_engine_thread.h"
#include <stdio.h>
#include <math.h>

#define x_0 120
#define y_0 180
#define y_max 360
#define x_max 240
#define PI 3.14159

ULONG points = 0;
INT angle = 0;
/* Game Engine Thread entry function */
void game_engine_thread_entry(void)
{
    ULONG message, status;
    while (1)
    {

        status = tx_queue_receive(&control_queue, &message, TX_WAIT_FOREVER);
        if(GX_SUCCESS != status) __BKPT(0);
        if(message >> 30 == 0x0) {
            points += 10;
            ULONG update_score_message = 5 << 18 | points;
            status = tx_queue_send(&graphic_queue, &update_score_message, TX_NO_WAIT);
            if(GX_SUCCESS != status) __BKPT(0);
        } else {
            UINT coord = message;
            // Extract the values from the bits
            int x = (coord >> 15) & 0x7FFF;
            int y = y_max - (coord & 0x7FFF);
            int dx = x-x_0;
            int dy = y-y_0;
            double angle_rad = atan2(dy, dx);
            double angle_deg = angle_rad * 180.0 / PI;
            angle = (360 + 90-(INT)(angle_deg))%360;

            ULONG update_spaceship_message = 0 << 18 | angle;
            status = tx_queue_send(&graphic_queue, &update_spaceship_message, TX_NO_WAIT);
            if(GX_SUCCESS != status) __BKPT(0);
        }
    }
}
