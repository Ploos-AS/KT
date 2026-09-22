#include "kt/terminal_geometry_event.h"
#include <string.h>

void kt_term_geometry_event_state_reset(kt_term_geometry_event_state*s){
 if(s)memset(s,0,sizeof(*s));
}

int kt_term_geometry_event_emit(kt_term_geometry_event_state*s,
                                const kt_term_geometry_event_ops*ops,void*ctx,
                                kt_term_geometry_source source,
                                uint16_t cols,uint16_t rows){
 kt_term_geometry_event e;
 unsigned i=(unsigned)source;
 int rc;
 if(!s||!ops||!ops->on_geometry||!cols||!rows||i>2u)return -1;
 if(s->valid[i]&&s->cols[i]==cols&&s->rows[i]==rows)return 1;
 e.source=source;e.cols=cols;e.rows=rows;
 rc=ops->on_geometry(ctx,&e);
 if(rc!=0)return rc;
 s->cols[i]=cols;s->rows[i]=rows;s->valid[i]=1;
 return 0;
}

static int resize_on_geometry(void*ctx,const kt_term_geometry_event*e){
 kt_term_geometry_resize_adapter*a=(kt_term_geometry_resize_adapter*)ctx;
 if(!a||!e||!a->session)return -1;
 return kt_term_resize_negotiate(a->session,a->scheduler,e->source,e->cols,e->rows,
                                 a->cell_height,a->fb_height,a->result);
}
static const kt_term_geometry_event_ops resize_ops={resize_on_geometry};

void kt_term_geometry_resize_adapter_init(kt_term_geometry_resize_adapter*a,
                                          kt_term_session*s,
                                          kt_term_present_scheduler*sched,
                                          uint8_t cell_height,uint16_t fb_height,
                                          kt_term_resize_result*result){
 if(!a)return;
 a->session=s;a->scheduler=sched;a->cell_height=cell_height;
 a->fb_height=fb_height;a->result=result;
}
const kt_term_geometry_event_ops *kt_term_geometry_resize_adapter_ops(void){
 return &resize_ops;
}
