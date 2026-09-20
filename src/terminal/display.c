#include "kt/terminal_display.h"
static int valid(const kt_term_display_frame*f){
 if(!f||!f->pixels||!f->width||!f->height)return 0;
 if(f->format==KT_TERM_DISPLAY_INDEXED8)return f->stride>=f->width;
 if(f->format==KT_TERM_DISPLAY_RGB24)return f->stride>=(size_t)f->width*3u;
 return 0;
}
int kt_term_display_present_rows(const kt_term_display_ops*o,void*ctx,const kt_term_display_frame*f,uint16_t first,uint16_t count){
 int r;if(!o||!o->present_rows||!valid(f)||!count||first>=f->height||(uint32_t)first+count>f->height)return -1;
 if(o->begin){r=o->begin(ctx,f);if(r)return r;}r=o->present_rows(ctx,f,first,count);if(r)return r;
 if(o->end){r=o->end(ctx,f);if(r)return r;}return 0;
}
int kt_term_display_present(const kt_term_display_ops*o,void*ctx,const kt_term_display_frame*f){
 if(!valid(f))return -1;return kt_term_display_present_rows(o,ctx,f,0,f->height);
}
