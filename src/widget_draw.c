#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"
#include "constants.h"

VOID custom_widget_draw(GX_WIDGET *widget, GX_PIXELMAP *pixelmap);
VOID big_asteroid1_widget_draw(GX_WIDGET *widget);
VOID big_asteroid2_widget_draw(GX_WIDGET *widget);
VOID small_asteroid1_widget_draw(GX_WIDGET *widget);
VOID small_asteroid2_widget_draw(GX_WIDGET *widget);
VOID bullet_widget_draw(GX_WIDGET *widget);

VOID custom_widget_draw(GX_WIDGET *widget, GX_PIXELMAP *pixelmap)
{
    ULONG widget_style;
    gx_widget_style_get(widget, &widget_style);

    GX_VALUE xpos, ypos;

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
