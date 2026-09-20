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

static int same_cell(const kt_term_cell*a,const kt_term_cell*b){
 return a->ch==b->ch&&a->charset==b->charset&&
  a->attr.fg==b->attr.fg&&a->attr.bg==b->attr.bg&&a->attr.flags==b->attr.flags;
}
int kt_term_render_incremental(const kt_term_screen*s,uint8_t profile,
 kt_term_render_cache*cache,const kt_term_renderer_ops*ops,void*ctx){
 size_t i,n;int full;
 if(!s||!cache||!cache->cells||!ops||!ops->draw_cell)return -1;
 n=(size_t)s->width*s->height;if(cache->cell_count<n)return -1;
 full=!cache->valid||cache->width!=s->width||cache->height!=s->height||cache->profile!=profile;
 if(ops->begin_frame)ops->begin_frame(ctx,s->width,s->height);
 for(i=0;i<n;i++)if(full||!same_cell(&s->cells[i],&cache->cells[i])){
  kt_term_render_cell r; r.x=(uint16_t)(i%s->width);r.y=(uint16_t)(i/s->width);
  r.glyph=kt_term_resolve_glyph(profile,s->cells[i].ch,s->cells[i].charset);r.attr=s->cells[i].attr;
  ops->draw_cell(ctx,&r);cache->cells[i]=s->cells[i];
 }
 if(ops->draw_cursor&&(full||cache->cursor_x!=s->x||cache->cursor_y!=s->y))
  ops->draw_cursor(ctx,s->x,s->y);
 cache->width=s->width;cache->height=s->height;cache->cursor_x=s->x;cache->cursor_y=s->y;cache->profile=profile;cache->valid=1u;
 if(ops->end_frame)ops->end_frame(ctx);return 0;
}
