#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
typedef struct trace{uint32_t seq[8];int n;}trace;
static void cb(void*v,const kt_term_geometry_snapshot*o,const kt_term_geometry_snapshot*n,uint32_t f,uint32_t s){
 trace*t=(trace*)v;(void)o;(void)n;(void)f;t->seq[t->n++]=s;
}
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;kt_term_cell cells[132*43];
 kt_term_present_scheduler sched;kt_term_resize_result r;kt_term_geometry_session gs;
 kt_term_geometry_notifier slots[2],a,b;kt_term_geometry_notifier_set set;trace ta={{0},0},tb={{0},0};
 const uint8_t tel[4]={0,100,0,40};
 memset(&screen,0,sizeof(screen));memset(slots,0,sizeof(slots));
 assert(kt_term_geometry_init(&g,80,25)==0);assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&r)==0);
 kt_term_geometry_notifier_set_init(&set,slots,2);
 kt_term_geometry_notifier_init(&a,cb,&ta);
 kt_term_geometry_notifier_init(&b,cb,&tb);
 kt_term_geometry_notifier_set_filter(&b,KT_TERM_GEOMETRY_TRANSITION_POLICY|KT_TERM_GEOMETRY_TRANSITION_SCREEN);
 assert(kt_term_geometry_notifier_set_add(&set,&a,0)==0);assert(kt_term_geometry_notifier_set_add(&set,&b,0)==0);
 kt_term_geometry_session_bind_notifier_set(&gs,&set);

 /* Connect is two commits: all-observer sees 1,2; filtered observer sees only 2. */
 assert(kt_term_geometry_session_connect_telnet(&gs,tel,4)==0);
 assert(ta.n==2&&ta.seq[0]==1&&ta.seq[1]==2);
 assert(tb.n==1&&tb.seq[0]==2);

 /* Disconnect is commit 3 and both observers receive the identical ID. */
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(ta.n==3&&ta.seq[2]==3);assert(tb.n==2&&tb.seq[1]==3);

 /* Wrap skips reserved zero. */
 gs.notify_sequence=UINT32_MAX;
 assert(kt_term_geometry_session_connect_telnet(&gs,tel,4)==0);
 assert(ta.seq[3]==1&&ta.seq[4]==2);
 assert(tb.seq[2]==2); /* filtered gap remains visible */
 return 0;
}
