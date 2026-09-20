#include "kt/terminal_frontend.h"
int kt_term_frontend_init(kt_term_frontend*f,kt_term_screen*s,uint8_t p,
 kt_term_render_cache*c,const kt_term_renderer_ops*r,void*rc,
 const kt_term_output_ops*o,void*oc){
 if(!f||!s||!c||!c->cells||!r||!r->draw_cell)return -1;
 f->screen=s;f->profile=p;f->cache=c;f->renderer=r;f->renderer_ctx=rc;
 f->output=o;f->output_ctx=oc;kt_term_key_output_adapter_init(&f->key_output,o,oc);return 0;
}
void kt_term_frontend_invalidate(kt_term_frontend*f){if(f&&f->cache)f->cache->valid=0u;}
int kt_term_frontend_set_profile(kt_term_frontend*f,uint8_t p){if(!f)return -1;if(f->profile!=p){f->profile=p;kt_term_frontend_invalidate(f);}return 0;}
void kt_term_frontend_invalidate(kt_term_frontend*f){if(f&&f->cache)f->cache->valid=0u;}
int kt_term_frontend_render(kt_term_frontend*f){
 if(!f)return -1;return kt_term_render_incremental(f->screen,f->profile,f->cache,f->renderer,f->renderer_ctx);
}
int kt_term_frontend_key(kt_term_frontend*f,uint32_t key,uint8_t mods){
 kt_term_event e;if(!f||!f->output)return -1;e.type=KT_TERM_EVENT_KEY;e.key=key;e.mods=mods;
 return kt_term_dispatch_event(&e,kt_term_key_output_adapter_ops(),&f->key_output);
}
int kt_term_frontend_flush(kt_term_frontend*f){if(!f)return -1;return kt_term_key_output_flush(&f->key_output);}
