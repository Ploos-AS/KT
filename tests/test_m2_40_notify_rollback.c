#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
typedef struct trace { int count; } trace;
static void cb(void*ctx,const kt_term_geometry_snapshot*o,
               const kt_term_geometry_snapshot*n,uint32_t flags,uint32_t sequence){
 trace*t=(trace*)ctx;(void)o;(void)n;(void)flags;(void)sequence;t->count++;
}
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 kt_term_geometry_notifier n;trace tr;
 const uint8_t tel132[4]={0,132,0,43};
 memset(&screen,0,sizeof(screen));memset(&tr,0,sizeof(tr));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,80u*25u)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);
 kt_term_geometry_notifier_init(&n,cb,&tr);
 kt_term_geometry_session_bind_notifier(&gs,&n);

 /* Geometry can commit while FIXED remains active: one notification. */
 assert(kt_term_geometry_session_telnet_naws(&gs,tel132,4)==0);
 assert(tr.count==1);
 assert(g.remote_valid&&g.policy==KT_TERM_GEOMETRY_FIXED);

 /* REMOTE activation fails capacity preflight: no notification. */
 assert(kt_term_geometry_session_activate_remote(&gs,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==-3);
 assert(tr.count==1);
 assert(g.policy==KT_TERM_GEOMETRY_FIXED);
 assert(screen.width==80&&screen.height==25);

 /* After capacity is available the same activation commits once. */
 session.cell_capacity=132u*43u;
 assert(kt_term_geometry_session_activate_remote(&gs,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(tr.count==2);
 assert(g.policy==KT_TERM_GEOMETRY_REMOTE);

 /* Wrong owner fails without notification. */
 assert(kt_term_geometry_session_activate_remote(&gs,
        KT_TERM_GEOMETRY_SOURCE_SSH_PTY)==-2);
 assert(tr.count==2);
 return 0;
}
