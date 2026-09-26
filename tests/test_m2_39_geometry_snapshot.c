#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;
 kt_term_cell cells[132*43];kt_term_present_scheduler sched;
 kt_term_resize_result result;kt_term_geometry_session gs;
 kt_term_geometry_snapshot snap;kt_term_viewport vp;
 const uint8_t tel100[4]={0,100,0,40};
 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,132*43)==0);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_session_init(&gs,&session,&sched,16,688,&result)==0);

 assert(kt_term_geometry_session_snapshot(&gs,&snap)==0);
 assert(snap.policy==KT_TERM_GEOMETRY_FIXED);
 assert(snap.screen_cols==80&&snap.screen_rows==25);
 assert(!snap.remote_valid&&!snap.remote_source_valid);

 assert(kt_term_viewport_init(&vp,640,480,8,16)==0);
 assert(kt_term_geometry_session_connect_viewport(&gs,&vp)==0);
 assert(kt_term_geometry_session_snapshot(&gs,&snap)==0);
 assert(snap.policy==KT_TERM_GEOMETRY_VIEWPORT);
 assert(snap.screen_cols==80&&snap.screen_rows==30);
 assert(snap.viewport_cols==80&&snap.viewport_rows==30);

 assert(kt_term_geometry_session_connect_telnet(&gs,tel100,4)==0);
 assert(kt_term_geometry_session_snapshot(&gs,&snap)==0);
 assert(snap.policy==KT_TERM_GEOMETRY_REMOTE);
 assert(snap.screen_cols==100&&snap.screen_rows==40);
 assert(snap.viewport_cols==80&&snap.viewport_rows==30);
 assert(snap.remote_valid&&snap.remote_source_valid);
 assert(snap.remote_source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
 assert(snap.remote_cols==100&&snap.remote_rows==40);

 assert(kt_term_geometry_session_disconnect(&gs,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS)==0);
 assert(kt_term_geometry_session_snapshot(&gs,&snap)==0);
 assert(snap.policy==KT_TERM_GEOMETRY_FIXED);
 assert(snap.screen_cols==80&&snap.screen_rows==25);
 assert(!snap.remote_valid&&!snap.remote_source_valid);
 assert(snap.viewport_cols==80&&snap.viewport_rows==30);

 assert(kt_term_geometry_session_snapshot((const kt_term_geometry_session*)0,&snap)==-1);
 assert(kt_term_geometry_session_snapshot(&gs,(kt_term_geometry_snapshot*)0)==-1);
 return 0;
}
