#include "kt/terminal_renderer.h"

int kt_term_render(const kt_term_screen *s,uint8_t profile,
                   const kt_term_renderer_ops *ops,void *ctx){
 uint16_t x,y;
 if(!s||!ops||!ops->draw_cell)return -1;
 if(ops->begin_frame)ops->begin_frame(ctx,s->width,s->height);
 for(y=0;y<s->height;y++)for(x=0;x<s->width;x++){
  const kt_term_cell *c=&s->cells[(size_t)y*s->width+x];
  kt_term_render_cell r;
  r.x=x;r.y=y;r.glyph=kt_term_resolve_glyph(profile,c->ch,c->charset);r.attr=c->attr;
  ops->draw_cell(ctx,&r);
 }
 if(ops->draw_cursor)ops->draw_cursor(ctx,s->x,s->y);
 if(ops->end_frame)ops->end_frame(ctx);
 return 0;
}
