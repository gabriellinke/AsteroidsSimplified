#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"
#include <stdio.h>
#include "constants.h"

#include "main_thread.h"
#include "spaceship_control_thread.h"
#include "game_engine_thread.h"

void updateDraw(GX_WINDOW *widget);

extern GX_CANVAS display_1_canvas_control_block;
extern int recordScore;

extern VOID custom_widget_draw(GX_WIDGET *widget, GX_PIXELMAP *pixelmap);
extern VOID big_asteroid1_widget_draw(GX_WIDGET *widget);
extern VOID big_asteroid2_widget_draw(GX_WIDGET *widget);
extern VOID small_asteroid1_widget_draw(GX_WIDGET *widget);
extern VOID small_asteroid2_widget_draw(GX_WIDGET *widget);
extern VOID bullet_widget_draw(GX_WIDGET *widget);

void updateScore(UINT score);
void updateRecordScore();
void updateSpaceShip(int angle, GX_WINDOW *window);

void updateWidgets(int num_message, GX_WINDOW *window);
void createWidget(int id, int type, GX_WINDOW *window);
void drawWidget(int position, int message);

typedef struct {
    int id;
    GX_WIDGET widget;
} MyWidget;

MyWidget widgetArray[MAX_WIDGETS];
int widgetsToUpdate[MAX_WIDGETS];
int num_widgets = 0;
char score_buffer[12] = "0";

void updateDraw(GX_WINDOW *widget) {
    ULONG message, status;
    int i=0;

    status = tx_queue_receive(&graphic_queue, &message, TX_NO_WAIT);
    while(TX_SUCCESS == status) {
        if((message & MASK_TYPE) >> SHIFT_TYPE == SPACESHIP) updateSpaceShip(message & 0x0003FFFF, widget);
        if((message & MASK_TYPE) >> SHIFT_TYPE == SCORE) updateScore(message & 0x0003FFFF);
        if(((message & MASK_TYPE) >> SHIFT_TYPE == BULLET) || ((message & MASK_TYPE) >> SHIFT_TYPE == BIG_ASTEROID) || ((message & MASK_TYPE) >> SHIFT_TYPE == SMALL_ASTEROID)) {
            widgetsToUpdate[i] = (int) message;
            i++;
        }
        status = tx_queue_receive(&graphic_queue, &message, TX_NO_WAIT);
    }

    updateWidgets(i, widget);

    if(TX_QUEUE_EMPTY != status) __BKPT(0);
}

void updateSpaceShip(int angle, GX_WINDOW *window) {
    GX_PIXELMAP * lp_pxmap;
    UINT result;
    GX_RECTANGLE rect_area;

    gx_utility_rectangle_define(&rect_area, 0, 0, X_MAX-1, Y_MAX-1);
    result = gx_canvas_drawing_initiate(
       &display_1_canvas_control_block,
       (GX_WIDGET *) window,
       &rect_area);
    if(GX_SUCCESS != result) __BKPT(0);

    /* Get a pointer of the pixelmap resource */
    result = gx_context_pixelmap_get(GX_PIXELMAP_ID_NAVE2, &lp_pxmap);
    if(GX_SUCCESS != result) __BKPT(0);

    /* Draw the rotated pixelpmap */
    result = gx_canvas_pixelmap_rotate(108, 148, lp_pxmap, angle, -1, -1);// only draws a rotated pixelmap, the source pixelmap is not changed;
    if(GX_SUCCESS != result) __BKPT(0);

    result = gx_canvas_drawing_complete(&display_1_canvas_control_block, GX_TRUE);
    if(GX_SUCCESS != result) __BKPT(0);
}

// TODO: Explicar melhor a função, deixar código mais claro e resolver bugs
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
    GX_VALUE x = (message & MASK_COORDS_X) >> SHIFT_COORDS_X;
    GX_VALUE y = (message & MASK_COORDS_Y) >> SHIFT_COORDS_Y;

    GX_VALUE size = 0;
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

void updateScore(UINT score) {
    GX_WIDGET *widget_found;

    snprintf(score_buffer, sizeof(score_buffer), "%d", score);
    GX_STRING new_string;
    new_string.gx_string_ptr = score_buffer;
    new_string.gx_string_length = strlen(new_string.gx_string_ptr);

    // Update score
    UINT status = gx_system_widget_find(ID_SCORE, GX_SEARCH_DEPTH_INFINITE, &widget_found);
    if(GX_SUCCESS != status) __BKPT(0);
    status = gx_prompt_text_set_ext((GX_PROMPT *)widget_found, &new_string);
    if(GX_SUCCESS != status) __BKPT(0);
    status = gx_system_dirty_mark(widget_found);
    if(GX_SUCCESS != status) __BKPT(0);
}

void updateRecordScore() {
    GX_WIDGET *widget_found;

    snprintf(score_buffer, sizeof(score_buffer), "%d", recordScore);
    GX_STRING new_string;
    new_string.gx_string_ptr = score_buffer;
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

