#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"
#include "main_thread.h"
#include "spaceship_control_thread.h"
#include "game_engine_thread.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

#define FLAG0 (1 << 0)
#define FLAG1 (1 << 1)
#define SPACESHIP 0
#define BULLET 1
#define BIG_ASTEROID 2
#define SMALL_ASTEROID 3
#define SCORE 4
#define RECORD_SCORE 5
#define BULLET_SIZE 3
#define BIG_ASTEROID_SIZE 32
#define SMALL_ASTEROID_SIZE 16
#define MAX_WIDGETS 1024
#define MASK_ID 0xFFE00000
#define SHIFT_ID 21
#define MASK_TYPE 0x001C0000
#define SHIFT_TYPE 18
#define MASK_COORDS 0x0003FFFF
#define SHIFT_COORDS 0
#define MASK_COORDS_X 0x0003FE00
#define SHIFT_COORDS_X 9
#define MASK_COORDS_Y 0x000001FF
#define SHIFT_COORDS_Y 0

extern TX_THREAD game_engine_thread;
extern TX_THREAD spaceship_control_thread;
extern GX_CANVAS display_1_canvas_control_block;
extern GX_WINDOW_ROOT * p_window_root;

void sendPressedPosition(GX_EVENT *event_ptr);

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old);
VOID custom_widget_draw(GX_WIDGET *widget, GX_PIXELMAP *pixelmap);
VOID big_asteroid1_widget_draw(GX_WIDGET *widget);
VOID big_asteroid2_widget_draw(GX_WIDGET *widget);
VOID small_asteroid1_widget_draw(GX_WIDGET *widget);
VOID small_asteroid2_widget_draw(GX_WIDGET *widget);
VOID bullet_widget_draw(GX_WIDGET *widget);

void updateScore(UINT score);
void updateRecordScore(UINT score);
void updateSpaceShip(int angle);

void updateDraw(GX_WINDOW *widget);

void updateWidgets(int num_message, GX_WINDOW *window);
void createWidget(int id, int type, GX_WINDOW *window);
void drawWidget(int position, int message);

typedef struct {
    int id;
    GX_WIDGET widget;
} MyWidget;

int num_widgets = 0;
MyWidget widgetArray[MAX_WIDGETS];
int widgetsToUpdate[MAX_WIDGETS];

char buffer[12] = "0";

VOID custom_widget_draw(GX_WIDGET *widget, GX_PIXELMAP *pixelmap)
{
    ULONG widget_style;
    gx_widget_style_get(widget, &widget_style);

    INT xpos;
    INT ypos;

    xpos = widget->gx_widget_size.gx_rectangle_right;
    xpos -= pixelmap->gx_pixelmap_width;

    ypos = widget->gx_widget_size.gx_rectangle_top;

    /* draw the extra pixelmap on top of the button */
    gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);
}

VOID small_asteroid1_widget_draw(GX_WIDGET *widget)
{
    GX_PIXELMAP *pixelmap;
    gx_context_pixelmap_get(GX_PIXELMAP_ID_ASTEROIDE2, &pixelmap);
    custom_widget_draw(widget, pixelmap);
}

VOID small_asteroid2_widget_draw(GX_WIDGET *widget)
{
    GX_PIXELMAP *pixelmap;
    gx_context_pixelmap_get(GX_PIXELMAP_ID_ASTEROIDE4, &pixelmap);
    custom_widget_draw(widget, pixelmap);
}

VOID big_asteroid1_widget_draw(GX_WIDGET *widget)
{
    GX_PIXELMAP *pixelmap;
    gx_context_pixelmap_get(GX_PIXELMAP_ID_ASTEROIDE1, &pixelmap);
    custom_widget_draw(widget, pixelmap);
}

VOID big_asteroid2_widget_draw(GX_WIDGET *widget)
{
    GX_PIXELMAP *pixelmap;
    gx_context_pixelmap_get(GX_PIXELMAP_ID_ASTEROIDE3, &pixelmap);
    custom_widget_draw(widget, pixelmap);
}

VOID bullet_widget_draw(GX_WIDGET *widget)
{
    GX_PIXELMAP *pixelmap;
    gx_context_pixelmap_get(GX_PIXELMAP_ID_PROJETIL, &pixelmap);
    custom_widget_draw(widget, pixelmap);
}

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
    GX_EVENT event;

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

/* TODO: a função verificará os objetos em uma fila e os desenhará
 * A fila pode conter o id do objeto e o offset para onde ele vai se mover
 * ou o id com a posição onde deve ser desenhado.
*/
void updateDraw(GX_WINDOW *widget) {
    UINT result;
    GX_RECTANGLE rect_area;

    gx_utility_rectangle_define(&rect_area, 0, 0, 239, 319);
    result = gx_canvas_drawing_initiate(
       &display_1_canvas_control_block,
       (GX_WIDGET *) widget,
       &rect_area);
    if(GX_SUCCESS != result) __BKPT(0);

    ULONG message;
    UINT status;

    int i=0;

    status = tx_queue_receive(&graphic_queue, &message, TX_NO_WAIT);
    while(TX_SUCCESS == status) {
        if((message & MASK_TYPE) >> SHIFT_TYPE == SPACESHIP) updateSpaceShip(message & 0x0003FFFF);
        if((message & MASK_TYPE) >> SHIFT_TYPE == SCORE) updateScore(message & 0x0003FFFF);
        if((message & MASK_TYPE) >> SHIFT_TYPE == RECORD_SCORE) updateRecordScore(message & 0x0003FFFF);
        if(((message & MASK_TYPE) >> SHIFT_TYPE == BULLET) || ((message & MASK_TYPE) >> SHIFT_TYPE == BIG_ASTEROID) || ((message & MASK_TYPE) >> SHIFT_TYPE == SMALL_ASTEROID)) {
            widgetsToUpdate[i] = message;
            i++;
        }
        status = tx_queue_receive(&graphic_queue, &message, TX_NO_WAIT);
    }

    updateWidgets(i, widget);

    if(TX_QUEUE_EMPTY != status) __BKPT(0);

    result = gx_canvas_drawing_complete(&display_1_canvas_control_block, GX_TRUE);
    if(GX_SUCCESS != result) __BKPT(0);
}

/* TODO: função deverá atualizar a orientação da nave
*/
void updateSpaceShip(int angle) {
    GX_PIXELMAP * lp_pxmap;
    UINT result;
    /* Get a pointer of the pixelmap resource */
    result = gx_context_pixelmap_get(GX_PIXELMAP_ID_NAVE2, &lp_pxmap);
    if(GX_SUCCESS != result) __BKPT(0);

    /* Draw the rotated pixelpmap */
    result = gx_canvas_pixelmap_rotate(108, 148, lp_pxmap, angle, -1, -1);                // only draws a rotated pixelmap, the source pixelmap is not changed;
    if(GX_SUCCESS != result) __BKPT(0);
}


void updateWidgets(int num_messages, GX_WINDOW *window) {
    if(!num_messages) {
        for(int i=0; i < MAX_WIDGETS; i++) {
            // Remove todos os widgets da window
            if(widgetArray[i].id != 0) {
                gx_widget_detach(&widgetArray[i].widget);
                gx_widget_delete(&widgetArray[i].widget);
            }
            widgetArray[i].id = 0;
            widgetsToUpdate[i] = 0;
        }
        return;
    }

    int i, j;
    for(i=0; i < num_messages; i++) {
        int message = widgetsToUpdate[i];
        int id = (message & MASK_ID) >> SHIFT_ID;
        int type = (message & MASK_TYPE) >> SHIFT_TYPE;
        int widgetExists = 0;

        for(j=0; j<MAX_WIDGETS; j++) if(widgetArray[j].id == id) widgetExists = 1;

        // ID não está no array -> cria widget e adiciona no array
        // No fim todos os IDS do widgetsToUpdate estarão no array
        if(!widgetExists) {
            createWidget(id, type, window);
        }
    }

    // Compara os IDS dos 2 vetores. Se no array tem IDs sobrando, remove widget da window, exclui o widget e tira do array
    for(j=0; j<MAX_WIDGETS; j++) {
        if(widgetArray[j].id != 0) {
            int isInUpdateArray = 0;
            for(i=0; i < num_messages; i++) {
                int message = widgetsToUpdate[i];
                int id = (message & MASK_ID) >> SHIFT_ID;
                if(widgetArray[j].id == id) isInUpdateArray = 1;
            }
            if(!isInUpdateArray) {
                gx_widget_detach(&widgetArray[j].widget);
                gx_widget_delete(&widgetArray[j].widget);
                widgetArray[j].id = 0;
            }
        }
    }

    // Desenha todos os widgets que estiverem no array
    for(i=0; i < num_messages; i++) {
        int message = widgetsToUpdate[i];
        int id = (message & MASK_ID) >> SHIFT_ID;

        for(j=0; j<MAX_WIDGETS; j++)
            if(widgetArray[j].id == id)
                drawWidget(j, message);
    }

    // Limpa widgetsToUpdate
    for(i=0; i < MAX_WIDGETS; i++) {
        widgetsToUpdate[i] = 0;
    }
}


void drawWidget(int position, int message) {
    int type = (message & MASK_TYPE) >> SHIFT_TYPE;
    int x = (message & MASK_COORDS_X) >> SHIFT_COORDS_X;
    int y = (message & MASK_COORDS_Y) >> SHIFT_COORDS_Y;

    int size = 0;
    if(type == BULLET) size = BULLET_SIZE;
    else if(type == BIG_ASTEROID) size = BIG_ASTEROID_SIZE;
    else size = SMALL_ASTEROID_SIZE;

    GX_RECTANGLE client;
    gx_widget_client_get(&widgetArray[position].widget, 0, &client);
    GX_VALUE xpos = client.gx_rectangle_left + size/2;
    GX_VALUE ypos = client.gx_rectangle_top + size/2;

    GX_VALUE dx = x - xpos;
    GX_VALUE dy = y - ypos;
    UINT status = gx_widget_shift(&widgetArray[position].widget, dx, dy, GX_TRUE);
    if(GX_SUCCESS != status) __BKPT(0);
}

void sendPressedPosition(GX_EVENT *event_ptr) {
    GX_VALUE x = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x;
    GX_VALUE y = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y;

    // b31-b30 = 10 / b29-b15 = x / b14-b0 = y
    UINT coord  = (1 << 31) | (0 << 30) | (x << 15) | y;

    UINT status = tx_queue_send(&touch_queue, &coord, TX_NO_WAIT);
    if(TX_SUCCESS != status) __BKPT(0);
}

void updateScore(UINT score) {
    GX_WIDGET *widget_found;

    snprintf(buffer, sizeof(buffer), "%d", score);
    GX_STRING new_string;
    new_string.gx_string_ptr = buffer;
    new_string.gx_string_length = strlen(new_string.gx_string_ptr);

    // Update score
    UINT status = gx_system_widget_find(ID_SCORE, GX_SEARCH_DEPTH_INFINITE, &widget_found);
    if(GX_SUCCESS != status) __BKPT(0);
    status = gx_prompt_text_set_ext((GX_PROMPT *)widget_found, &new_string);
    if(GX_SUCCESS != status) __BKPT(0);
    status = gx_system_dirty_mark(widget_found);
    if(GX_SUCCESS != status) __BKPT(0);
}

void updateRecordScore(UINT score) {
    GX_WIDGET *widget_found;

    snprintf(buffer, sizeof(buffer), "%d", score);
    GX_STRING new_string;
    new_string.gx_string_ptr = buffer;
    new_string.gx_string_length = strlen(new_string.gx_string_ptr);

    // Update score
    UINT status = gx_system_widget_find(ID_RECORD_SCORE, GX_SEARCH_DEPTH_INFINITE, &widget_found);
    if(GX_SUCCESS != status) __BKPT(0);
    status = gx_prompt_text_set_ext((GX_PROMPT *)widget_found, &new_string);
    if(GX_SUCCESS != status) __BKPT(0);
    status = gx_system_dirty_mark(widget_found);
    if(GX_SUCCESS != status) __BKPT(0);
}

void createWidget(int id, int type, GX_WINDOW *window) {
    GX_RECTANGLE rect_area;
    UINT status;
    srand(time(NULL));
    int random_number = rand()%2+1;
    switch(type) {
        case 1:
            gx_utility_rectangle_define(&rect_area, 0, 0, BULLET_SIZE, BULLET_SIZE);
            status = gx_widget_create((GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget, "bullet", window, GX_NULL, id, &rect_area);
            if(GX_SUCCESS != status) __BKPT(0);
            gx_widget_draw_set((GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget, (VOID (*)(GX_WIDGET *))bullet_widget_draw);
            gx_widget_attach(window, (GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget);
            break;
        case 2:
            gx_utility_rectangle_define(&rect_area, 0, 0, BIG_ASTEROID_SIZE, BIG_ASTEROID_SIZE);
            status = gx_widget_create((GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget, "big asteroid", window, GX_NULL, id, &rect_area);
            if(GX_SUCCESS != status) __BKPT(0);
            if(random_number == 1)
                gx_widget_draw_set((GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget, (VOID (*)(GX_WIDGET *))big_asteroid1_widget_draw);
            else
                gx_widget_draw_set((GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget, (VOID (*)(GX_WIDGET *))big_asteroid2_widget_draw);
            gx_widget_attach(window, (GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget);
            break;
        case 3:
            gx_utility_rectangle_define(&rect_area, 0, 0, SMALL_ASTEROID_SIZE, BIG_ASTEROID_SIZE);
            status = gx_widget_create((GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget, "small asteroid", window, GX_NULL, id, &rect_area);
            if(GX_SUCCESS != status) __BKPT(0);
            if(random_number == 1)
                gx_widget_draw_set((GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget, (VOID (*)(GX_WIDGET *))small_asteroid1_widget_draw);
            else
                gx_widget_draw_set((GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget, (VOID (*)(GX_WIDGET *))small_asteroid2_widget_draw);
            gx_widget_attach(window, (GX_WIDGET *)&widgetArray[num_widgets % MAX_WIDGETS].widget);
            break;
        default:
            break;
    }
    widgetArray[num_widgets % MAX_WIDGETS].id = id;
    num_widgets++;
}
