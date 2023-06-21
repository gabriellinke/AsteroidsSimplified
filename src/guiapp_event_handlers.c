#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"
#include "constants.h"

#include "main_thread.h"
#include "spaceship_control_thread.h"
#include "game_engine_thread.h"

extern TX_THREAD game_engine_thread;
extern TX_THREAD spaceship_control_thread;
extern GX_WINDOW_ROOT* p_window_root;

void sendPressedPosition(GX_EVENT *event_ptr);
static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old);

extern void updateDraw(GX_WINDOW *widget);

UINT window1_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process(widget, event_ptr);

    switch (event_ptr->gx_event_type)
    {
        // Se foi gerado um evento de clique no botão de inciar jogo, renderiza a tela de jogo (segunda tela)
        case GX_SIGNAL(ID_BUTTON, GX_EVENT_CLICKED):
            show_window((GX_WINDOW*)&window2, (GX_WIDGET*)widget, true);
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
            result = tx_event_flags_set(&event_flags, FLAG0, TX_OR);
            if(TX_SUCCESS != result) __BKPT(0);
            break;
        default:
            result = gx_window_event_process(widget, event_ptr);
            break;
    }
    return result;
}

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old)
{
    UINT err = GX_SUCCESS;

    // TODO: verificar se tá indo pra tela 2. Se sim, inicia timer. Se tiver indo pra tela 1, desliga timer.
    // TODO: Também iniciar / parar threads
    UINT status = tx_thread_resume(&game_engine_thread);
    if(GX_SUCCESS != status) __BKPT(0);
    status = tx_thread_resume(&spaceship_control_thread);
    if(GX_SUCCESS != status) __BKPT(0);

    gx_system_timer_start(p_new, 1, 2, 2);

    if (!p_new->gx_widget_parent)
    {
        err = gx_widget_attach(p_window_root, p_new);
    }
    else
    {
        err = gx_widget_show(p_new);
    }

    gx_system_focus_claim(p_new);

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
