#include "game_engine_thread.h"

/* Game Engine Thread entry function */
void game_engine_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        tx_thread_sleep (1);
    }
}
