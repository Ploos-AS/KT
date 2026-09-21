#include "kt/terminal_geometry.h"
#include <assert.h>
int main(void){
 kt_term_geometry g;
 assert(kt_term_geometry_init(&g,80,25)==0&&g.cols==80&&g.rows==25&&g.policy==KT_TERM_GEOMETRY_FIXED);
 assert(kt_term_geometry_set_viewport(&g,80,30)==0&&g.cols==80&&g.rows==25);
 assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_VIEWPORT)==0&&g.cols==80&&g.rows==30);
 assert(kt_term_geometry_set_remote(&g,132,43)==0&&g.cols==80&&g.rows==30&&g.remote_valid);
 assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_REMOTE)==0&&g.cols==132&&g.rows==43);
 assert(kt_term_geometry_set_remote(&g,100,40)==0&&g.cols==100&&g.rows==40);
 kt_term_geometry_clear_remote(&g);
 assert(!g.remote_valid&&g.cols==80&&g.rows==25&&g.policy==KT_TERM_GEOMETRY_FIXED);
 assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_REMOTE)==-2&&g.policy==KT_TERM_GEOMETRY_FIXED);
 assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_FIXED)==0&&g.cols==80&&g.rows==25);
 assert(kt_term_geometry_set_fixed(&g,132,25)==0&&g.cols==132&&g.rows==25);
 assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_VIEWPORT)==0&&g.cols==80&&g.rows==30);
 assert(kt_term_geometry_set_viewport(&g,0,30)==-1&&g.cols==80&&g.rows==30);
 { kt_term_geometry old=g;
   assert(kt_term_geometry_set_policy(&g,(kt_term_geometry_policy)99)==-2);
   assert(g.policy==old.policy&&g.cols==old.cols&&g.rows==old.rows);
 }
 assert(kt_term_geometry_init(&g,0,25)==-1);
 { kt_term_viewport v;
   assert(kt_term_viewport_init(&v,640,400,8,16)==0&&v.cols==80&&v.rows==25);
   assert(kt_term_geometry_init(&g,80,25)==0);
   assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_VIEWPORT)==0);
   assert(kt_term_geometry_from_viewport(&g,&v)==0&&g.cols==80&&g.rows==25);
   assert(kt_term_viewport_resize(&v,640,480)==0&&v.cols==80&&v.rows==30);
   assert(kt_term_geometry_from_viewport(&g,&v)==0&&g.cols==80&&g.rows==30);
   assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_FIXED)==0&&g.cols==80&&g.rows==25);
   assert(kt_term_viewport_resize(&v,800,600)==0);
   assert(kt_term_geometry_from_viewport(&g,&v)==0&&g.cols==80&&g.rows==25);
   assert(g.viewport_cols==100&&g.viewport_rows==37);
 }
 { kt_term_geometry n;
   assert(kt_term_geometry_init(&n,80,25)==0);
   assert(kt_term_geometry_negotiate(&n,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,80,25)==0);
   assert(n.remote_valid&&n.remote_cols==80&&n.remote_rows==25&&n.cols==80&&n.rows==25);
   assert(kt_term_geometry_set_policy(&n,KT_TERM_GEOMETRY_REMOTE)==0);
   assert(kt_term_geometry_negotiate(&n,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,132,43)==0);
   assert(n.cols==132&&n.rows==43);
   assert(kt_term_geometry_negotiate(&n,KT_TERM_GEOMETRY_SOURCE_SSH_PTY,100,40)==0);
   assert(n.cols==100&&n.rows==40);
   assert(kt_term_geometry_set_policy(&n,KT_TERM_GEOMETRY_FIXED)==0);
   assert(kt_term_geometry_negotiate(&n,KT_TERM_GEOMETRY_SOURCE_LOCAL,120,37)==0);
   assert(n.viewport_cols==120&&n.viewport_rows==37&&n.cols==80&&n.rows==25);
   assert(kt_term_geometry_set_policy(&n,KT_TERM_GEOMETRY_VIEWPORT)==0&&n.cols==120&&n.rows==37);
   assert(kt_term_geometry_negotiate(&n,(kt_term_geometry_source)99,80,25)==-2);
   assert(kt_term_geometry_negotiate(&n,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,0,25)==-1);
   assert(kt_term_geometry_negotiate(&n,KT_TERM_GEOMETRY_SOURCE_SSH_PTY,80,0)==-1);
 }
 { kt_term_geometry o;
   assert(kt_term_geometry_init(&o,80,25)==0);
   assert(kt_term_geometry_negotiate(&o,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,132,43)==0);
   assert(o.remote_source_valid&&o.remote_source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
   assert(kt_term_geometry_negotiate(&o,KT_TERM_GEOMETRY_SOURCE_SSH_PTY,100,40)==-3);
   assert(o.remote_cols==132&&o.remote_rows==43);
   kt_term_geometry_release_remote(&o,KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
   assert(o.remote_valid&&o.remote_source_valid);
   kt_term_geometry_release_remote(&o,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
   assert(!o.remote_valid&&!o.remote_source_valid);
   assert(kt_term_geometry_negotiate(&o,KT_TERM_GEOMETRY_SOURCE_SSH_PTY,100,40)==0);
   assert(o.remote_source==KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
   assert(kt_term_geometry_negotiate(&o,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,80,24)==-3);
   assert(kt_term_geometry_set_policy(&o,KT_TERM_GEOMETRY_REMOTE)==0);
   assert(o.cols==100&&o.rows==40);
   kt_term_geometry_release_remote(&o,KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
   assert(o.policy==KT_TERM_GEOMETRY_FIXED&&o.cols==80&&o.rows==25);
   assert(!o.remote_valid&&!o.remote_source_valid);
 }
 return 0;
}
