#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"

int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 const uint8_t naws132[4]={0,132,0,43};

 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);

 /* Store Telnet geometry, then activate REMOTE policy. */
 assert(kt_term_geometry_session_telnet_naws(&gs,naws132,4)==0);
 assert(g.remote_valid&&g.remote_source_valid);
 assert(g.remote_source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
 assert(g.remote_cols==132&&g.remote_rows==43);
 assert(screen.width==80&&screen.height==25);
 assert(kt_term_resize_set_policy(&session,&sched,KT_TERM_GEOMETRY_REMOTE,16,688,&result)==0);
 assert(screen.width==132&&screen.height==43);
 cells[5].ch='R';

 /* Owner disconnect falls back transactionally to FIXED. */
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(g.policy==KT_TERM_GEOMETRY_FIXED);
 assert(!g.remote_valid&&!g.remote_source_valid);
 assert(screen.width==80&&screen.height==25);
 assert(cells[5].ch=='R');
 assert(!gs.events.valid[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]);
 assert(sched.pending&&sched.count==688);

 /* Same NAWS must be accepted after reconnect, not suppressed as duplicate. */
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_telnet_naws(&gs,naws132,4)==0);
 assert(g.remote_valid&&g.remote_source_valid);
 assert(g.remote_source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
 assert(g.remote_cols==132&&g.remote_rows==43);
 assert(gs.events.valid[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]);
 assert(kt_term_geometry_session_telnet_naws(&gs,naws132,4)==1);

 /* Non-owner SSH disconnect is harmless; LOCAL is not a remote transport. */
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_SSH_PTY)==0);
 assert(g.remote_valid&&g.remote_source_valid);
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_LOCAL)==-1);
 return 0;
}
