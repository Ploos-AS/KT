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
 return 0;
}
