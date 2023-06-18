#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"
#include <string.h>
#include <stdio.h>

#include "main_thread.h"
#include "spaceship_control_thread.h"
#include "game_engine_thread.h"

extern TX_THREAD game_engine_thread;
extern TX_THREAD spaceship_control_thread;
extern GX_WINDOW_ROOT * p_window_root;

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old);
void updateScore(UINT score);
void updateDraw();
void updateSpaceShip();
void sendPressedPosition(GX_EVENT *event_ptr);

int angle = 0;
char buffer[12] = "0";

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
            sendPressedPosition(event_ptr);
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
    UINT status = gx_system_widget_find(asteroids_1, GX_SEARCH_DEPTH_INFINITE, &widget_found);
    status = gx_widget_shift(widget_found, 0, 2, GX_TRUE);

    ULONG message;
    status = tx_queue_receive(&graphic_queue, &message, 1);
    if(message >> 18 == 5) updateScore(message);

   /* UINT status = gx_system_widget_find(spaceship, GX_SEARCH_DEPTH_INFINITE, &widget_found);
    gx_utility_pixelmap_rotate();
    widget_found.gx_widget_rotation_angle = 90;
    status = gx_system_dirty_mark(widget_found);*/
}

/* TODO: função deverá atualizar a orientação da nave
*/
void updateSpaceShip() {
    GX_WIDGET *widget_found;
    UINT status = gx_system_widget_find(asteroids_1, GX_SEARCH_DEPTH_INFINITE, &widget_found);
    status = gx_widget_shift(widget_found, 0, -50, GX_TRUE );
}

void sendPressedPosition(GX_EVENT *event_ptr) {
    GX_VALUE x = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x;
    GX_VALUE y = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y;

    // b31-b30 = 10 / b29-b15 = x / b14-b0 = y
    UINT coord  = (1 << 31) | (0 << 30) | (x << 15) | y;

    UINT status = tx_queue_send(&touch_queue, &coord, TX_NO_WAIT);
    if(status) printf("\nError sending shot message to touch_queue. Err %d", status);
}

void updateScore(UINT score) {
    GX_WIDGET *widget_found;

    snprintf(buffer, sizeof(buffer), "%d", score & 0x0003FFFF);
    GX_STRING new_string;
    new_string.gx_string_ptr = buffer;
    new_string.gx_string_length = strlen(new_string.gx_string_ptr);

    // Update score
    UINT status = gx_system_widget_find(ID_SCORE, GX_SEARCH_DEPTH_INFINITE, &widget_found);
    status = gx_prompt_text_set_ext((GX_PROMPT *)widget_found, &new_string);
    status = gx_system_dirty_mark(widget_found);
}

VOID window2_draw(GX_WINDOW *widget) {
    UINT result;
    GX_PIXELMAP * lp_pxmap;

    /* Draw the specified window */
    gx_window_draw(widget);

    /* Get a pointer of the pixelmap resource */
    result = gx_context_pixelmap_get(GX_PIXELMAP_ID_NAVE2, &lp_pxmap);
    if(TX_SUCCESS != result) __BKPT(0);

    /* Draw the rotated pixelpmap */
    angle++;
    result = gx_canvas_pixelmap_rotate(108, 148, lp_pxmap, angle, -1, -1);                // only draws a rotated pixelmap, the source pixelmap is not changed;
    if(TX_SUCCESS != result) __BKPT(0);
}
