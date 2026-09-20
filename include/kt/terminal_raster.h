#ifndef KT_TERMINAL_RASTER_H
#define KT_TERMINAL_RASTER_H
#include <stddef.h>
#include <stdint.h>
#include "kt/terminal_renderer.h"

typedef struct kt_term_framebuffer {
 uint8_t *pixels;
 uint16_t width;
 uint16_t height;
 size_t stride;
} kt_term_framebuffer;

typedef struct kt_term_raster {
 kt_term_framebuffer fb;
 uint8_t cell_width;
 uint8_t cell_height;
 uint8_t fg;
 uint8_t bg;
} kt_term_raster;

int kt_term_raster_init(kt_term_raster *,uint8_t *,uint16_t,uint16_t,size_t,uint8_t,uint8_t);
void kt_term_raster_clear(kt_term_raster *,uint8_t);
void kt_term_raster_draw_bitmap_cell(void *,const kt_term_bitmap_render_cell *);
const kt_term_bitmap_renderer_ops *kt_term_raster_renderer_ops(void);
#endif
