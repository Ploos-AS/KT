#include "kt/terminal_dirty.h"
void kt_term_dirty_rows_reset(kt_term_dirty_rows*d){if(d){d->first=0;d->count=0;d->valid=0;}}
static int add(kt_term_dirty_rows*d,uint16_t first,uint16_t count,uint16_t h){uint32_t end,cur;if(!d||!count||first>=h)return -1;end=(uint32_t)first+count;if(end>h)end=h;if(!d->valid){d->first=first;d->count=(uint16_t)(end-first);d->valid=1;return 0;}cur=(uint32_t)d->first+d->count;if(first<d->first)d->first=first;if(end>cur)cur=end;d->count=(uint16_t)(cur-d->first);return 0;}
int kt_term_dirty_rows_add_cell(kt_term_dirty_rows*d,uint16_t y,uint8_t ch,uint16_t h){uint32_t first;if(!ch)return -1;first=(uint32_t)y*ch;if(first>=h)return -1;return add(d,(uint16_t)first,ch,h);}
int kt_term_dirty_rows_add_cursor(kt_term_dirty_rows*d,uint16_t old_y,uint16_t new_y,uint8_t ch,uint16_t h){int a,b;a=kt_term_dirty_rows_add_cell(d,old_y,ch,h);b=kt_term_dirty_rows_add_cell(d,new_y,ch,h);return (a==0||b==0)?0:-1;}

int kt_term_dirty_rows_from_damage(kt_term_dirty_rows*d,const kt_term_render_damage*r,uint8_t ch,uint16_t h){
 uint32_t y;int ok=0;if(!d||!r||!ch||!h)return -1;kt_term_dirty_rows_reset(d);
 if(r->full){d->first=0;d->count=h;d->valid=1u;return 0;}
 if(r->cells_changed)for(y=r->first_row;y<(uint32_t)r->first_row+r->row_count;y++)
  if(kt_term_dirty_rows_add_cell(d,(uint16_t)y,ch,h)==0)ok=1;
 if(r->cursor_changed&&kt_term_dirty_rows_add_cursor(d,r->old_cursor_y,r->new_cursor_y,ch,h)==0)ok=1;
 return ok?0:1;
}
