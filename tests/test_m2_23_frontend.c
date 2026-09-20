#include "kt/terminal_frontend.h"
#include <assert.h>
#include <string.h>
typedef struct sink{unsigned begin,draw,cursor,end;uint8_t wire[32];size_t n;int blocked;}sink;
static void bf(void*p,uint16_t w,uint16_t h){sink*s=p;(void)w;(void)h;s->begin++;}
static void dc(void*p,const kt_term_render_cell*c){sink*s=p;(void)c;s->draw++;}
static void cu(void*p,uint16_t x,uint16_t y){sink*s=p;(void)x;(void)y;s->cursor++;}
static void ef(void*p){((sink*)p)->end++;}
static int wr(void*p,const uint8_t*d,size_t n){sink*s=p;if(s->blocked)return 0;memcpy(s->wire+s->n,d,n);s->n+=n;return (int)n;}
int main(void){
 kt_term_cell cells[4],cache_cells[4];kt_term_screen scr;kt_term_render_cache cache={cache_cells,4,0,0,0,0,0,0};
 kt_term_renderer_ops ro={bf,dc,cu,ef};kt_term_output_ops oo={wr};kt_term_frontend f;sink s={0};
 assert(kt_term_screen_init(&scr,4,1,cells,4)==0);
 assert(kt_term_frontend_init(&f,&scr,KT_TERM_PROFILE_ANSI,&cache,&ro,&s,&oo,&s)==0);
 assert(kt_term_frontend_render(&f)==0&&s.draw==4u&&s.cursor==1u);
 s.draw=s.cursor=0;assert(kt_term_frontend_render(&f)==0&&s.draw==0u&&s.cursor==0u);
 scr.cells[2].ch='X';assert(kt_term_frontend_render(&f)==0&&s.draw==1u);
 s.draw=s.cursor=0;f.profile=KT_TERM_PROFILE_PETSCII;
 assert(kt_term_frontend_render(&f)==0&&s.draw==4u&&s.cursor==1u);
 s.draw=s.cursor=0;cache.width=2u;
 assert(kt_term_frontend_render(&f)==0&&s.draw==4u&&s.cursor==1u);
 s.draw=s.cursor=0;assert(kt_term_frontend_set_profile(&f,KT_TERM_PROFILE_CP437)==0);
 assert(kt_term_frontend_render(&f)==0&&s.draw==4u&&s.cursor==1u);
 s.draw=s.cursor=0;assert(kt_term_frontend_set_profile(&f,KT_TERM_PROFILE_CP437)==0);
 assert(kt_term_frontend_render(&f)==0&&s.draw==0u);
 kt_term_frontend_invalidate(&f);assert(kt_term_frontend_render(&f)==0&&s.draw==4u);
 assert(kt_term_frontend_key(&f,KT_TERM_KEY_UP,0)==0&&s.n==3u&&!memcmp(s.wire,"\x1b[A",3));
 s.blocked=1;assert(kt_term_frontend_key(&f,KT_TERM_KEY_F12,KT_TERM_MOD_ALT)==0);
 assert(f.key_output.pending_len==7u);s.blocked=0;assert(kt_term_frontend_flush(&f)==0);
 assert(s.n==10u&&!memcmp(s.wire+3,"\x1b[24;3~",7));
 {kt_term_frontend noout;sink z={0};kt_term_render_cache cc={cache_cells,4,0,0,0,0,0};
  assert(kt_term_frontend_init(&noout,&scr,KT_TERM_PROFILE_ANSI,&cc,&ro,&z,0,0)==0);
  assert(kt_term_frontend_key(&noout,KT_TERM_KEY_UP,0)==-1);
 }
 return 0;
}
