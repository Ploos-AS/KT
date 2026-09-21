#include "kt/terminal_session.h"
#include <limits.h>
#include <string.h>

static int required_cells(uint16_t cols,uint16_t rows,size_t *out){
 size_t c=(size_t)cols,r=(size_t)rows;
 if(!out||!cols||!rows)return -1;
 if(c>SIZE_MAX/r)return -2;
 *out=c*r;
 return 0;
}
static void blank_cell(kt_term_cell*c,const kt_term_attr*a){
 c->ch=' ';c->charset=KT_TERM_PETSCII_UPPER_GRAPHICS;c->attr=*a;
}

int kt_term_session_init(kt_term_session*s,kt_term_geometry*g,kt_term_screen*screen,
                         kt_term_cell*cells,size_t capacity){
 size_t need,i;
 if(!s||!g||!screen||!cells||!capacity)return -1;
 if(required_cells(g->cols,g->rows,&need)!=0||need>capacity)return -3;
 s->geometry=g;s->screen=screen;s->cells=cells;s->cell_capacity=capacity;s->render_cache=0;
 screen->cells=cells;screen->width=g->cols;screen->height=g->rows;
 screen->cursor_x=0;screen->cursor_y=0;
 screen->attr.fg=7;screen->attr.bg=0;screen->attr.flags=0;
 screen->charset=KT_TERM_PETSCII_UPPER_GRAPHICS;
 for(i=0;i<need;i++)blank_cell(&cells[i],&screen->attr);
 return 0;
}

void kt_term_session_bind_render_cache(kt_term_session*s,kt_term_render_cache*cache){
 if(s){s->render_cache=cache;if(cache)cache->valid=0;}
}

int kt_term_session_apply_geometry(kt_term_session*s){
 size_t need,i;
 uint16_t ow,oh,nw,nh,copyw,copyh,y;
 kt_term_cell *cells;
 if(!s||!s->geometry||!s->screen||!s->cells)return -1;
 nw=s->geometry->cols;nh=s->geometry->rows;
 if(required_cells(nw,nh,&need)!=0)return -2;
 if(need>s->cell_capacity)return -3;
 ow=s->screen->width;oh=s->screen->height;cells=s->cells;
 if(ow==nw&&oh==nh)return 0;
 copyw=ow<nw?ow:nw;copyh=oh<nh?oh:nh;
 /* Move retained rows in-place. Growing width moves bottom-up to avoid overlap. */
 if(nw>ow){
  for(y=copyh;y>0;y--)memmove(&cells[(size_t)(y-1)*nw],&cells[(size_t)(y-1)*ow],(size_t)copyw*sizeof(*cells));
 }else{
  for(y=0;y<copyh;y++)memmove(&cells[(size_t)y*nw],&cells[(size_t)y*ow],(size_t)copyw*sizeof(*cells));
 }
 for(y=0;y<nh;y++)for(i=(y<copyh?copyw:0);i<nw;i++)blank_cell(&cells[(size_t)y*nw+i],&s->screen->attr);
 s->screen->width=nw;s->screen->height=nh;s->screen->cells=cells;
 if(s->screen->cursor_x>=nw)s->screen->cursor_x=(uint16_t)(nw-1);
 if(s->screen->cursor_y>=nh)s->screen->cursor_y=(uint16_t)(nh-1);
 if(s->render_cache)s->render_cache->valid=0;
 return 0;
}
