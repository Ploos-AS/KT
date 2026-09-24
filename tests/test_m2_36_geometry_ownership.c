#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 const uint8_t ssh100[4]={0,100,0,40};
 const uint8_t tel132[4]={0,132,0,43};
 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);
 assert(kt_term_geometry_session_ssh_pty(&gs,ssh100,4)==0);
 assert(g.remote_source==KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
 assert(kt_term_resize_set_policy(&session,&sched,KT_TERM_GEOMETRY_REMOTE,16,688,&result)==0);
 assert(screen.width==100&&screen.height==40);
 /* Telnet cannot steal SSH ownership. Failed event remains retryable. */
 assert(kt_term_geometry_session_telnet_naws(&gs,tel132,4)==-3);
 assert(g.remote_source==KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
 assert(g.remote_cols==100&&g.remote_rows==40);
 assert(!gs.events.valid[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]);
 /* Non-owner disconnect leaves SSH session intact. */
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(g.remote_source_valid&&g.remote_source==KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
 assert(screen.width==100&&screen.height==40);
 /* Owner disconnect falls back; Telnet can then acquire ownership. */
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_SSH_PTY)==0);
 assert(g.policy==KT_TERM_GEOMETRY_FIXED&&screen.width==80&&screen.height==25);
 assert(kt_term_geometry_session_telnet_naws(&gs,tel132,4)==0);
 assert(g.remote_source_valid&&g.remote_source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
 assert(g.remote_cols==132&&g.remote_rows==43);
 return 0;
}
