#include "game_engine_thread.h"
#include <stdio.h>
int points = 0;
/* Game Engine Thread entry function */
void game_engine_thread_entry(void)
{
    ULONG message, status;
    while (1)
    {

        status = tx_queue_receive(&control_queue, &message, TX_WAIT_FOREVER);
        if(status) printf("\nError receiving message from control queue. Error %lu", status);
        else printf("\nMessage received from control queue: %lu\n", message);
        if(message >> 30 == 0x0) {
            printf("\nShot\n");
            points += 10;
            ULONG update_score_message = 5 << 18 | points;
            UINT status = tx_queue_send(&graphic_queue, &update_score_message, TX_NO_WAIT);
        } else {
            UINT coord = message;
            // Extract the values from the bits
            int x = (coord >> 15) & 0x7FFF;
            int y = coord & 0x7FFF;
            printf("\nx: %d, y: %d, coord: %u\n", x, y, coord);
            for (int i = 31; i >= 0; i--) {
                unsigned int bit = (coord >> i) & 1;
                printf("%u", bit);
            }
            printf("\n");
        }
    }
}
