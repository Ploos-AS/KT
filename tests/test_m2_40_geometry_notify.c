#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
typedef struct trace { int count; kt_term_geometry_snapshot oldv,newv; uint32_t flags[8]; } trace;
static void on_change(void*ctx,const kt_term_geometry_snapshot*o,
                      const kt_term_geometry_snapshot*n,uint32_t flags){
 trace*t=(trace*)ctx;t->oldv=*o;t->newv=*n;t->flags[t->count]=flags;t->count++;
}
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 kt_term_geometry_notifier notifier;trace tr;kt_term_viewport vp;
 const uint8_t tel100[4]={0,100,0,40};
 const uint8_t bad[4]={0,0,0,40};
 memset(&screen,0,sizeof(screen));memset(&tr,0,sizeof(tr));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);
 kt_term_geometry_notifier_init(&notifier,on_change,&tr);
 kt_term_geometry_session_bind_notifier(&gs,&notifier);

 /* Connect intentionally exposes two committed transitions:
    remote geometry/ownership, then REMOTE policy activation. */
 assert(kt_term_geometry_session_connect_telnet(&gs,tel100,4)==0);
 assert(tr.count==2);
 assert(tr.flags[0]==(KT_TERM_GEOMETRY_TRANSITION_REMOTE|KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP));
 assert(tr.flags[1]==(KT_TERM_GEOMETRY_TRANSITION_POLICY|KT_TERM_GEOMETRY_TRANSITION_SCREEN));
 assert(tr.newv.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(tr.newv.screen_cols==100&&tr.newv.screen_rows==40);

 /* Duplicate update is a no-op and must not notify. */
 assert(kt_term_geometry_session_telnet_naws(&gs,tel100,4)==1);
 assert(tr.count==2);

 /* Invalid update fails before commit and must not notify. */
 assert(kt_term_geometry_session_telnet_naws(&gs,bad,4)<0);
 assert(tr.count==2);

 /* Disconnect is one committed transition. */
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(tr.count==3);
 assert(tr.flags[2]==(KT_TERM_GEOMETRY_TRANSITION_POLICY|KT_TERM_GEOMETRY_TRANSITION_SCREEN|KT_TERM_GEOMETRY_TRANSITION_REMOTE|KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP|KT_TERM_GEOMETRY_TRANSITION_DISCONNECT));
 assert(tr.oldv.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(tr.newv.policy==KT_TERM_GEOMETRY_FIXED);

 /* Local connect likewise reports geometry storage then policy activation. */
 assert(kt_term_viewport_init(&vp,640,480,8,16)==0);
 assert(kt_term_geometry_session_connect_viewport(&gs,&vp)==0);
 assert(tr.count==5);
 assert(tr.flags[3]==KT_TERM_GEOMETRY_TRANSITION_VIEWPORT);
 assert(tr.flags[4]==(KT_TERM_GEOMETRY_TRANSITION_POLICY|KT_TERM_GEOMETRY_TRANSITION_SCREEN));
 assert(tr.newv.policy==KT_TERM_GEOMETRY_VIEWPORT);
 assert(tr.newv.screen_cols==80&&tr.newv.screen_rows==30);
 return 0;
}
