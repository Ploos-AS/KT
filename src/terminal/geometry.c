#include "kt/terminal_geometry.h"
static int apply(kt_term_geometry*g){
 uint16_t c,r;if(!g)return -1;
 if(g->policy==KT_TERM_GEOMETRY_FIXED){c=g->fixed_cols;r=g->fixed_rows;}
 else if(g->policy==KT_TERM_GEOMETRY_VIEWPORT){c=g->viewport_cols;r=g->viewport_rows;}
 else if(g->policy==KT_TERM_GEOMETRY_REMOTE){
  if(!g->remote_valid)return -2;c=g->remote_cols;r=g->remote_rows;
 }else return -1;
 if(!c||!r)return -2;g->cols=c;g->rows=r;return 0;
}
int kt_term_geometry_init(kt_term_geometry*g,uint16_t c,uint16_t r){
 if(!g||!c||!r)return -1;g->cols=c;g->rows=r;g->fixed_cols=c;g->fixed_rows=r;
 g->viewport_cols=c;g->viewport_rows=r;g->remote_cols=0;g->remote_rows=0;g->remote_valid=0;
 g->policy=KT_TERM_GEOMETRY_FIXED;return 0;
}
int kt_term_geometry_set_policy(kt_term_geometry*g,kt_term_geometry_policy p){
 kt_term_geometry old;if(!g)return -1;old=*g;g->policy=p;if(apply(g)!=0){*g=old;return -2;}return 0;
}
int kt_term_geometry_set_fixed(kt_term_geometry*g,uint16_t c,uint16_t r){
 if(!g||!c||!r)return -1;g->fixed_cols=c;g->fixed_rows=r;return g->policy==KT_TERM_GEOMETRY_FIXED?apply(g):0;
}
int kt_term_geometry_set_viewport(kt_term_geometry*g,uint16_t c,uint16_t r){
 if(!g||!c||!r)return -1;g->viewport_cols=c;g->viewport_rows=r;return g->policy==KT_TERM_GEOMETRY_VIEWPORT?apply(g):0;
}
int kt_term_geometry_set_remote(kt_term_geometry*g,uint16_t c,uint16_t r){
 if(!g||!c||!r)return -1;g->remote_cols=c;g->remote_rows=r;g->remote_valid=1;return g->policy==KT_TERM_GEOMETRY_REMOTE?apply(g):0;
}
void kt_term_geometry_clear_remote(kt_term_geometry*g){if(g){g->remote_valid=0;if(g->policy==KT_TERM_GEOMETRY_REMOTE){g->policy=KT_TERM_GEOMETRY_FIXED;g->cols=g->fixed_cols;g->rows=g->fixed_rows;}}}
