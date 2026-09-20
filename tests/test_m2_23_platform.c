#include "kt/terminal_platform.h"
#include <assert.h>
#include <string.h>
typedef struct S{unsigned draw,present;uint8_t wire[32];size_t n;uint32_t bias;}S;
static void draw(void*p,const kt_term_render_cell*c){(void)c;((S*)p)->draw++;}
static int writefn(void*p,const uint8_t*d,size_t n){S*s=p;memcpy(s->wire+s->n,d,n);s->n+=n;return (int)n;}
static int norm(void*p,uint32_t raw,uint32_t mods,kt_term_platform_key*k){S*s=p;(void)mods;if(raw!=s->bias+1u)return -1;k->code=KT_TERM_KEY_UP;k->mods=KT_TERM_MOD_CTRL;return 0;}
static int present(void*p){((S*)p)->present++;return 0;}
int main(void){
 kt_term_cell cells[4],cc[4];kt_term_screen scr;kt_term_render_cache cache={cc,4,0,0,0,0,0,0};
 kt_term_renderer_ops ro={0,draw,0,0};kt_term_output_ops oo={writefn};kt_term_frontend f;
 kt_term_platform_ops po={norm,present};kt_term_platform_adapter a;S s={0};unsigned i;
 assert(kt_term_screen_init(&scr,4,1,cells,4)==0);
 assert(kt_term_frontend_init(&f,&scr,KT_TERM_PROFILE_ANSI,&cache,&ro,&s,&oo,&s)==0);
 for(i=0;i<3;i++){
  s.bias=100u*i;f.cache->valid=0u;
  assert(kt_term_platform_init(&a,(kt_term_platform)i,&f,&po,&s)==0);
  assert(kt_term_platform_present(&a)==0&&s.present==i+1u);
  assert(kt_term_platform_key_event(&a,s.bias+1u,0)==0);
  assert(s.n==3u*(i+1u)&&!memcmp(s.wire+s.n-3,"\x1b[1;5A",6)==0);
 }
 assert(kt_term_platform_key_event(&a,999u,0)==-2);
 return 0;
}
