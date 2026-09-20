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
typedef struct kt_term_render_cache {
    kt_term_cell *cells;
    size_t cell_count;
    uint16_t width, height, cursor_x, cursor_y;
    uint8_t valid;
} kt_term_render_cache;

/* Incremental renderer. Cache storage is caller-owned and must hold
   width*height cells. First call renders a complete frame. */
int kt_term_render_incremental(const kt_term_screen *, uint8_t profile,
                               kt_term_render_cache *,
                               const kt_term_renderer_ops *, void *);
#endif
