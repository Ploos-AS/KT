#include <assert.h>
#include <string.h>
#include "kt/terminal_session.h"

static void mark(kt_term_screen*s,uint16_t x,uint16_t y,uint8_t ch){
 s->cells[(size_t)y*s->width+x].ch=ch;
}
static uint8_t at(const kt_term_screen*s,uint16_t x,uint16_t y){
 return s->cells[(size_t)y*s->width+x].ch;
}
int main(void){
 kt_term_geometry g;
 kt_term_screen screen;
 kt_term_session session;
 kt_term_cell cells[100*30];
 kt_term_cell snapshot[100*30];

 memset(&screen,0,sizeof(screen));
 assert(kt_term_geometry_init(&g,80,25)==0);
 assert(kt_term_session_init(&session,&g,&screen,cells,100*30)==0);
 mark(&screen,0,0,'A');mark(&screen,79,24,'Z');
 screen.cursor_x=79;screen.cursor_y=24;

 assert(kt_term_geometry_set_fixed(&g,80,30)==0);
 assert(kt_term_session_apply_geometry(&session)==0);
 assert(screen.width==80&&screen.height==30);
 assert(at(&screen,0,0)=='A'&&at(&screen,79,24)=='Z');
 assert(at(&screen,0,25)==' '&&at(&screen,79,29)==' ');
 assert(screen.cursor_x==79&&screen.cursor_y==24);

 mark(&screen,1,1,'B');mark(&screen,79,29,'Q');
 assert(kt_term_geometry_set_fixed(&g,100,30)==0);
 assert(kt_term_session_apply_geometry(&session)==0);
 assert(screen.width==100&&screen.height==30);
 assert(at(&screen,0,0)=='A'&&at(&screen,1,1)=='B'&&at(&screen,79,29)=='Q');
 assert(at(&screen,80,0)==' '&&at(&screen,99,29)==' ');

 screen.cursor_x=99;screen.cursor_y=29;
 assert(kt_term_geometry_set_fixed(&g,40,20)==0);
 assert(kt_term_session_apply_geometry(&session)==0);
 assert(screen.width==40&&screen.height==20);
 assert(at(&screen,0,0)=='A'&&at(&screen,1,1)=='B');
 assert(screen.cursor_x==39&&screen.cursor_y==19);

 memcpy(snapshot,cells,sizeof(cells));
 session.cell_capacity=40*20;
 assert(kt_term_geometry_set_fixed(&g,80,25)==0);
 assert(kt_term_session_apply_geometry(&session)==-3);
 assert(screen.width==40&&screen.height==20);
 assert(screen.cursor_x==39&&screen.cursor_y==19);
 assert(memcmp(snapshot,cells,sizeof(cells))==0);

 {
  kt_term_geometry pg;
  kt_term_screen ps;
  kt_term_session ss;
  kt_term_cell pcells[132*43];
  kt_term_viewport vp;
  memset(&ps,0,sizeof(ps));
  assert(kt_term_geometry_init(&pg,80,25)==0);
  assert(kt_term_session_init(&ss,&pg,&ps,pcells,132*43)==0);
  mark(&ps,5,5,'V');
  assert(kt_term_viewport_init(&vp,640,480,8,16)==0);
  assert(kt_term_geometry_from_viewport(&pg,&vp)==0);
  assert(pg.cols==80&&pg.rows==25);
  assert(kt_term_geometry_set_policy(&pg,KT_TERM_GEOMETRY_VIEWPORT)==0);
  assert(pg.cols==80&&pg.rows==30);
  assert(kt_term_session_apply_geometry(&ss)==0);
  assert(ps.width==80&&ps.height==30&&at(&ps,5,5)=='V');

  assert(kt_term_geometry_negotiate(&pg,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,132,43)==0);
  assert(kt_term_geometry_set_policy(&pg,KT_TERM_GEOMETRY_REMOTE)==0);
  assert(kt_term_session_apply_geometry(&ss)==0);
  assert(ps.width==132&&ps.height==43&&at(&ps,5,5)=='V');

  kt_term_geometry_release_remote(&pg,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
  assert(pg.policy==KT_TERM_GEOMETRY_FIXED&&pg.cols==80&&pg.rows==25);
  assert(kt_term_session_apply_geometry(&ss)==0);
  assert(ps.width==80&&ps.height==25&&at(&ps,5,5)=='V');
 }
 return 0;
}
