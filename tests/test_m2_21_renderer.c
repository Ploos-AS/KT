#include "kt/terminal_renderer.h"
#include <assert.h>
#include <string.h>
typedef struct sink { unsigned begin,draw,cursor,end; kt_term_render_cell cells[4]; uint16_t cx,cy; } sink;
static void begin(void*p,uint16_t w,uint16_t h){sink*s=p;s->begin++;assert(w==2&&h==2);}
static void draw(void*p,const kt_term_render_cell*c){sink*s=p;s->cells[s->draw++]=*c;}
static void cursor(void*p,uint16_t x,uint16_t y){sink*s=p;s->cursor++;s->cx=x;s->cy=y;}
static void end(void*p){((sink*)p)->end++;}
int main(void){
 kt_term_cell cells[4];kt_term_screen scr;kt_term t;sink out;kt_term_renderer_ops ops={begin,draw,cursor,end};
 static const uint8_t v[]={0x41u,0x0Eu,0x41u};
 memset(&out,0,sizeof out);assert(kt_term_screen_init(&scr,2,2,cells,4)==0);
 kt_term_init(&t,kt_term_screen_ops(),&scr);kt_term_set_profile(&t,KT_TERM_PROFILE_PETSCII);kt_term_feed(&t,v,sizeof v);
 assert(kt_term_render(&scr,KT_TERM_PROFILE_PETSCII,&ops,&out)==0);
 assert(out.begin==1&&out.draw==4&&out.cursor==1&&out.end==1);
 assert(out.cells[0].glyph.codepoint==0x41u);
 assert(out.cells[1].glyph.codepoint==0x61u);
 assert(out.cells[1].glyph.source_byte==0x41u);
 assert(out.cells[1].glyph.charset==KT_TERM_PETSCII_LOWER_UPPER);
 assert(out.cx==scr.x&&out.cy==scr.y);
 assert(out.cells[0].x==0u&&out.cells[0].y==0u);
 assert(out.cells[1].x==1u&&out.cells[1].y==0u);
 assert(out.cells[2].x==0u&&out.cells[2].y==1u);
 assert(out.cells[3].x==1u&&out.cells[3].y==1u);
 assert(kt_term_render(0,KT_TERM_PROFILE_PETSCII,&ops,&out)==-1);
 assert(kt_term_render(&scr,KT_TERM_PROFILE_PETSCII,0,&out)==-1);
 { kt_term_renderer_ops bad={0,0,0,0};
   assert(kt_term_render(&scr,KT_TERM_PROFILE_PETSCII,&bad,&out)==-1);
 }
 { kt_term_renderer_ops minimal={0,draw,0,0}; sink m;
   memset(&m,0,sizeof m);
   assert(kt_term_render(&scr,KT_TERM_PROFILE_PETSCII,&minimal,&m)==0);
   assert(m.draw==4u&&m.begin==0u&&m.cursor==0u&&m.end==0u);
 }
 { kt_term_cell cache_cells[4]; kt_term_render_cache cache;
   sink inc; memset(&cache,0,sizeof cache); cache.cells=cache_cells;cache.cell_count=4u;
   memset(&inc,0,sizeof inc);
   assert(kt_term_render_incremental(&scr,KT_TERM_PROFILE_PETSCII,&cache,&ops,&inc)==0);
   assert(inc.draw==4u&&inc.cursor==1u);
   memset(&inc,0,sizeof inc);
   assert(kt_term_render_incremental(&scr,KT_TERM_PROFILE_PETSCII,&cache,&ops,&inc)==0);
   assert(inc.draw==0u&&inc.cursor==0u);
   scr.cells[3].ch='X'; memset(&inc,0,sizeof inc);
   assert(kt_term_render_incremental(&scr,KT_TERM_PROFILE_PETSCII,&cache,&ops,&inc)==0);
   assert(inc.draw==1u&&inc.cells[0].x==1u&&inc.cells[0].y==1u);
   scr.x=0u;scr.y=0u;memset(&inc,0,sizeof inc);
   assert(kt_term_render_incremental(&scr,KT_TERM_PROFILE_PETSCII,&cache,&ops,&inc)==0);
   assert(inc.draw==0u&&inc.cursor==1u&&inc.cx==0u&&inc.cy==0u);
   cache.valid=0u;memset(&inc,0,sizeof inc);
   assert(kt_term_render_incremental(&scr,KT_TERM_PROFILE_PETSCII,&cache,&ops,&inc)==0);
   assert(inc.draw==4u&&inc.cursor==1u);
   cache.cell_count=3u;
   assert(kt_term_render_incremental(&scr,KT_TERM_PROFILE_PETSCII,&cache,&ops,&inc)==-1);
   cache.cell_count=4u;cache.valid=1u;cache.width=1u;cache.height=4u;
   memset(&inc,0,sizeof inc);
   assert(kt_term_render_incremental(&scr,KT_TERM_PROFILE_PETSCII,&cache,&ops,&inc)==0);
   assert(inc.draw==4u&&inc.cursor==1u);
 }
 return 0;
}
