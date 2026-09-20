#include "kt/terminal_renderer.h"
#include <assert.h>
static const uint8_t bits[8]={0};
typedef struct S{unsigned lookup,draw;uint8_t byte,charset;uint32_t cp;}S;
static int legacy(void*p,uint8_t profile,uint8_t charset,uint8_t byte,kt_term_bitmap_glyph*g){
 S*s=p;(void)profile;s->lookup++;s->byte=byte;s->charset=charset;
 if(byte!=0x41u)return 1;g->width=8;g->height=8;g->stride=1;g->bitmap=bits;g->bitmap_size=8;return 0;
}
static int unicode(void*p,uint32_t cp,kt_term_bitmap_glyph*g){S*s=p;s->cp=cp;
 if(cp!=0xfffdu)return 1;g->width=8;g->height=8;g->stride=1;g->bitmap=bits;g->bitmap_size=8;return 0;}
static void draw(void*p,const kt_term_bitmap_render_cell*c){S*s=p;s->draw++;assert(c->bitmap.width==8u&&c->bitmap.height==8u);}
int main(void){
 kt_term_cell cells[2];kt_term_screen scr;kt_term_font_ops fo={legacy,unicode};kt_term_bitmap_renderer_ops ro={0,draw,0,0};S s={0};
 assert(kt_term_screen_init(&scr,2,1,cells,2)==0);scr.cells[0].ch=0x41;scr.cells[0].charset=1;
 scr.cells[1].ch=0xfe;scr.cells[1].charset=0;
 assert(kt_term_render_bitmap(&scr,KT_TERM_PROFILE_PETSCII,&fo,&s,&ro,&s)==0);
 assert(s.draw==2u&&s.lookup==2u&&s.byte==0xfeu&&s.cp==0xfffdu);
 return 0;
}
