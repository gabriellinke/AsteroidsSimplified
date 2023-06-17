#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"

#include "main_thread.h"

extern TX_THREAD game_engine_thread;
extern TX_THREAD spaceship_control_thread;
extern GX_WINDOW_ROOT * p_window_root;

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old);
void updateDraw();
void updateSpaceShip();

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

    switch (event_ptr->gx_event_type){

        case GX_EVENT_PEN_UP:
            // Se a tela tiver sido pressionada atualiza a nave
            updateSpaceShip();
            break;
        case GX_EVENT_TIMER:
            // Cada vez que o timer estourar atualiza a tela
            updateDraw();
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
    status = tx_thread_resume(&spaceship_control_thread);

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

/* TODO: a função verificará os objetos em uma fila e os desenhará
 * A fila pode conter o id do objeto e o offset para onde ele vai se mover
 * ou o id com a posição onde deve ser desenhado.
*/
void updateDraw() {
    GX_WIDGET *widget_found;
    int status;
    status = gx_system_widget_find(asteroids_1, GX_SEARCH_DEPTH_INFINITE, &widget_found);
    status = gx_widget_shift(widget_found, 0, 2, GX_TRUE );
}

/* TODO: função deverá atualizar a orientação da nave
*/
void updateSpaceShip() {
    GX_WIDGET *widget_found;
    int status;
    status = gx_system_widget_find(asteroids_1, GX_SEARCH_DEPTH_INFINITE, &widget_found);
    status = gx_widget_shift(widget_found, 0, -50, GX_TRUE );
}
