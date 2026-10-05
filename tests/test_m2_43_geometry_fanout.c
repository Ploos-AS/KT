#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
typedef struct trace { int count; uint32_t last; } trace;
static void cb(void*ctx,const kt_term_geometry_snapshot*o,
               const kt_term_geometry_snapshot*n,uint32_t flags){
 trace*t=(trace*)ctx;(void)o;(void)n;t->count++;t->last=flags;
}
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 kt_term_geometry_notifier ns[3],slots[3];kt_term_geometry_notifier_set set;
 trace ui={0,0},transport={0,0},diag={0,0};
 const uint8_t tel100[4]={0,100,0,40};
 memset(&screen,0,sizeof(screen));memset(slots,0,sizeof(slots));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);
 kt_term_geometry_notifier_init(&ns[0],cb,&ui);
 kt_term_geometry_notifier_set_filter(&ns[0],KT_TERM_GEOMETRY_TRANSITION_POLICY|
                                               KT_TERM_GEOMETRY_TRANSITION_SCREEN);
 kt_term_geometry_notifier_init(&ns[1],cb,&transport);
 kt_term_geometry_notifier_set_filter(&ns[1],KT_TERM_GEOMETRY_TRANSITION_REMOTE|
                                               KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP|
                                               KT_TERM_GEOMETRY_TRANSITION_DISCONNECT);
 kt_term_geometry_notifier_init(&ns[2],cb,&diag);
 kt_term_geometry_notifier_set_init(&set,slots,3);
 assert(kt_term_geometry_notifier_set_add(&set,&ns[0],0)==0);
 assert(kt_term_geometry_notifier_set_add(&set,&ns[1],0)==0);
 assert(kt_term_geometry_notifier_set_add(&set,&ns[2],0)==0);
 kt_term_geometry_session_bind_notifier_set(&gs,&set);

 assert(kt_term_geometry_session_connect_telnet(&gs,tel100,4)==0);
 assert(ui.count==1);              /* activation */
 assert(transport.count==1);       /* geometry/ownership */
 assert(diag.count==2);            /* both committed transitions */
 assert(ui.last==(KT_TERM_GEOMETRY_TRANSITION_POLICY|
                  KT_TERM_GEOMETRY_TRANSITION_SCREEN));
 assert(transport.last==(KT_TERM_GEOMETRY_TRANSITION_REMOTE|
                         KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP));

 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(ui.count==2);
 assert(transport.count==2);
 assert(diag.count==3);
 assert((transport.last&KT_TERM_GEOMETRY_TRANSITION_DISCONNECT)!=0);
 assert((diag.last&KT_TERM_GEOMETRY_TRANSITION_DISCONNECT)!=0);

 /* Empty caller-owned set is valid and suppresses fan-out. */
 kt_term_geometry_notifier_set_init(&set,0,3);
 assert(set.count==0);
 return 0;
}
