#include "kt/terminal_renderer.h"
#include <assert.h>
#include <string.h>
static const uint8_t bits[8]={0x18,0x24,0x42,0x7e,0x42,0x42,0x42,0};
typedef struct S{unsigned lookups,draws;uint8_t byte,charset;uint32_t cp;}S;
static int legacy(void*p,uint8_t profile,uint8_t charset,uint8_t byte,kt_term_bitmap_glyph*g){S*s=p;(void)profile;s->lookups++;s->byte=byte;s->charset=charset;if(byte!=0x41)return 1;g->width=8;g->height=8;g->stride=1;g->bitmap=bits;g->bitmap_size=8;return 0;}
static int unicode(void*p,uint32_t cp,kt_term_bitmap_glyph*g){S*s=p;s->cp=cp;(void)g;return 1;}
static void draw(void*p,const kt_term_bitmap_render_cell*r){S*s=p;s->draws++;assert(r->cell.glyph.source_byte==0x41u);assert(r->cell.glyph.charset==1u);assert(r->cell.glyph.codepoint==0x61u);assert(r->bitmap.bitmap==bits);}
int main(void){kt_term_cell cell;kt_term_screen screen;kt_term_font_ops font={legacy,unicode};kt_term_bitmap_renderer_ops rops={0,draw,0,0};S s={0};memset(&cell,0,sizeof cell);cell.ch=0x41u;cell.charset=1u;screen.cells=&cell;screen.width=1;screen.height=1;screen.x=0;screen.y=0;assert(kt_term_render_bitmap(&screen,KT_TERM_PROFILE_PETSCII,&font,&s,&rops,&s)==0);assert(s.lookups==1u&&s.draws==1u&&s.byte==0x41u&&s.charset==1u);return 0;}
