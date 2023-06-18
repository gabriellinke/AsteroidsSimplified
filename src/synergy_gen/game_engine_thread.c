/* generated thread source file - do not edit */
#include "game_engine_thread.h"

TX_THREAD game_engine_thread;
void game_engine_thread_create(void);
static void game_engine_thread_func(ULONG thread_input);
static uint8_t game_engine_thread_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.game_engine_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
TX_QUEUE control_queue;
static uint8_t queue_memory_control_queue[60];
TX_QUEUE graphic_queue;
static uint8_t queue_memory_graphic_queue[60];
TX_EVENT_FLAGS_GROUP event_flags;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void game_engine_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_control_queue;
    err_control_queue = tx_queue_create (&control_queue, (CHAR*) "Control Queue", 1, &queue_memory_control_queue,
                                         sizeof(queue_memory_control_queue));
    if (TX_SUCCESS != err_control_queue)
    {
        tx_startup_err_callback (&control_queue, 0);
    }
    UINT err_graphic_queue;
    err_graphic_queue = tx_queue_create (&graphic_queue, (CHAR*) "Graphic Queue", 1, &queue_memory_graphic_queue,
                                         sizeof(queue_memory_graphic_queue));
    if (TX_SUCCESS != err_graphic_queue)
    {
        tx_startup_err_callback (&graphic_queue, 0);
    }
    UINT err_event_flags;
    err_event_flags = tx_event_flags_create (&event_flags, (CHAR*) "Sync Event Flags");
    if (TX_SUCCESS != err_event_flags)
    {
        tx_startup_err_callback (&event_flags, 0);
    }

    UINT err;
    err = tx_thread_create (&game_engine_thread, (CHAR*) "Game Engine Thread", game_engine_thread_func, (ULONG) NULL,
                            &game_engine_thread_stack, 1024, 6, 6, 10, TX_DONT_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&game_engine_thread, 0);
    }
}

static void game_engine_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    game_engine_thread_entry ();
}
