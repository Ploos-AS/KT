#ifndef KT_TERMINAL_RENDERER_H
#define KT_TERMINAL_RENDERER_H
#include "kt/terminal_screen.h"

typedef struct kt_term_render_cell {
    uint16_t x, y;
    kt_term_glyph glyph;
    kt_term_attr attr;
} kt_term_render_cell;

typedef struct kt_term_renderer_ops {
    void (*begin_frame)(void *, uint16_t, uint16_t);
    void (*draw_cell)(void *, const kt_term_render_cell *);
    void (*draw_cursor)(void *, uint16_t, uint16_t);
    void (*end_frame)(void *);
} kt_term_renderer_ops;

/* Platform-neutral full-frame reference renderer.
   Frontends own graphics, fonts, clipping and presentation. */
int kt_term_render(const kt_term_screen *, uint8_t profile,
                   const kt_term_renderer_ops *, void *);
#endif
