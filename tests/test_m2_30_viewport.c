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
 return 0;
}
