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
   assert(kt_term_viewport_resize_adapter_result(&a)==0);
 }
 { kt_term_viewport_resize_adapter a;kt_term_event e;kt_term_viewport old;
   assert(kt_term_viewport_init(&v,640,480,8,16)==0);
   assert(kt_term_viewport_set_fixed(&v,80,25)==0);
   assert(kt_term_viewport_set_anchor(&v,KT_TERM_VIEWPORT_CENTER)==0);
   old=v;kt_term_viewport_resize_adapter_init(&a,&v);
   e.type=KT_TERM_EVENT_RESIZE;e.width=639;e.height=399;
   assert(kt_term_dispatch_event(&e,kt_term_viewport_resize_adapter_ops(),&a)==0);
   assert(kt_term_viewport_resize_adapter_result(&a)==-2);
   assert(v.pixel_width==old.pixel_width&&v.pixel_height==old.pixel_height);
   assert(v.cols==old.cols&&v.rows==old.rows&&v.used_width==old.used_width&&v.used_height==old.used_height);
   assert(v.offset_x==old.offset_x&&v.offset_y==old.offset_y&&v.mode==old.mode);
 }
 assert(kt_term_viewport_init(&v,640,480,8,16)==0);
 assert(kt_term_viewport_set_fixed(&v,80,25)==0&&v.mode==KT_TERM_VIEWPORT_FIXED&&v.cols==80&&v.rows==25&&v.used_height==400);
 assert(kt_term_viewport_set_anchor(&v,KT_TERM_VIEWPORT_CENTER)==0&&v.offset_x==0&&v.offset_y==40);
 assert(kt_term_viewport_resize(&v,648,488)==0&&v.cols==80&&v.rows==25&&v.offset_x==4&&v.offset_y==44);
 { kt_term_viewport old=v;
   assert(kt_term_viewport_resize(&v,639,399)==-2);
   assert(v.pixel_width==old.pixel_width&&v.pixel_height==old.pixel_height);
   assert(v.cols==old.cols&&v.rows==old.rows&&v.used_width==old.used_width&&v.used_height==old.used_height);
   assert(v.offset_x==old.offset_x&&v.offset_y==old.offset_y);
   assert(v.mode==old.mode&&v.fixed_cols==old.fixed_cols&&v.fixed_rows==old.fixed_rows);
 }
 assert(kt_term_viewport_resize(&v,648,488)==0);
 assert(kt_term_viewport_set_fixed(&v,100,40)==-2&&v.cols==80&&v.rows==25&&v.fixed_cols==80&&v.fixed_rows==25);
 assert(kt_term_viewport_set_fit(&v)==0&&v.mode==KT_TERM_VIEWPORT_FIT&&v.cols==81&&v.rows==30);
 return 0;
}
