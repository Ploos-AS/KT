#include "kt/terminal_screen.h"
static kt_term_cell *at(kt_term_screen *s,uint16_t x,uint16_t y){return &s->cells[(size_t)y*s->width+x];}
void kt_term_screen_clear(kt_term_screen *s){size_t i;if(!s)return;for(i=0;i<s->cell_count;i++){s->cells[i].ch=' ';s->cells[i].attr.fg=7u;s->cells[i].attr.bg=0u;s->cells[i].attr.flags=0u;}s->x=0;s->y=0;}
int kt_term_screen_init(kt_term_screen *s,uint16_t w,uint16_t h,kt_term_cell *c,size_t n){if(!s||!c||!w||!h||n<(size_t)w*h)return -1;s->width=w;s->height=h;s->cells=c;s->cell_count=(size_t)w*h;kt_term_screen_clear(s);return 0;}
static void scroll(kt_term_screen *s){size_t i,row=(size_t)s->width;for(i=0;i+row<s->cell_count;i++)s->cells[i]=s->cells[i+row];for(;i<s->cell_count;i++){s->cells[i].ch=' ';s->cells[i].attr.fg=7u;s->cells[i].attr.bg=0u;s->cells[i].attr.flags=0u;}}
static void advance_line(kt_term_screen*s){s->x=0;if(s->y+1u<s->height)s->y++;else{scroll(s);s->y=(uint16_t)(s->height-1u);}}
static void put(void *p,uint8_t ch,const kt_term_attr *a){kt_term_screen*s=p;if(s->x>=s->width||s->y>=s->height)return;at(s,s->x,s->y)->ch=ch;at(s,s->x,s->y)->attr=*a;if(++s->x>=s->width)advance_line(s);}
static void cr(void*p){((kt_term_screen*)p)->x=0;}
static void lf(void*p){kt_term_screen*s=p;if(s->y+1u<s->height)s->y++;else scroll(s);}
static void bs(void*p){kt_term_screen*s=p;if(s->x)s->x--;}
static void bell(void*p){(void)p;}
static void move(void*p,int16_t dx,int16_t dy){kt_term_screen*s=p;int32_t x=(int32_t)s->x+dx,y=(int32_t)s->y+dy;if(x<0)x=0;if(y<0)y=0;if(x>=s->width)x=s->width-1;if(y>=s->height)y=s->height-1;s->x=(uint16_t)x;s->y=(uint16_t)y;}
static void pos(void*p,uint16_t r,uint16_t c){kt_term_screen*s=p;if(!r)r=1;if(!c)c=1;s->y=(uint16_t)((r>s->height?s->height:r)-1u);s->x=(uint16_t)((c>s->width?s->width:c)-1u);}
static void blank(kt_term_cell*c){c->ch=' ';c->attr.fg=7u;c->attr.bg=0u;c->attr.flags=0u;}
static void erase(void*p,uint8_t m){kt_term_screen*s=p;size_t i,cur=(size_t)s->y*s->width+s->x;
 if(m==0u){for(i=cur;i<s->cell_count;i++)blank(&s->cells[i]);}
 else if(m==1u){for(i=0;i<=cur&&i<s->cell_count;i++)blank(&s->cells[i]);}
 else if(m==2u){for(i=0;i<s->cell_count;i++)blank(&s->cells[i]);s->x=0;s->y=0;}
}
static const kt_term_ops ops={put,cr,lf,bs,bell,move,pos,erase};
const kt_term_ops *kt_term_screen_ops(void){return &ops;}

uint32_t kt_term_decode_codepoint(uint8_t profile,uint8_t ch){
 if(profile==KT_TERM_PROFILE_CP437){
  switch(ch){
   case 0xB3u:return 0x2502u; case 0xC4u:return 0x2500u;
   case 0xDAu:return 0x250Cu; case 0xBFu:return 0x2510u;
   case 0xC0u:return 0x2514u; case 0xD9u:return 0x2518u;
   case 0xC3u:return 0x251Cu; case 0xB4u:return 0x2524u;
   case 0xC2u:return 0x252Cu; case 0xC1u:return 0x2534u;
   case 0xC5u:return 0x253Cu; default:break;
  }
 }
 if(profile==KT_TERM_PROFILE_PETSCII){
  /* PETSCII letters are non-contiguous relative to Unicode/ASCII:
     $41-$5A are uppercase; $61-$7A are the alternate uppercase range. */
  if(ch>=0x41u&&ch<=0x5Au)return (uint32_t)ch;
  if(ch>=0x61u&&ch<=0x7Au)return (uint32_t)(ch-0x20u);
 }
 /* ASCII-compatible bytes remain identity-mapped. */
 if(ch<0x80u)return (uint32_t)ch;
 return 0xFFFDu;
}
