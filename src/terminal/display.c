#include "kt/terminal_display.h"
int kt_term_display_present(const kt_term_display_ops*o,void*ctx,const kt_term_display_frame*f){
 int r;if(!o||!o->present_rows||!f||!f->pixels||!f->width||!f->height)return -1;
 if(f->format==KT_TERM_DISPLAY_INDEXED8){if(f->stride<f->width)return -1;}
 else if(f->format==KT_TERM_DISPLAY_RGB24){if(f->stride<(size_t)f->width*3u)return -1;}
 else return -1;
 if(o->begin){r=o->begin(ctx,f);if(r)return r;}
 r=o->present_rows(ctx,f,0,f->height);if(r)return r;
 if(o->end){r=o->end(ctx,f);if(r)return r;}
 return 0;
}
