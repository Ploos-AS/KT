#include "kt/terminal_blink.h"
#include "kt/terminal_scheduler.h"
#include <assert.h>
#include <string.h>
typedef struct S{unsigned n;uint16_t first,count;}S;
static int rows(void*p,const kt_term_display_frame*f,uint16_t first,uint16_t count){S*s=p;(void)f;s->n++;s->first=first;s->count=count;return 0;}
int main(void){
 kt_term_cell cells[6];kt_term_screen scr;kt_term_dirty_rows d;kt_term_present_scheduler q;uint8_t pix[24*8];kt_term_display_frame f={pix,8,24,8,KT_TERM_DISPLAY_INDEXED8,0};kt_term_display_ops o={0,rows,0};S s={0};
 assert(kt_term_screen_init(&scr,2,3,cells,6)==0);assert(kt_term_blink_dirty_rows(&scr,8,24,&d)==1&&!d.valid);
 scr.cells[2].attr.flags|=KT_TERM_ATTR_BLINK;scr.cells[5].attr.flags|=KT_TERM_ATTR_BLINK;
 assert(kt_term_blink_dirty_rows(&scr,8,24,&d)==0&&d.first==8&&d.count==16);
 kt_term_present_scheduler_reset(&q);kt_term_present_scheduler_request_blink(&q);assert(q.blink_pending);
 assert(kt_term_present_scheduler_add_rows(&q,&d)==0);
 assert(kt_term_present_scheduler_present(&q,&o,&s,&f)==0&&s.n==1&&s.first==8&&s.count==16&&!q.pending&&!q.blink_pending);
 return 0;
}
