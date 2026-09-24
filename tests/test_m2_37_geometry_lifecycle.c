#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"

int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 const uint8_t n100[4]={0,100,0,30};
 const uint8_t n132[4]={0,132,0,43};
 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);

 /* Connect is one lifecycle operation: ingest, own, activate, resize. */
 assert(kt_term_geometry_session_connect_telnet(&gs,n100,4)==0);
 assert(g.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(g.remote_source_valid&&g.remote_source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
 assert(screen.width==100&&screen.height==30);
 cells[7].ch='L';

 /* Live update while active REMOTE resizes through the same transaction path. */
 assert(kt_term_geometry_session_telnet_naws(&gs,n132,4)==0);
 assert(screen.width==132&&screen.height==43);
 assert(cells[7].ch=='L');

 /* Disconnect returns atomically to fixed geometry. */
 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(g.policy==KT_TERM_GEOMETRY_FIXED);
 assert(screen.width==80&&screen.height==25);
 assert(cells[7].ch=='L');

 /* Capacity failure during connect must not leave ownership or REMOTE policy. */
 session.cell_capacity=80u*25u;
 kt_term_geometry_event_state_reset(&gs.events);
 assert(kt_term_geometry_session_connect_telnet(&gs,n132,4)==-3);
 assert(g.policy==KT_TERM_GEOMETRY_FIXED);
 assert(!g.remote_valid&&!g.remote_source_valid);
 assert(screen.width==80&&screen.height==25);
 assert(!gs.events.valid[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]);

 /* Same input remains retryable after capacity becomes available. */
 session.cell_capacity=132u*43u;
 assert(kt_term_geometry_session_connect_telnet(&gs,n132,4)==0);
 assert(g.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(screen.width==132&&screen.height==43);
 return 0;
}
