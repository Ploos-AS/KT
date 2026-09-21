#include "kt/terminal_session.h"
#include <limits.h>

static int required_cells(uint16_t cols,uint16_t rows,size_t *out){
 size_t c=(size_t)cols,r=(size_t)rows;
 if(!out||!cols||!rows)return -1;
 if(c>SIZE_MAX/r)return -2;
 *out=c*r;
 return 0;
}

int kt_term_session_init(kt_term_session*s,kt_term_geometry*g,kt_term_screen*screen,
                         kt_term_cell*cells,size_t capacity){
 if(!s||!g||!screen||!cells||!capacity)return -1;
 s->geometry=g;s->screen=screen;s->cells=cells;s->cell_capacity=capacity;
 return kt_term_session_apply_geometry(s);
}

int kt_term_session_apply_geometry(kt_term_session*s){
 size_t need,i;
 if(!s||!s->geometry||!s->screen||!s->cells)return -1;
 if(required_cells(s->geometry->cols,s->geometry->rows,&need)!=0)return -2;
 if(need>s->cell_capacity)return -3;
 s->screen->cells=s->cells;
 s->screen->width=s->geometry->cols;
 s->screen->height=s->geometry->rows;
 s->screen->cursor_x=0;
 s->screen->cursor_y=0;
 s->screen->attr.fg=7;s->screen->attr.bg=0;s->screen->attr.flags=0;
 s->screen->charset=KT_TERM_PETSCII_UPPER_GRAPHICS;
 for(i=0;i<need;i++){
  s->cells[i].ch=' ';
  s->cells[i].charset=KT_TERM_PETSCII_UPPER_GRAPHICS;
  s->cells[i].attr=s->screen->attr;
 }
 return 0;
}
