#include "kt/terminal_event.h"
#include <assert.h>
#include <string.h>
#include "kt/terminal_screen.h"
static unsigned n,b,p,r,k,m;static uint8_t wire[32];static size_t wire_n;
static int wr(void*c,const uint8_t*d,size_t z){(void)c;assert(z<=sizeof wire);memcpy(wire,d,z);wire_n=z;return 0;}static uint8_t last;static uint32_t key;
static void cb(void*c,uint8_t x){(void)c;n++;last=x;}
static void cp(void*c,const uint8_t*d,uint16_t z){(void)c;(void)d;p=z;}
static void cr(void*c,uint16_t w,uint16_t h){(void)c;r=w+h;}
static void ck(void*c,uint32_t x,uint8_t q){(void)c;k=x;m=q;}
static void cm(void*c,uint16_t x,uint16_t y,uint8_t q,uint8_t z){(void)c;b=x+y+q+z;}
int main(void){
 kt_term_input_ops o={cb,cp,cr,ck,cm};kt_term_event e={KT_TERM_EVENT_BYTE,0x41,0,0,0,0,0,0};
 assert(kt_term_dispatch_event(&e,&o,0)==0&&n==1u&&last==0x41u);
 e.type=KT_TERM_EVENT_PASTE;e.data=(const uint8_t*)"abc";e.data_len=3;assert(kt_term_dispatch_event(&e,&o,0)==0&&p==3u);
 e.type=KT_TERM_EVENT_RESIZE;e.width=80;e.height=25;assert(kt_term_dispatch_event(&e,&o,0)==0&&r==105u);
 e.type=KT_TERM_EVENT_KEY;e.key=KT_TERM_KEY_UP;e.mods=KT_TERM_MOD_CTRL|KT_TERM_MOD_SHIFT;
 assert(kt_term_dispatch_event(&e,&o,0)==0&&k==KT_TERM_KEY_UP&&m==(KT_TERM_MOD_CTRL|KT_TERM_MOD_SHIFT));
 e.key=KT_TERM_KEY_F12;e.mods=KT_TERM_MOD_ALT;assert(kt_term_dispatch_event(&e,&o,0)==0&&k==KT_TERM_KEY_F12&&m==KT_TERM_MOD_ALT);
 e.type=KT_TERM_EVENT_MOUSE;e.width=4;e.height=5;e.byte=1;e.mods=2;assert(kt_term_dispatch_event(&e,&o,0)==0&&b==12u);
 e.type=99;assert(kt_term_dispatch_event(&e,&o,0)==-3);
 assert(kt_term_dispatch_event(0,&o,0)==-1);
 { kt_term_cell cells[4];kt_term_screen s;kt_term t;kt_term_feed_adapter a;kt_term_event x;
   assert(kt_term_screen_init(&s,4u,1u,cells,4u)==0);
   kt_term_init(&t,kt_term_screen_ops(),&s);kt_term_feed_adapter_init(&a,&t);
   x.type=KT_TERM_EVENT_BYTE;x.byte='A';assert(kt_term_dispatch_event(&x,kt_term_feed_adapter_ops(),&a)==0);
   assert(cells[0].ch=='A');
   x.type=KT_TERM_EVENT_PASTE;x.data=(const uint8_t*)"BC";x.data_len=2u;
   assert(kt_term_dispatch_event(&x,kt_term_feed_adapter_ops(),&a)==0);
   assert(cells[1].ch=='B'&&cells[2].ch=='C');
   x.type=KT_TERM_EVENT_KEY;assert(kt_term_dispatch_event(&x,kt_term_feed_adapter_ops(),&a)==-2);
 }
 {kt_term_output_ops oo={wr};kt_term_key_output_adapter oa;kt_term_event q;
   kt_term_key_output_adapter_init(&oa,&oo,0);q.type=KT_TERM_EVENT_KEY;q.key=KT_TERM_KEY_F12;q.mods=KT_TERM_MOD_ALT;wire_n=0;
   assert(kt_term_dispatch_event(&q,kt_term_key_output_adapter_ops(),&oa)==0);
   assert(wire_n==7u&&!memcmp(wire,"\x1b[24;3~",7));
 }
 return 0;
}
