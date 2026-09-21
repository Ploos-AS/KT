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
 assert(!g.remote_valid&&g.cols==80&&g.rows==25);
 assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_REMOTE)==-2&&g.policy==KT_TERM_GEOMETRY_REMOTE);
 /* Policy was already REMOTE; unavailable remote geometry leaves fallback active. */
 assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_FIXED)==0&&g.cols==80&&g.rows==25);
 assert(kt_term_geometry_set_fixed(&g,132,25)==0&&g.cols==132&&g.rows==25);
 assert(kt_term_geometry_set_policy(&g,KT_TERM_GEOMETRY_VIEWPORT)==0&&g.cols==80&&g.rows==30);
 assert(kt_term_geometry_set_viewport(&g,0,30)==-1&&g.cols==80&&g.rows==30);
 { kt_term_geometry old=g;
   assert(kt_term_geometry_set_policy(&g,(kt_term_geometry_policy)99)==-2);
   assert(g.policy==old.policy&&g.cols==old.cols&&g.rows==old.rows);
 }
 assert(kt_term_geometry_init(&g,0,25)==-1);
 return 0;
}
