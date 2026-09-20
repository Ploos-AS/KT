#include "kt/terminal_scheduler.h"
void kt_term_present_scheduler_reset(kt_term_present_scheduler*s){if(s){s->pending=0;s->immediate=0;s->blink_pending=0;s->first_row=0;s->count=0;}}
int kt_term_present_scheduler_add_rows(kt_term_present_scheduler*s,const kt_term_dirty_rows*d){
 uint32_t end,cur;if(!s||!d)return -1;if(!d->valid||!d->count)return 1;
 end=(uint32_t)d->first+d->count;if(!s->pending){s->first_row=d->first;s->count=d->count;s->pending=1;s->immediate=1;return 0;}
 cur=(uint32_t)s->first_row+s->count;if(d->first<s->first_row)s->first_row=d->first;if(end>cur)cur=end;s->count=(uint16_t)(cur-s->first_row);return 0;
}
void kt_term_present_scheduler_request_blink(kt_term_present_scheduler*s){if(s)s->blink_pending=1;}
int kt_term_present_scheduler_take(kt_term_present_scheduler*s,kt_term_dirty_rows*out){
 if(!s||!out)return -1;kt_term_dirty_rows_reset(out);
 if(!s->pending&&!s->blink_pending)return 1;
 if(s->pending){out->first=s->first_row;out->count=s->count;out->valid=1;}
 s->pending=0;s->immediate=0;s->blink_pending=0;return 0;
}
