#include <assert.h>
#include <string.h>
#include "kt/terminal_session.h"

typedef struct draw_state { unsigned cells; } draw_state;
static void draw(void *ctx,const kt_term_render_cell *cell){draw_state*s=(draw_state*)ctx;(void)cell;s->cells++;}

int main(void){
 kt_term_geometry g;
 kt_term_screen screen;
 kt_term_session session;
 kt_term_cell cells[100*30],cache_cells[100*30];
 kt_term_render_cache cache;
 kt_term_renderer_ops ops;
 kt_term_render_damage damage;
 draw_state d;
 memset(&screen,0,sizeof(screen));memset(&cache,0,sizeof(cache));memset(&ops,0,sizeof(ops));
 ops.draw_cell=draw;cache.cells=cache_cells;cache.cell_count=100*30;
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,100*30)==0);
 kt_term_session_bind_render_cache(&session,&cache);
 d.cells=0;
 assert(kt_term_render_incremental_damage(&screen,KT_TERM_PROFILE_ANSI,&cache,&ops,&d,&damage)==0);
 assert(damage.full&&d.cells==80u*25u&&cache.valid);

 d.cells=0;
 assert(kt_term_render_incremental_damage(&screen,KT_TERM_PROFILE_ANSI,&cache,&ops,&d,&damage)==0);
 assert(!damage.full&&!damage.cells_changed&&d.cells==0);

 assert(kt_term_geometry_set_fixed(&g,80,30)==0);
 assert(kt_term_session_apply_geometry(&session)==0);
 assert(!cache.valid);
 d.cells=0;
 assert(kt_term_render_incremental_damage(&screen,KT_TERM_PROFILE_ANSI,&cache,&ops,&d,&damage)==0);
 assert(damage.full&&damage.cells_changed&&d.cells==80u*30u&&cache.valid);

 assert(kt_term_geometry_set_fixed(&g,100,30)==0);
 assert(kt_term_session_apply_geometry(&session)==0);
 assert(!cache.valid);
 d.cells=0;
 assert(kt_term_render_incremental_damage(&screen,KT_TERM_PROFILE_ANSI,&cache,&ops,&d,&damage)==0);
 assert(damage.full&&d.cells==100u*30u&&cache.valid);
 return 0;
}
