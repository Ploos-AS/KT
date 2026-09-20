#include "kt/terminal_font.h"
#include <assert.h>
static const uint8_t bits8[8]={0},bits16[16]={0};
typedef struct S{unsigned legacy,unicode;uint8_t byte,charset;int mode;}S;
static int legacy(void*p,uint8_t profile,uint8_t charset,uint8_t byte,kt_term_bitmap_glyph*g){
 S*s=p;(void)profile;s->legacy++;s->byte=byte;s->charset=charset;
 if(s->mode==-7)return -7;if(byte!=0x41)return 1;
 g->width=8;g->height=8;g->stride=1;g->bitmap=bits8;g->bitmap_size=s->mode==2?7:8;return 0;
}
static int unicode(void*p,uint32_t cp,kt_term_bitmap_glyph*g){
 S*s=p;s->unicode++;if(s->mode==-8)return -8;
 if(cp==0x2500u){g->width=8;g->height=16;g->stride=1;g->bitmap=bits16;g->bitmap_size=16;return 0;}
 if(cp==0xfffdu){g->width=8;g->height=8;g->stride=1;g->bitmap=bits8;g->bitmap_size=8;return 0;}return 1;
}
int main(void){
 kt_term_font_ops o={legacy,unicode};kt_term_bitmap_glyph b;S s={0};kt_term_glyph g={0x41u,0x41u,KT_TERM_PROFILE_PETSCII,1u};
 assert(kt_term_font_resolve(&o,&s,&g,&b)==0&&b.width==8u&&b.height==8u&&s.unicode==0u);
 g.source_byte=0xc4u;g.codepoint=0x2500u;assert(kt_term_font_resolve(&o,&s,&g,&b)==0&&b.height==16u);
 g.source_byte=0xfeu;g.codepoint=0x1234u;assert(kt_term_font_resolve(&o,&s,&g,&b)==0&&s.unicode>=2u);
 s.mode=2;g.source_byte=0x41u;g.codepoint=0x41u;assert(kt_term_font_resolve(&o,&s,&g,&b)==-2);
 s.mode=-7;assert(kt_term_font_resolve(&o,&s,&g,&b)==-7);
 s.mode=-8;g.source_byte=0xfeu;assert(kt_term_font_resolve(&o,&s,&g,&b)==-8);
 {kt_term_bitmap_glyph x={9,1,1,bits8,8};assert(!kt_term_bitmap_glyph_valid(&x));}
 assert(kt_term_font_resolve(0,&s,&g,&b)==-1);
 return 0;
}
