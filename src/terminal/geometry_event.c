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
