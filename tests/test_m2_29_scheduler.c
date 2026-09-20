#include "kt/terminal_scheduler.h"
#include <assert.h>
int main(void){
 kt_term_present_scheduler s;kt_term_dirty_rows d,o;kt_term_present_scheduler_reset(&s);kt_term_dirty_rows_reset(&d);assert(kt_term_present_scheduler_take(&s,&o)==1);
 d.first=10;d.count=4;d.valid=1;assert(kt_term_present_scheduler_add_rows(&s,&d)==0);
 d.first=20;d.count=3;d.valid=1;assert(kt_term_present_scheduler_add_rows(&s,&d)==0);
 d.first=12;d.count=10;d.valid=1;assert(kt_term_present_scheduler_add_rows(&s,&d)==0);
 assert(kt_term_present_scheduler_take(&s,&o)==0&&o.valid&&o.first==10&&o.count==13);
 assert(kt_term_present_scheduler_take(&s,&o)==1);
 kt_term_present_scheduler_request_blink(&s);assert(kt_term_present_scheduler_take(&s,&o)==0&&!o.valid);
 d.valid=0;d.count=0;assert(kt_term_present_scheduler_add_rows(&s,&d)==1);
 kt_term_present_scheduler_reset(&s);assert(s.policy==KT_TERM_PRESENT_IMMEDIATE);
 d.first=2;d.count=2;d.valid=1;assert(kt_term_present_scheduler_add_rows(&s,&d)==0&&s.pending&&s.immediate);
 kt_term_present_scheduler_reset(&s);kt_term_present_scheduler_set_policy(&s,KT_TERM_PRESENT_COALESCE);
 assert(s.policy==KT_TERM_PRESENT_COALESCE&&!s.immediate);
 d.first=2;d.count=2;assert(kt_term_present_scheduler_add_rows(&s,&d)==0&&s.pending&&!s.immediate);
 d.first=8;d.count=2;assert(kt_term_present_scheduler_add_rows(&s,&d)==0&&!s.immediate);
 assert(kt_term_present_scheduler_take(&s,&o)==0&&o.first==2&&o.count==8&&!s.pending);
 kt_term_present_scheduler_set_policy(&s,KT_TERM_PRESENT_IMMEDIATE);assert(s.policy==KT_TERM_PRESENT_IMMEDIATE&&s.immediate);
 return 0;
}
