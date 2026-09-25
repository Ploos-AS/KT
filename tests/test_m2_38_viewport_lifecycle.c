#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 kt_term_viewport vp;
 const uint8_t tel100[4]={0,100,0,40};
 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);
 assert(kt_term_viewport_init(&vp,640,400,8,16)==0);
 assert(kt_term_geometry_session_connect_viewport(&gs,&vp)==0);
 assert(g.policy==KT_TERM_GEOMETRY_VIEWPORT);
 assert(screen.width==80&&screen.height==25);
 cells[9].ch='V';
 assert(kt_term_viewport_resize(&vp,640,480)==0);
 assert(kt_term_geometry_session_viewport(&gs,&vp)==0);
 assert(screen.width==80&&screen.height==30);
 assert(cells[9].ch=='V');

 /* Remote transport may take ownership and activate REMOTE explicitly. */
 assert(kt_term_geometry_session_connect_telnet(&gs,tel100,4)==0);
 assert(g.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(screen.width==100&&screen.height==40);
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(g.policy==KT_TERM_GEOMETRY_FIXED);
 assert(screen.width==80&&screen.height==25);
 /* Stored local viewport remains available for deterministic reactivation. */
 assert(kt_term_geometry_session_activate_viewport(&gs)==0);
 assert(g.policy==KT_TERM_GEOMETRY_VIEWPORT);
 assert(screen.width==80&&screen.height==30);

 /* Local resize failure remains retryable. */
 session.cell_capacity=80u*30u;
 assert(kt_term_viewport_resize(&vp,800,640)==0); /* 100x40 */
 assert(kt_term_geometry_session_viewport(&gs,&vp)==-3);
 assert(screen.width==80&&screen.height==30);
 assert(g.viewport_cols==80&&g.viewport_rows==30);
 assert(!gs.events.valid[KT_TERM_GEOMETRY_SOURCE_LOCAL] ||
        gs.events.cols[KT_TERM_GEOMETRY_SOURCE_LOCAL]==80);
 session.cell_capacity=132u*43u;
 assert(kt_term_geometry_session_viewport(&gs,&vp)==0);
 assert(screen.width==100&&screen.height==40);
 return 0;
}
