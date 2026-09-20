#include "kt/terminal_font.h"
int kt_term_font_resolve(const kt_term_font_ops*o,void*c,const kt_term_glyph*g,kt_term_bitmap_glyph*out){
 int r;if(!o||!g||!out)return -1;
 if(o->lookup_legacy){
  r=o->lookup_legacy(c,g->profile,g->charset,g->source_byte,out);
  if(r==0)return 0;if(r<0)return r;
 }
 if(o->lookup_codepoint)return o->lookup_codepoint(c,g->codepoint,out);
 return 1;
}
