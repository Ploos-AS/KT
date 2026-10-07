#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
typedef struct trace { int count; uint32_t last; } trace;
static void cb(void*ctx,const kt_term_geometry_snapshot*o,
               const kt_term_geometry_snapshot*n,uint32_t flags,uint32_t sequence){
 trace*t=(trace*)ctx;(void)o;(void)n;t->count++;t->last=flags;
}
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 kt_term_geometry_notifier n;trace tr;const uint8_t tel100[4]={0,100,0,40};
 memset(&screen,0,sizeof(screen));memset(&tr,0,sizeof(tr));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);
 kt_term_geometry_notifier_init(&n,cb,&tr);
 kt_term_geometry_session_bind_notifier(&gs,&n);

 /* UI subscription ignores ownership storage, receives policy/screen activation. */
 kt_term_geometry_notifier_set_filter(&n,KT_TERM_GEOMETRY_TRANSITION_POLICY|
                                         KT_TERM_GEOMETRY_TRANSITION_SCREEN);
 assert(kt_term_geometry_session_connect_telnet(&gs,tel100,4)==0);
 assert(tr.count==1);
 assert(tr.last==(KT_TERM_GEOMETRY_TRANSITION_POLICY|
                  KT_TERM_GEOMETRY_TRANSITION_SCREEN));

 /* Transport subscription receives disconnect because it carries ownership. */
 tr.count=0;
 kt_term_geometry_notifier_set_filter(&n,KT_TERM_GEOMETRY_TRANSITION_REMOTE|
                                         KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP|
                                         KT_TERM_GEOMETRY_TRANSITION_DISCONNECT);
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(tr.count==1);
 assert((tr.last&KT_TERM_GEOMETRY_TRANSITION_DISCONNECT)!=0);
 assert((tr.last&KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP)!=0);

 /* Zero mask suppresses all callbacks without suppressing commits. */
 tr.count=0;kt_term_geometry_notifier_set_filter(&n,0);
 assert(kt_term_geometry_session_connect_telnet(&gs,tel100,4)==0);
 assert(tr.count==0);
 assert(g.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(screen.width==100&&screen.height==40);

 /* Default init restores all-transitions behavior. */
 kt_term_geometry_notifier_init(&n,cb,&tr);
 assert(n.filter==0xffffffffu);
 return 0;
}
