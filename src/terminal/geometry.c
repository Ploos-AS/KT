#include "kt/terminal_geometry.h"
static int within(const kt_term_geometry*g,uint16_t c,uint16_t r){return g&&c>=g->min_cols&&r>=g->min_rows&&c<=g->max_cols&&r<=g->max_rows;}
static int apply(kt_term_geometry*g){
 uint16_t c,r;if(!g)return -1;
 if(g->policy==KT_TERM_GEOMETRY_FIXED){c=g->fixed_cols;r=g->fixed_rows;}
 else if(g->policy==KT_TERM_GEOMETRY_VIEWPORT){c=g->viewport_cols;r=g->viewport_rows;}
 else if(g->policy==KT_TERM_GEOMETRY_REMOTE){
  if(!g->remote_valid)return -2;c=g->remote_cols;r=g->remote_rows;
 }else return -1;
 if(!c||!r||!within(g,c,r))return -2;g->cols=c;g->rows=r;return 0;
}
int kt_term_geometry_init(kt_term_geometry*g,uint16_t c,uint16_t r){
 if(!g||!c||!r)return -1;g->cols=c;g->rows=r;g->fixed_cols=c;g->fixed_rows=r;
 g->viewport_cols=c;g->viewport_rows=r;g->remote_cols=0;g->remote_rows=0;g->remote_valid=0;g->remote_source_valid=0;g->remote_source=0;g->min_cols=1;g->min_rows=1;g->max_cols=512;g->max_rows=256;
 g->policy=KT_TERM_GEOMETRY_FIXED;return 0;
}
int kt_term_geometry_set_policy(kt_term_geometry*g,kt_term_geometry_policy p){
 kt_term_geometry old;if(!g)return -1;old=*g;g->policy=p;if(apply(g)!=0){*g=old;return -2;}return 0;
}
int kt_term_geometry_set_limits(kt_term_geometry*g,uint16_t minc,uint16_t minr,uint16_t maxc,uint16_t maxr){
 kt_term_geometry old;if(!g||!minc||!minr||minc>maxc||minr>maxr)return -1;
 old=*g;g->min_cols=minc;g->min_rows=minr;g->max_cols=maxc;g->max_rows=maxr;
 if(!within(g,g->fixed_cols,g->fixed_rows)||!within(g,g->viewport_cols,g->viewport_rows)||
    (g->remote_valid&&!within(g,g->remote_cols,g->remote_rows))||apply(g)!=0){*g=old;return -2;}
 return 0;
}
int kt_term_geometry_set_fixed(kt_term_geometry*g,uint16_t c,uint16_t r){
 if(!g||!c||!r)return -1;if(!within(g,c,r))return -2;g->fixed_cols=c;g->fixed_rows=r;return g->policy==KT_TERM_GEOMETRY_FIXED?apply(g):0;
}
int kt_term_geometry_set_viewport(kt_term_geometry*g,uint16_t c,uint16_t r){
 if(!g||!c||!r)return -1;if(!within(g,c,r))return -2;g->viewport_cols=c;g->viewport_rows=r;return g->policy==KT_TERM_GEOMETRY_VIEWPORT?apply(g):0;
}
int kt_term_geometry_from_viewport(kt_term_geometry*g,const kt_term_viewport*v){
 if(!g||!v||!v->cols||!v->rows)return -1;
 return kt_term_geometry_set_viewport(g,v->cols,v->rows);
}
int kt_term_geometry_set_remote(kt_term_geometry*g,uint16_t c,uint16_t r){
 if(!g||!c||!r)return -1;if(!within(g,c,r))return -2;g->remote_cols=c;g->remote_rows=r;g->remote_valid=1;return g->policy==KT_TERM_GEOMETRY_REMOTE?apply(g):0;
}
void kt_term_geometry_clear_remote(kt_term_geometry*g){if(g){g->remote_valid=0;g->remote_source_valid=0;if(g->policy==KT_TERM_GEOMETRY_REMOTE){g->policy=KT_TERM_GEOMETRY_FIXED;g->cols=g->fixed_cols;g->rows=g->fixed_rows;}}}
int kt_term_geometry_claim_remote(kt_term_geometry*g,kt_term_geometry_source source){
 if(!g||(source!=KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS&&source!=KT_TERM_GEOMETRY_SOURCE_SSH_PTY))return -1;
 if(g->remote_source_valid&&g->remote_source!=(uint8_t)source)return -2;
 g->remote_source=(uint8_t)source;g->remote_source_valid=1;return 0;
}
void kt_term_geometry_release_remote(kt_term_geometry*g,kt_term_geometry_source source){
 if(g&&g->remote_source_valid&&g->remote_source==(uint8_t)source)kt_term_geometry_clear_remote(g);
}

int kt_term_geometry_negotiate(kt_term_geometry*g,kt_term_geometry_source source,uint16_t c,uint16_t r){
 if(!g||!c||!r)return -1;
 if(source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS||source==KT_TERM_GEOMETRY_SOURCE_SSH_PTY){
  if(kt_term_geometry_claim_remote(g,source)!=0)return -3;
  return kt_term_geometry_set_remote(g,c,r);
 }
 if(source==KT_TERM_GEOMETRY_SOURCE_LOCAL)
  return kt_term_geometry_set_viewport(g,c,r);
 return -2;
}
