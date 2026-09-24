#include <assert.h>
#include "kt/terminal_geometry_adapter.h"
typedef struct capture{unsigned calls;kt_term_geometry_event last;}capture;
static int on_geometry(void*ctx,const kt_term_geometry_event*e){capture*c=(capture*)ctx;c->calls++;c->last=*e;return 0;}
int main(void){
 kt_term_geometry_event_state state;kt_term_geometry_event_ops ops={on_geometry};
 kt_term_viewport v;capture c={0};
 kt_term_geometry_event_state_reset(&state);
 assert(kt_term_viewport_init(&v,640,400,8,16)==0);
 assert(kt_term_geometry_from_viewport(&state,&ops,&c,&v)==0);
 assert(c.calls==1&&c.last.source==KT_TERM_GEOMETRY_SOURCE_LOCAL);
 assert(c.last.cols==80&&c.last.rows==25);
 assert(kt_term_geometry_from_viewport(&state,&ops,&c,&v)==1&&c.calls==1);
 assert(kt_term_viewport_resize(&v,640,480)==0);
 assert(kt_term_geometry_from_viewport(&state,&ops,&c,&v)==0);
 assert(c.calls==2&&c.last.cols==80&&c.last.rows==30);
 assert(kt_term_geometry_from_viewport(0,&ops,&c,&v)==-1);
 assert(kt_term_geometry_from_viewport(&state,&ops,&c,0)==-1);
 return 0;
}
