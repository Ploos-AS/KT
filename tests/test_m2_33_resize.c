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
 return 0;
}
