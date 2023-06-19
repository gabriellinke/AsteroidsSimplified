#include "game_engine_thread.h"

extern void game_engine_thread_function(void);

void game_engine_thread_entry(void)
{
    game_engine_thread_function();
}
