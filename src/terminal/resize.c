#include "kt/terminal_resize.h"

int kt_term_resize_apply(kt_term_session*s,kt_term_present_scheduler*sched,
                         uint8_t cell_height,uint16_t fb_height,
                         kt_term_resize_result*out){
 uint16_t ow,oh,nw,nh;
 kt_term_dirty_rows d;
 int rc;
 if(!s||!s->screen||!s->geometry||!cell_height||!fb_height)return -1;
 ow=s->screen->width;oh=s->screen->height;
 nw=s->geometry->cols;nh=s->geometry->rows;
 if(out){out->old_cols=ow;out->old_rows=oh;out->new_cols=nw;out->new_rows=nh;out->changed=0;kt_term_dirty_rows_reset(&out->dirty);}
 if(ow==nw&&oh==nh)return 0;
 rc=kt_term_session_apply_geometry(s);
 if(rc!=0)return rc;
 d.first=0;d.count=fb_height;d.valid=1;
 if(sched){
  rc=kt_term_present_scheduler_add_rows(sched,d.first,d.count);
  if(rc<0)return -4;
 }
 if(out){out->changed=1;out->dirty=d;}
 return 0;
}
