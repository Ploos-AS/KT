#include "kt/terminal_scheduler.h"
#include <assert.h>
typedef struct S{unsigned n;int fail;}S;
static int rows(void*p,const kt_term_display_frame*f,uint16_t first,uint16_t count){S*s=p;(void)f;assert(first==4&&count==8);s->n++;return s->fail?-7:0;}
int main(void){
 kt_term_present_scheduler s;kt_term_dirty_rows d;kt_term_display_ops o={0,rows,0};kt_term_display_frame f={(const uint8_t*)1,8,16,8,KT_TERM_DISPLAY_INDEXED8,0};S x={0,0};
 kt_term_present_scheduler_reset(&s);d.first=4;d.count=8;d.valid=1;assert(kt_term_present_scheduler_add_rows(&s,&d)==0);
 x.fail=1;assert(kt_term_present_scheduler_present(&s,&o,&x,&f)==-7);assert(s.pending&&x.n==1);
 x.fail=0;assert(kt_term_present_scheduler_present(&s,&o,&x,&f)==0);assert(!s.pending&&x.n==2);
 assert(kt_term_present_scheduler_present(&s,&o,&x,&f)==1);
 return 0;
}
