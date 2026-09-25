#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;kt_term_viewport vp;
 const uint8_t ssh100[4]={0,100,0,40};
 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);
 assert(kt_term_viewport_init(&vp,640,480,8,16)==0);
 assert(kt_term_geometry_session_connect_viewport(&gs,&vp)==0);
 assert(g.policy==KT_TERM_GEOMETRY_VIEWPORT&&screen.width==80&&screen.height==30);

 /* Remote ownership/policy may supersede local presentation geometry. */
 assert(kt_term_geometry_session_connect_ssh(&gs,ssh100,4)==0);
 assert(g.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(g.remote_source==KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
 assert(screen.width==100&&screen.height==40);

 /* Local updates are remembered while REMOTE remains authoritative. */
 assert(kt_term_viewport_resize(&vp,720,480)==0); /* 90x30 */
 assert(kt_term_geometry_session_viewport(&gs,&vp)==0);
 assert(g.viewport_cols==90&&g.viewport_rows==30);
 assert(g.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(screen.width==100&&screen.height==40);

 /* Explicit local activation switches policy without disturbing ownership. */
 assert(kt_term_geometry_session_activate_viewport(&gs)==0);
 assert(g.policy==KT_TERM_GEOMETRY_VIEWPORT);
 assert(screen.width==90&&screen.height==30);
 assert(g.remote_source_valid&&g.remote_source==KT_TERM_GEOMETRY_SOURCE_SSH_PTY);

 /* Remote can be explicitly reactivated while it still owns geometry. */
 assert(kt_term_geometry_session_activate_remote(&gs,KT_TERM_GEOMETRY_SOURCE_SSH_PTY)==0);
 assert(g.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(screen.width==100&&screen.height==40);
 assert(kt_term_geometry_session_activate_remote(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==-2);
 assert(screen.width==100&&screen.height==40);
 return 0;
}
