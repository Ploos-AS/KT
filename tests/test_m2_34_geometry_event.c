#include <assert.h>
#include "kt/terminal_geometry_event.h"

typedef struct capture {
 unsigned calls;
 kt_term_geometry_event last;
 int fail_once;
} capture;

static int on_geometry(void *ctx,const kt_term_geometry_event *e){
 capture *c=(capture *)ctx;
 c->calls++;
 c->last=*e;
 if(c->fail_once){c->fail_once=0;return -7;}
 return 0;
}

int main(void){
 kt_term_geometry_event_state state;
 kt_term_geometry_event_ops ops;
 capture c={0};
 ops.on_geometry=on_geometry;
 kt_term_geometry_event_state_reset(&state);

 assert(kt_term_geometry_event_emit(&state,&ops,&c,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,80,25)==0);
 assert(c.calls==1&&c.last.source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
 assert(c.last.cols==80&&c.last.rows==25);
 assert(kt_term_geometry_event_emit(&state,&ops,&c,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,80,25)==1);
 assert(c.calls==1);

 assert(kt_term_geometry_event_emit(&state,&ops,&c,
        KT_TERM_GEOMETRY_SOURCE_SSH_PTY,80,25)==0);
 assert(c.calls==2&&c.last.source==KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
 assert(kt_term_geometry_event_emit(&state,&ops,&c,
        KT_TERM_GEOMETRY_SOURCE_LOCAL,100,30)==0);
 assert(c.calls==3&&c.last.source==KT_TERM_GEOMETRY_SOURCE_LOCAL);

 c.fail_once=1;
 assert(kt_term_geometry_event_emit(&state,&ops,&c,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,132,43)==-7);
 assert(c.calls==4);
 assert(state.cols[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==80);
 assert(state.rows[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==25);
 assert(kt_term_geometry_event_emit(&state,&ops,&c,
        KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS,132,43)==0);
 assert(c.calls==5);
 assert(state.cols[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==132);
 assert(state.rows[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==43);

 assert(kt_term_geometry_event_emit(&state,&ops,&c,
        KT_TERM_GEOMETRY_SOURCE_LOCAL,0,25)==-1);
 assert(kt_term_geometry_event_emit(0,&ops,&c,
        KT_TERM_GEOMETRY_SOURCE_LOCAL,80,25)==-1);
 return 0;
}
