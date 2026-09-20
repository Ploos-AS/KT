#include "kt/terminal_display.h"
#include <assert.h>
#include <string.h>
typedef struct S{unsigned begin,present,end;uint16_t first,count;int fail;}S;
static int b(void*p,const kt_term_display_frame*f){S*s=p;(void)f;s->begin++;return s->fail==1?-11:0;}
static int pr(void*p,const kt_term_display_frame*f,uint16_t first,uint16_t count){S*s=p;(void)f;s->present++;s->first=first;s->count=count;return s->fail==2?-12:0;}
static int e(void*p,const kt_term_display_frame*f){S*s=p;(void)f;s->end++;return s->fail==3?-13:0;}
int main(void){
 uint8_t pix[64]={0};kt_term_display_frame f={pix,4,4,4,KT_TERM_DISPLAY_INDEXED8,0};kt_term_display_ops o={b,pr,e};S s={0};
 assert(kt_term_display_present(&o,&s,&f)==0&&s.begin==1&&s.present==1&&s.end==1&&s.first==0&&s.count==4);
 memset(&s,0,sizeof s);assert(kt_term_display_present_rows(&o,&s,&f,1,2)==0&&s.first==1&&s.count==2);
 assert(kt_term_display_present_rows(&o,&s,&f,4,1)==-1);assert(kt_term_display_present_rows(&o,&s,&f,3,2)==-1);assert(kt_term_display_present_rows(&o,&s,&f,0,0)==-1);
 f.format=KT_TERM_DISPLAY_RGB24;f.stride=12;assert(kt_term_display_present(&o,&s,&f)==0);f.stride=11;assert(kt_term_display_present(&o,&s,&f)==-1);
 f.format=KT_TERM_DISPLAY_INDEXED8;f.stride=4;s.fail=1;assert(kt_term_display_present(&o,&s,&f)==-11);s.fail=2;assert(kt_term_display_present(&o,&s,&f)==-12);s.fail=3;assert(kt_term_display_present(&o,&s,&f)==-13);
 return 0;
}
