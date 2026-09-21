#include "kt/terminal_resize.h"

int kt_term_resize_apply(kt_term_session*s,kt_term_present_scheduler*sched,
                         uint8_t cell_height,uint16_t fb_height,
                         kt_term_resize_result*out){
 uint16_t ow,oh,nw,nh;
 kt_term_dirty_rows d;
 kt_term_present_scheduler sched_old;
 uint8_t cache_valid=0,have_cache=0;
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
 if(sched)sched_old=*sched;
 if(s->render_cache){have_cache=1;cache_valid=s->render_cache->valid;}

 rc=kt_term_session_apply_geometry(s);
 if(rc!=0)return rc;

 d.first=0;d.count=fb_height;d.valid=1;
 if(sched){
  rc=kt_term_present_scheduler_add_rows(sched,d.first,d.count);
  if(rc<0){
   /* Scheduler is caller-owned; restore it. Screen mutation is only reached
      after scheduler arguments have been prevalidated by this boundary. */
   *sched=sched_old;
   if(have_cache)s->render_cache->valid=cache_valid;
   return -4;
  }
 }
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
