#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_event.h"

int main(void){
 kt_term_geometry g;
 kt_term_screen screen;
 kt_term_session session;
 kt_term_cell cells[132*43];
 kt_term_present_scheduler sched;
 kt_term_resize_result result;
 kt_term_geometry_event_state events;
 kt_term_geometry_resize_adapter adapter;
 const kt_term_geometry_event_ops *ops;

 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,80*25)==0);
 kt_term_present_scheduler_reset(&sched);
 kt_term_geometry_event_state_reset(&events);
 kt_term_geometry_resize_adapter_init(&adapter,&session,&sched,16,688,&result);
 ops=kt_term_geometry_resize_adapter_ops();

 /* FIXED policy stores remote geometry without changing the active screen. */
 assert(kt_term_geometry_event_emit(&events,ops,&adapter,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,80,25)==0);
 assert(g.remote_valid&&g.remote_cols==80&&g.remote_rows==25);
 assert(g.cols==80&&g.rows==25&&screen.width==80&&screen.height==25);

 assert(kt_term_resize_set_policy(&session,&sched,KT_TERM_GEOMETRY_REMOTE,
                                  16,688,&result)==0);
 assert(g.policy==KT_TERM_GEOMETRY_REMOTE);
 kt_term_present_scheduler_reset(&sched);
 cells[9].ch='E';

 /* Event callback reaches M2.33, fails capacity, and must not be consumed. */
 assert(kt_term_geometry_event_emit(&events,ops,&adapter,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,132,43)==-3);
 assert(g.cols==80&&g.rows==25&&g.remote_cols==80&&g.remote_rows==25);
 assert(screen.width==80&&screen.height==25&&cells[9].ch=='E');
 assert(events.cols[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==80);
 assert(events.rows[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==25);
 assert(!sched.pending);

 /* Same event is retryable after caller supplies sufficient storage. */
 session.cell_capacity=132*43;
 assert(kt_term_geometry_event_emit(&events,ops,&adapter,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,132,43)==0);
 assert(g.cols==132&&g.rows==43&&g.remote_cols==132&&g.remote_rows==43);
 assert(screen.width==132&&screen.height==43&&cells[9].ch=='E');
 assert(events.cols[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==132);
 assert(events.rows[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==43);
 assert(result.changed&&result.dirty.valid&&result.dirty.count==688);
 assert(sched.pending&&sched.first_row==0&&sched.count==688);

 /* Successful event is now suppressed as a duplicate. */
 assert(kt_term_geometry_event_emit(&events,ops,&adapter,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,132,43)==1);
 return 0;
}
