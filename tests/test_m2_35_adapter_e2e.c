#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_adapter.h"

int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_event_state events;
 kt_term_geometry_resize_adapter adapter;const kt_term_geometry_event_ops *ops;
 const uint8_t naws80[4]={0,80,0,25},naws132[4]={0,132,0,43};
 kt_term_viewport v;
 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,80*25)==0);
 kt_term_present_scheduler_reset(&sched);
 kt_term_geometry_event_state_reset(&events);
 kt_term_geometry_resize_adapter_init(&adapter,&session,&sched,16,688,&result);
 ops=kt_term_geometry_resize_adapter_ops();

 assert(kt_term_geometry_from_telnet_naws(&events,ops,&adapter,naws80,4)==0);
 assert(kt_term_resize_set_policy(&session,&sched,KT_TERM_GEOMETRY_REMOTE,16,688,&result)==0);
 kt_term_present_scheduler_reset(&sched);
 cells[7].ch='T';
 assert(kt_term_geometry_from_telnet_naws(&events,ops,&adapter,naws132,4)==-3);
 assert(screen.width==80&&screen.height==25&&cells[7].ch=='T');
 session.cell_capacity=132*43;
 assert(kt_term_geometry_from_telnet_naws(&events,ops,&adapter,naws132,4)==0);
 assert(screen.width==132&&screen.height==43&&cells[7].ch=='T');
 assert(sched.pending&&sched.count==688);

 /* Release remote ownership, switch to viewport policy and propagate LOCAL. */
 kt_term_geometry_release_remote(&g,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
 assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_VIEWPORT)==0);
 assert(kt_term_viewport_init(&v,640,480,8,16)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_from_viewport(&events,ops,&adapter,&v)==0);
 assert(g.viewport_cols==80&&g.viewport_rows==30);
 assert(screen.width==80&&screen.height==30);
 assert(sched.pending&&sched.count==688);
 return 0;
}
