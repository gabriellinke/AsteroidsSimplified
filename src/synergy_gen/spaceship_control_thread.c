/* generated thread source file - do not edit */
#include "spaceship_control_thread.h"

TX_THREAD spaceship_control_thread;
void spaceship_control_thread_create(void);
static void spaceship_control_thread_func(ULONG thread_input);
static uint8_t spaceship_control_thread_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.spaceship_control_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
TX_QUEUE button_queue;
static uint8_t queue_memory_button_queue[32];
TX_QUEUE touch_queue;
static uint8_t queue_memory_touch_queue[32];
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void spaceship_control_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_button_queue;
    err_button_queue = tx_queue_create (&button_queue, (CHAR*) "Button Queue", 1, &queue_memory_button_queue,
                                        sizeof(queue_memory_button_queue));
    if (TX_SUCCESS != err_button_queue)
    {
        tx_startup_err_callback (&button_queue, 0);
    }
    UINT err_touch_queue;
    err_touch_queue = tx_queue_create (&touch_queue, (CHAR*) "Touch Queue", 1, &queue_memory_touch_queue,
                                       sizeof(queue_memory_touch_queue));
    if (TX_SUCCESS != err_touch_queue)
    {
        tx_startup_err_callback (&touch_queue, 0);
    }

    UINT err;
    err = tx_thread_create (&spaceship_control_thread, (CHAR*) "Spaceship Control Thread",
                            spaceship_control_thread_func, (ULONG) NULL, &spaceship_control_thread_stack, 1024, 6, 6,
                            10, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&spaceship_control_thread, 0);
    }
}

static void spaceship_control_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    spaceship_control_thread_entry ();
}
