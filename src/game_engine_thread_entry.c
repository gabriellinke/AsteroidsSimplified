#include "game_engine_thread.h"
#include <stdio.h>
/* Game Engine Thread entry function */
void game_engine_thread_entry(void)
{
    ULONG message, status;
    /* TODO: add your own code here */
    while (1)
    {

        status = tx_queue_receive(&control_queue, &message, TX_WAIT_FOREVER);
        if(status) printf("\nError receiving message from control queue. Error %lu", status);
        else printf("\nMessage received from control queue: %lu", message);
    }
}
