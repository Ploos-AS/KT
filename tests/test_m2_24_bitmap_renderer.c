#include "kt/terminal_renderer.h"
#include <assert.h>
static const uint8_t px[8]={0};
typedef struct Q{unsigned legacy,draw;uint8_t byte,charset;}Q;
static int legacy(void*p,uint8_t profile,uint8_t charset,uint8_t byte,kt_term_bitmap_glyph*g){Q*q=p;(void)profile;q->legacy++;q->byte=byte;q->charset=charset;g->width=8;g->height=8;g->stride=1;g->bitmap=px;g->bitmap_size=8;return 0;}
static void draw(void*p,const kt_term_bitmap_render_cell*c){Q*q=p;q->draw++;assert(c->bitmap.width==8u);assert(c->cell.glyph.source_byte==0x41u);assert(c->cell.glyph.charset==1u);}
int main(void){kt_term_cell cell={0};kt_term_screen s={0};kt_term_font_ops f={legacy,0};kt_term_bitmap_renderer_ops r={0};Q q={0};cell.ch=0x41u;cell.charset=1u;s.cells=&cell;s.cell_count=1;s.width=1;s.height=1;r.draw_bitmap_cell=draw;assert(kt_term_render_bitmap(&s,KT_TERM_PROFILE_PETSCII,&f,&q,&r,&q)==0);assert(q.legacy==1u&&q.draw==1u&&q.byte==0x41u&&q.charset==1u);return 0;}
