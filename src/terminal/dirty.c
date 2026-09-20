#include "kt/terminal_dirty.h"
void kt_term_dirty_rows_reset(kt_term_dirty_rows*d){if(d){d->first=0;d->count=0;d->valid=0;}}
static int add(kt_term_dirty_rows*d,uint16_t first,uint16_t count,uint16_t h){uint32_t end,cur;if(!d||!count||first>=h)return -1;end=(uint32_t)first+count;if(end>h)end=h;if(!d->valid){d->first=first;d->count=(uint16_t)(end-first);d->valid=1;return 0;}cur=(uint32_t)d->first+d->count;if(first<d->first)d->first=first;if(end>cur)cur=end;d->count=(uint16_t)(cur-d->first);return 0;}
int kt_term_dirty_rows_add_cell(kt_term_dirty_rows*d,uint16_t y,uint8_t ch,uint16_t h){uint32_t first;if(!ch)return -1;first=(uint32_t)y*ch;if(first>=h)return -1;return add(d,(uint16_t)first,ch,h);}
int kt_term_dirty_rows_add_cursor(kt_term_dirty_rows*d,uint16_t old_y,uint16_t new_y,uint8_t ch,uint16_t h){int a,b;a=kt_term_dirty_rows_add_cell(d,old_y,ch,h);b=kt_term_dirty_rows_add_cell(d,new_y,ch,h);return (a==0||b==0)?0:-1;}
