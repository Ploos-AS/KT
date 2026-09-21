#include <assert.h>
#include <string.h>
#include "kt/terminal_resize.h"

int main(void){
 kt_term_geometry g;
 kt_term_screen screen;
 kt_term_session session;
 kt_term_cell cells[80*30],before[80*30];
 kt_term_present_scheduler sched,before_sched;
 kt_term_resize_result result;

 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,80*30)==0);
 cells[10].ch='X';

 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_geometry_set_fixed(&g,80,30)==0);
 assert(kt_term_resize_apply(&session,&sched,16,480,&result)==0);
 assert(result.changed&&result.old_cols==80&&result.old_rows==25);
 assert(result.new_cols==80&&result.new_rows==30);
 assert(result.dirty.valid&&result.dirty.first==0&&result.dirty.count==480);
 assert(screen.width==80&&screen.height==30&&cells[10].ch=='X');
 assert(sched.pending&&sched.first_row==0&&sched.count==480);

 before_sched=sched;
 assert(kt_term_resize_apply(&session,&sched,16,480,&result)==0);
 assert(!result.changed);
 assert(memcmp(&before_sched,&sched,sizeof(sched))==0);

 memcpy(before,cells,sizeof(cells));
 session.cell_capacity=80*25;
 before_sched=sched;
 assert(kt_term_geometry_set_fixed(&g,100,30)==0);
 assert(kt_term_resize_apply(&session,&sched,16,480,&result)==-3);
 assert(screen.width==80&&screen.height==30);
 assert(memcmp(before,cells,sizeof(cells))==0);
 assert(memcmp(&before_sched,&sched,sizeof(sched))==0);

 assert(kt_term_resize_apply(0,&sched,16,480,&result)==-1);
 assert(kt_term_resize_apply(&session,&sched,0,480,&result)==-1);
 assert(kt_term_resize_apply(&session,&sched,16,0,&result)==-1);
 {
  kt_term_geometry pg;
  kt_term_screen ps;
  kt_term_session ss;
  kt_term_cell pcells[132*43];
  kt_term_present_scheduler psched;
  kt_term_resize_result pr;
  memset(&ps,0,sizeof(ps));
  assert(kt_term_geometry_init(&pg,80,25)==0);
  assert(kt_term_session_init(&ss,&pg,&ps,pcells,80*25)==0);
  assert(kt_term_geometry_negotiate(&pg,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,132,43)==0);
  assert(pg.policy==KT_TERM_GEOMETRY_FIXED&&pg.cols==80&&pg.rows==25);
  kt_term_present_scheduler_reset(&psched);
  assert(kt_term_resize_set_policy(&ss,&psched,KT_TERM_GEOMETRY_REMOTE,16,688,&pr)==-3);
  assert(pg.policy==KT_TERM_GEOMETRY_FIXED&&pg.cols==80&&pg.rows==25);
  assert(pg.remote_valid&&pg.remote_cols==132&&pg.remote_rows==43);
  assert(ps.width==80&&ps.height==25);
  assert(!psched.pending);

  ss.cell_capacity=132*43;
  assert(kt_term_resize_set_policy(&ss,&psched,KT_TERM_GEOMETRY_REMOTE,16,688,&pr)==0);
  assert(pg.policy==KT_TERM_GEOMETRY_REMOTE&&pg.cols==132&&pg.rows==43);
  assert(ps.width==132&&ps.height==43);
  assert(pr.changed&&pr.old_cols==80&&pr.old_rows==25&&pr.new_cols==132&&pr.new_rows==43);
  assert(pr.dirty.valid&&pr.dirty.first==0&&pr.dirty.count==688);
  assert(psched.pending&&psched.first_row==0&&psched.count==688);
 }
 {
  kt_term_geometry ng;
  kt_term_screen ns;
  kt_term_session nss;
  kt_term_cell ncells[132*43],snap[132*43];
  kt_term_present_scheduler nsched,sched_snap;
  kt_term_resize_result nr;
  memset(&ns,0,sizeof(ns));
  assert(kt_term_geometry_init(&ng,80,25)==0);
  assert(kt_term_session_init(&nss,&ng,&ns,ncells,80*25)==0);
  assert(kt_term_geometry_negotiate(&ng,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,80,25)==0);
  assert(kt_term_geometry_set_policy(&ng,KT_TERM_GEOMETRY_REMOTE)==0);
  ncells[7].ch='N';
  kt_term_present_scheduler_reset(&nsched);
  memcpy(snap,ncells,sizeof(ncells));sched_snap=nsched;

  assert(kt_term_resize_negotiate(&nss,&nsched,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,
                                  132,43,16,688,&nr)==-3);
  assert(ng.policy==KT_TERM_GEOMETRY_REMOTE&&ng.cols==80&&ng.rows==25);
  assert(ng.remote_valid&&ng.remote_cols==80&&ng.remote_rows==25);
  assert(ng.remote_source_valid&&ng.remote_source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
  assert(ns.width==80&&ns.height==25&&ncells[7].ch=='N');
  assert(memcmp(snap,ncells,sizeof(ncells))==0);
  assert(memcmp(&sched_snap,&nsched,sizeof(nsched))==0);

  nss.cell_capacity=132*43;
  assert(kt_term_resize_negotiate(&nss,&nsched,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,
                                  132,43,16,688,&nr)==0);
  assert(ng.cols==132&&ng.rows==43&&ng.remote_cols==132&&ng.remote_rows==43);
  assert(ns.width==132&&ns.height==43&&ncells[7].ch=='N');
  assert(nr.changed&&nr.dirty.valid&&nr.dirty.count==688);
  assert(nsched.pending&&nsched.first_row==0&&nsched.count==688);
 }
 return 0;
}
