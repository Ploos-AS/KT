#include "kt/terminal_renderer.h"
#include "kt/terminal_dirty.h"
#include "kt/terminal_display.h"
#include <assert.h>
#include <string.h>
typedef struct S{unsigned draws,presents;uint16_t first,count;}S;
static void draw(void*p,const kt_term_render_cell*c){(void)c;((S*)p)->draws++;}
static int rows(void*p,const kt_term_display_frame*f,uint16_t first,uint16_t count){S*s=p;(void)f;s->presents++;s->first=first;s->count=count;return 0;}
int main(void){
 kt_term_cell cells[4],cache_cells[4];kt_term_screen scr;kt_term_render_cache cache;kt_term_render_damage dmg;kt_term_dirty_rows dirty;
 kt_term_renderer_ops ro={0,draw,0,0};kt_term_display_ops dop={0,rows,0};uint8_t pixels[16*16];kt_term_display_frame frame={pixels,16,16,16,KT_TERM_DISPLAY_INDEXED8,0};S s;
 memset(&cache,0,sizeof cache);cache.cells=cache_cells;cache.cell_count=4;memset(&s,0,sizeof s);
 assert(kt_term_screen_init(&scr,2,2,cells,4)==0);
 assert(kt_term_render_incremental_damage(&scr,KT_TERM_PROFILE_ANSI,&cache,&ro,&s,&dmg)==0);
 assert(dmg.full&&s.draws==4);assert(kt_term_dirty_rows_from_damage(&dirty,&dmg,8,16)==0&&dirty.first==0&&dirty.count==16);
 assert(kt_term_display_present_rows(&dop,&s,&frame,dirty.first,dirty.count)==0&&s.presents==1);
 memset(&s,0,sizeof s);assert(kt_term_render_incremental_damage(&scr,KT_TERM_PROFILE_ANSI,&cache,&ro,&s,&dmg)==0);
 assert(!dmg.full&&!dmg.cells_changed&&!dmg.cursor_changed&&s.draws==0);assert(kt_term_dirty_rows_from_damage(&dirty,&dmg,8,16)==1&&!dirty.valid&&s.presents==0);
 scr.cells[3].ch='X';assert(kt_term_render_incremental_damage(&scr,KT_TERM_PROFILE_ANSI,&cache,&ro,&s,&dmg)==0);
 assert(dmg.cells_changed&&dmg.first_row==1&&dmg.row_count==1&&s.draws==1);
 assert(kt_term_dirty_rows_from_damage(&dirty,&dmg,8,16)==0&&dirty.first==8&&dirty.count==8);
 assert(kt_term_display_present_rows(&dop,&s,&frame,dirty.first,dirty.count)==0&&s.presents==1&&s.first==8&&s.count==8);
 return 0;
}
