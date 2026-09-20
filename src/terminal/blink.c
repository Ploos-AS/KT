#include "kt/terminal_blink.h"
int kt_term_blink_dirty_rows(const kt_term_screen*s,uint8_t ch,uint16_t h,kt_term_dirty_rows*d){
 uint16_t x,y;int found=0;if(!s||!s->cells||!ch||!h||!d)return -1;kt_term_dirty_rows_reset(d);
 for(y=0;y<s->height;y++)for(x=0;x<s->width;x++){const kt_term_cell*c=&s->cells[(size_t)y*s->width+x];
  if(c->attr.flags&KT_TERM_ATTR_BLINK){if(kt_term_dirty_rows_add_cell(d,y,ch,h)==0)found=1;}
 }
 return found?0:1;
}
