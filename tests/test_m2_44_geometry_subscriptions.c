#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
typedef struct trace { int count; } trace;
static void cb(void*ctx,const kt_term_geometry_snapshot*o,
 const kt_term_geometry_snapshot*n,uint32_t f){
 trace*t=(trace*)ctx;(void)o;(void)n;(void)f;t->count++;
}
int main(void){
 kt_term_geometry_notifier slots[2],a,b,c;kt_term_geometry_notifier_set set;
 trace ta={0},tb={0},tc={0};size_t ida=99,idb=99,idc=99;
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 const uint8_t tel[4]={0,100,0,40};
 memset(slots,0,sizeof(slots));memset(&screen,0,sizeof(screen));
 kt_term_geometry_notifier_set_init(&set,slots,2);
 kt_term_geometry_notifier_init(&a,cb,&ta);
 kt_term_geometry_notifier_init(&b,cb,&tb);
 kt_term_geometry_notifier_init(&c,cb,&tc);
 assert(kt_term_geometry_notifier_set_add(&set,&a,&ida)==0&&ida==0);
 assert(kt_term_geometry_notifier_set_add(&set,&b,&idb)==0&&idb==1);
 assert(kt_term_geometry_notifier_set_add(&set,&c,&idc)==-2);
 assert(kt_term_geometry_notifier_set_remove(&set,ida)==0);
 assert(kt_term_geometry_notifier_set_add(&set,&c,&idc)==0&&idc==ida);
 assert(idb==1); /* existing subscriber identity remains stable */
 assert(kt_term_geometry_notifier_set_remove(&set,99)==-1);

 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);
 kt_term_geometry_session_bind_notifier_set(&gs,&set);
 assert(kt_term_geometry_session_connect_telnet(&gs,tel,4)==0);
 assert(tb.count==2&&tc.count==2&&ta.count==0);

 /* Detach one observer; lifecycle continues for the remaining observer. */
 assert(kt_term_geometry_notifier_set_remove(&set,idb)==0);
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(tb.count==2);
 assert(tc.count==3);
 assert(kt_term_geometry_notifier_set_remove(&set,idb)==-1);
 return 0;
}
