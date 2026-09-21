#include <assert.h>
#include <string.h>
#include "kt/terminal_session.h"
#include "kt/terminal_dirty.h"
#include "kt/terminal_scheduler.h"

typedef struct draw_state { unsigned cells; } draw_state;
static void draw(void *ctx,const kt_term_render_cell *cell){draw_state*s=(draw_state*)ctx;(void)cell;s->cells++;}

int main(void){
 kt_term_geometry g;
 kt_term_screen screen;
 kt_term_session session;
 kt_term_cell cells[80*30],cache_cells[80*30];
 kt_term_render_cache cache;
 kt_term_renderer_ops ops;
 kt_term_render_damage damage;
 kt_term_dirty_rows dirty;
 kt_term_present_scheduler sched;
 draw_state d;

 memset(&screen,0,sizeof(screen));memset(&cache,0,sizeof(cache));memset(&ops,0,sizeof(ops));
 ops.draw_cell=draw;cache.cells=cache_cells;cache.cell_count=80*30;
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,80*30)==0);
 kt_term_session_bind_render_cache(&session,&cache);
 d.cells=0;
 assert(kt_term_render_incremental_damage(&screen,KT_TERM_PROFILE_ANSI,&cache,&ops,&d,&damage)==0);
 assert(damage.full);

 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_dirty_rows_from_damage(&damage,16,480,&dirty)==0);
 assert(dirty.valid&&dirty.first==0&&dirty.count==400);
 assert(kt_term_present_scheduler_add_rows(&sched,dirty.first,dirty.count)==0);
 assert(sched.pending);

 assert(kt_term_geometry_set_fixed(&g,80,30)==0);
 assert(kt_term_session_apply_geometry(&session)==0);
 d.cells=0;
 assert(kt_term_render_incremental_damage(&screen,KT_TERM_PROFILE_ANSI,&cache,&ops,&d,&damage)==0);
 assert(damage.full&&d.cells==80u*30u);
 assert(kt_term_dirty_rows_from_damage(&damage,16,480,&dirty)==0);
 assert(dirty.valid&&dirty.first==0&&dirty.count==480);
 kt_term_present_scheduler_reset(&sched);
 assert(kt_term_present_scheduler_add_rows(&sched,dirty.first,dirty.count)==0);
 assert(sched.pending&&sched.first_row==0&&sched.count==480);
 return 0;
}
