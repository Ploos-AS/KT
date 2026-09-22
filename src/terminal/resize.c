#include "kt/terminal_resize.h"

int kt_term_resize_apply(kt_term_session*s,kt_term_present_scheduler*sched,
                         uint8_t cell_height,uint16_t fb_height,
                         kt_term_resize_result*out){
 uint16_t ow,oh,nw,nh;
 kt_term_dirty_rows d;
 kt_term_present_scheduler sched_new;
 uint8_t have_sched=0;
 int rc;
 (void)cell_height;
 if(!s||!s->screen||!s->geometry||!cell_height||!fb_height)return -1;
 ow=s->screen->width;oh=s->screen->height;
 nw=s->geometry->cols;nh=s->geometry->rows;
 if(out){out->old_cols=ow;out->old_rows=oh;out->new_cols=nw;out->new_rows=nh;out->changed=0;kt_term_dirty_rows_reset(&out->dirty);}
 if(ow==nw&&oh==nh)return 0;

 /* Preflight all operations that can fail before mutating screen state. */
 if((size_t)nw>(size_t)-1/(size_t)nh)return -2;
 if((size_t)nw*(size_t)nh>s->cell_capacity)return -3;
 d.first=0;d.count=fb_height;d.valid=1;
 if(sched){
  sched_new=*sched;
  rc=kt_term_present_scheduler_add_rows(&sched_new,&d);
  if(rc<0)return -4;
  have_sched=1;
 }

 rc=kt_term_session_apply_geometry(s);
 if(rc!=0)return rc;
 if(have_sched)*sched=sched_new;
 if(out){out->changed=1;out->dirty=d;}
 return 0;
}

int kt_term_resize_set_policy(kt_term_session*s,kt_term_present_scheduler*sched,
                              kt_term_geometry_policy policy,uint8_t cell_height,
                              uint16_t fb_height,kt_term_resize_result*out){
 kt_term_geometry old;
 int rc;
 if(!s||!s->geometry)return -1;
 old=*s->geometry;
 rc=kt_term_geometry_set_policy(s->geometry,policy);
 if(rc!=0)return rc;
 rc=kt_term_resize_apply(s,sched,cell_height,fb_height,out);
 if(rc!=0){*s->geometry=old;return rc;}
 return 0;
}

int kt_term_resize_negotiate(kt_term_session*s,kt_term_present_scheduler*sched,
                             kt_term_geometry_source source,uint16_t cols,uint16_t rows,
                             uint8_t cell_height,uint16_t fb_height,
                             kt_term_resize_result*out){
 kt_term_geometry old;
 int rc;
 if(!s||!s->geometry)return -1;
 old=*s->geometry;
 rc=kt_term_geometry_negotiate(s->geometry,source,cols,rows);
 if(rc!=0){*s->geometry=old;return rc;}
 rc=kt_term_resize_apply(s,sched,cell_height,fb_height,out);
 if(rc!=0){*s->geometry=old;return rc;}
 return 0;
}

int kt_term_resize_release_remote(kt_term_session*s,kt_term_present_scheduler*sched,
                                  kt_term_geometry_source source,uint8_t cell_height,
                                  uint16_t fb_height,kt_term_resize_result*out){
 kt_term_geometry old;
 int rc;
 if(!s||!s->geometry)return -1;
 old=*s->geometry;
 kt_term_geometry_release_remote(s->geometry,source);
 /* A non-owner release is deliberately a no-op. */
 if(s->geometry->remote_source_valid==old.remote_source_valid&&
    s->geometry->remote_valid==old.remote_valid&&
    s->geometry->policy==old.policy&&s->geometry->cols==old.cols&&
    s->geometry->rows==old.rows){
  if(out){out->old_cols=s->screen->width;out->old_rows=s->screen->height;
   out->new_cols=s->screen->width;out->new_rows=s->screen->height;
   out->changed=0;kt_term_dirty_rows_reset(&out->dirty);}
  return 0;
 }
 rc=kt_term_resize_apply(s,sched,cell_height,fb_height,out);
 if(rc!=0){*s->geometry=old;return rc;}
 return 0;
}
