#include "kt/terminal_font.h"
#include <assert.h>
static const uint8_t bits[8]={0};
typedef struct S{unsigned legacy,unicode;uint8_t byte,charset;}S;
static int legacy(void*p,uint8_t profile,uint8_t charset,uint8_t byte,kt_term_bitmap_glyph*g){
 S*s=p;(void)profile;s->legacy++;s->byte=byte;s->charset=charset;
 if(byte==0x41){g->width=8;g->height=8;g->stride=1;g->bitmap=bits;g->bitmap_size=8;return 0;}return 1;
}
static int unicode(void*p,uint32_t cp,kt_term_bitmap_glyph*g){S*s=p;s->unicode++;(void)g;return cp==0x2500u?0:1;}
int main(void){
 kt_term_font_ops o={legacy,unicode};kt_term_bitmap_glyph b;S s={0};kt_term_glyph g={0x41u,0x41u,KT_TERM_PROFILE_PETSCII,1u};
 assert(kt_term_font_resolve(&o,&s,&g,&b)==0&&s.legacy==1u&&s.unicode==0u&&s.byte==0x41u&&s.charset==1u);
 g.source_byte=0xc4u;g.codepoint=0x2500u;assert(kt_term_font_resolve(&o,&s,&g,&b)==0&&s.legacy==2u&&s.unicode==1u);
 assert(kt_term_font_resolve(0,&s,&g,&b)==-1);
 return 0;
}
