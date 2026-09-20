#ifndef KT_TERMINAL_FRONTEND_H
#define KT_TERMINAL_FRONTEND_H
#include <stddef.h>
#include <stdint.h>
#include "kt/terminal_event.h"
#include "kt/terminal_renderer.h"

typedef struct kt_term_frontend {
 kt_term_screen *screen;
 uint8_t profile;
 kt_term_render_cache *cache;
 const kt_term_renderer_ops *renderer;
 void *renderer_ctx;
 const kt_term_output_ops *output;
 void *output_ctx;
 kt_term_key_output_adapter key_output;
} kt_term_frontend;

int kt_term_frontend_init(kt_term_frontend *,kt_term_screen *,uint8_t,
 kt_term_render_cache *,const kt_term_renderer_ops *,void *,
 const kt_term_output_ops *,void *);
void kt_term_frontend_invalidate(kt_term_frontend *);
int kt_term_frontend_render(kt_term_frontend *);
int kt_term_frontend_set_profile(kt_term_frontend *,uint8_t);
int kt_term_frontend_key(kt_term_frontend *,uint32_t,uint8_t);
int kt_term_frontend_flush(kt_term_frontend *);
#endif
