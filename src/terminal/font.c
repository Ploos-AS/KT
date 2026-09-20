#include "kt/terminal_font.h"
int kt_term_bitmap_glyph_valid(const kt_term_bitmap_glyph*g){
 size_t need;if(!g||!g->bitmap||!g->width||!g->height||!g->stride)return 0;
 if((size_t)g->stride<((size_t)g->width+7u)/8u)return 0;
 need=(size_t)g->stride*(size_t)g->height;return g->bitmap_size>=need;
}
int kt_term_font_resolve(const kt_term_font_ops*o,void*c,const kt_term_glyph*g,kt_term_bitmap_glyph*out){
 int r;if(!o||!g||!out)return -1;
 if(o->lookup_legacy){r=o->lookup_legacy(c,g->profile,g->charset,g->source_byte,out);if(r<0)return r;if(r==0)return kt_term_bitmap_glyph_valid(out)?0:-2;}
 if(o->lookup_codepoint){
  r=o->lookup_codepoint(c,g->codepoint,out);if(r<0)return r;if(r==0)return kt_term_bitmap_glyph_valid(out)?0:-2;
  if(g->codepoint!=0xfffdu){r=o->lookup_codepoint(c,0xfffdu,out);if(r<0)return r;if(r==0)return kt_term_bitmap_glyph_valid(out)?0:-2;}
 }
 return 1;
}
