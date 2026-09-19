#include "kt/terminal.h"
#include "kt/terminal_screen.h"
#include <assert.h>
int main(void){
 kt_term_cell cells[24]; kt_term_screen s; kt_term t;
 static const uint8_t input[]={'A',0x1b,'[','2',';','3','H','X',0x1b,'[','2','J','Z'};
 assert(kt_term_screen_init(&s,8,3,cells,24)==0);
 kt_term_init(&t,kt_term_screen_ops(),&s);
 kt_term_feed(&t,input,7); /* A + CUP 2;3 */
 assert(cells[0].ch=='A'); assert(s.x==2u&&s.y==1u);
 kt_term_feed(&t,input+7,1); assert(cells[10].ch=='X');
 kt_term_feed(&t,input+8,5); assert(s.x==0u&&s.y==0u); assert(cells[0].ch==' ');
 kt_term_feed(&t,input+13,1); assert(cells[0].ch=='Z');
 /* ED0 clears cursor through end; ED1 clears start through cursor. */
 kt_term_screen_clear(&s);
 { static const uint8_t fill[]="abcdefghijklmnopqrstuvwx";
   static const uint8_t ed0[]={0x1b,'[','2',';','3','H',0x1b,'[','0','J'};
   static const uint8_t ed1[]={0x1b,'[','2',';','3','H',0x1b,'[','1','J'};
   kt_term_feed(&t,fill,sizeof fill-1u);
   kt_term_feed(&t,ed0,sizeof ed0);
   assert(cells[9].ch=='j' && cells[10].ch==' ' && cells[23].ch==' ');
   kt_term_screen_clear(&s); kt_term_feed(&t,fill,sizeof fill-1u);
   kt_term_feed(&t,ed1,sizeof ed1);
   assert(cells[0].ch==' ' && cells[10].ch==' ' && cells[11].ch=='l');
 }
 /* Cursor positioning and movement clamp to screen bounds. */
 { static const uint8_t clamp[]={0x1b,'[','9','9',';','9','9','H',0x1b,'[','9','9','C',0x1b,'[','9','9','B'};
   kt_term_feed(&t,clamp,sizeof clamp);
   assert(s.x==7u && s.y==2u);
 }
 /* Fill four 8-column lines into a 3-line screen: wrapping must scroll. */
 kt_term_screen_clear(&s);
 { static const uint8_t lines[]="11111111222222223333333344444444";
   kt_term_feed(&t,lines,sizeof lines-1u);
   assert(cells[0].ch=='3');
   assert(cells[8].ch=='4');
   assert(cells[16].ch==' ');
   assert(s.x==0u && s.y==2u);
 }
 return 0;
}
