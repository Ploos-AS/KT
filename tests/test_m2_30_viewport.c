#include "kt/terminal_viewport.h"
#include <assert.h>
int main(void){
 kt_term_viewport v;
 assert(kt_term_viewport_init(&v,640,480,8,16)==0&&v.cols==80&&v.rows==30&&v.used_width==640&&v.used_height==480&&v.offset_x==0&&v.offset_y==0);
 assert(kt_term_viewport_resize(&v,640,400)==0&&v.cols==80&&v.rows==25);
 assert(kt_term_viewport_resize(&v,645,407)==0&&v.cols==80&&v.rows==25&&v.used_width==640&&v.used_height==400);
 assert(kt_term_viewport_set_anchor(&v,KT_TERM_VIEWPORT_TOP_LEFT)==0&&v.offset_x==0&&v.offset_y==0);
 assert(kt_term_viewport_set_anchor(&v,KT_TERM_VIEWPORT_CENTER)==0&&v.offset_x==2&&v.offset_y==3);
 assert(kt_term_viewport_set_anchor(&v,KT_TERM_VIEWPORT_BOTTOM_RIGHT)==0&&v.offset_x==5&&v.offset_y==7);
 assert(kt_term_viewport_resize(&v,653,419)==0&&v.cols==81&&v.rows==26&&v.used_width==648&&v.used_height==416&&v.offset_x==5&&v.offset_y==3);
 assert(kt_term_viewport_set_anchor(&v,(kt_term_viewport_anchor)99)==-1);
 assert(kt_term_viewport_init(&v,7,15,8,16)==-1);
 assert(kt_term_viewport_init(&v,640,480,0,16)==-1);
 { kt_term_viewport_resize_adapter a;kt_term_event e;
   assert(kt_term_viewport_init(&v,640,400,8,16)==0);
   kt_term_viewport_resize_adapter_init(&a,&v);
   e.type=KT_TERM_EVENT_RESIZE;e.width=645;e.height=407;
   assert(kt_term_dispatch_event(&e,kt_term_viewport_resize_adapter_ops(),&a)==0);
   assert(v.pixel_width==645&&v.pixel_height==407&&v.cols==80&&v.rows==25);
 }
 return 0;
}
