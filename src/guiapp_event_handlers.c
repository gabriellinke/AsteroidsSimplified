#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"
#include "constants.h"

#include "main_thread.h"
#include "spaceship_control_thread.h"
#include "game_engine_thread.h"

extern TX_THREAD game_engine_thread;
extern TX_THREAD spaceship_control_thread;
extern GX_WINDOW_ROOT* p_window_root;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void sendPressedPosition(GX_EVENT *event_ptr);
static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old, bool isMenu);

extern void updateDraw(GX_WINDOW *widget);
extern void updateRecordScore();

UINT window1_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process(widget, event_ptr);

    switch (event_ptr->gx_event_type)
    {
        // Se foi gerado um evento de clique no botão de inciar jogo, renderiza a tela de jogo (segunda tela)
        case GX_SIGNAL(ID_BUTTON, GX_EVENT_CLICKED):
            show_window((GX_WINDOW*)&window2, (GX_WIDGET*)widget, true, false);
            break;
        default:
            gx_window_event_process(widget, event_ptr);
            break;
    }

    return result;
}

UINT window2_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process(widget, event_ptr);
    ULONG flag;

    switch (event_ptr->gx_event_type){

        case GX_EVENT_PEN_UP:
            // Se a tela tiver sido pressionada atualiza a nave
            sendPressedPosition(event_ptr);
            break;
        case GX_EVENT_TIMER:
            result = tx_event_flags_get(&event_flags, FLAG1, TX_OR_CLEAR, &flag, TX_WAIT_FOREVER);
            if(TX_SUCCESS != result) __BKPT(0);
            // Cada vez que o timer estourar atualiza a tela
            updateDraw(widget);

            result = tx_event_flags_get(&event_flags, FLAG2, TX_OR_CLEAR, &flag, TX_NO_WAIT);
            if(TX_SUCCESS == result) { // Se a FLAG2 está setada é porque acabou o jogo
                show_window((GX_WINDOW*)&window1, (GX_WIDGET*)widget, true, true);
            }

            result = tx_event_flags_set(&event_flags, FLAG0, TX_OR);
            if(TX_SUCCESS != result) __BKPT(0);
            break;
        default:
            result = gx_window_event_process(widget, event_ptr);
            break;
    }
    return result;
}

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old, bool isMenu)
{
    UINT err = GX_SUCCESS;

    if(isMenu) {
        UINT status;

        status = gx_system_timer_stop(&window2, 1);
        if(GX_SUCCESS != status) __BKPT(0);

        status = tx_thread_terminate(&game_engine_thread);
        if(TX_SUCCESS != status) __BKPT(0);
        status = tx_thread_reset(&game_engine_thread);
        if(TX_SUCCESS != status) __BKPT(0);
        status = tx_semaphore_ceiling_put(&g_ssp_common_initialized_semaphore, 1);
        if(TX_SUCCESS != status) __BKPT(0);

        status = tx_thread_suspend(&spaceship_control_thread);
        if(TX_SUCCESS != status) __BKPT(0);

    } else {
        UINT status;

        status = tx_thread_resume(&game_engine_thread);
        if(TX_SUCCESS != status) __BKPT(0);

        status = tx_thread_resume(&spaceship_control_thread);
        if(TX_SUCCESS != status) __BKPT(0);

        status = gx_system_timer_start(&window2, 1, 2, 2);
        if(GX_SUCCESS != status) __BKPT(0);
    }

    if (!p_new->gx_widget_parent)
    {
        err = gx_widget_attach(p_window_root, p_new);
    }
    else
    {
        err = gx_widget_show(p_new);
    }

    gx_system_focus_claim(p_new);

    if(isMenu) updateRecordScore();

    GX_WIDGET * p_old = p_widget;
    if (p_old && detach_old)
    {
        if (p_old != (GX_WIDGET*)p_new)
        {
            gx_widget_detach(p_old);
        }
    }

    return err;
}

void sendPressedPosition(GX_EVENT *event_ptr) {
    GX_VALUE x = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x;
    GX_VALUE y = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y;

    // b31-b30 = 10 / b17-b9 = x / b8-b0 = y
    UINT coord  = TOUCH_MESSAGE << SHIFT_INPUT | (x << SHIFT_COORDS_X) | y << SHIFT_COORDS_Y;

    UINT status = tx_queue_send(&touch_queue, &coord, TX_NO_WAIT);
    if(TX_SUCCESS != status) __BKPT(0);
}
