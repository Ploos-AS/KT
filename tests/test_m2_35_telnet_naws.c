#include <assert.h>
#include "kt/terminal_geometry_adapter.h"

typedef struct capture {
 unsigned calls;
 kt_term_geometry_event last;
 int fail_once;
} capture;

static int on_geometry(void *ctx,const kt_term_geometry_event *e){
 capture *c=(capture *)ctx;
 c->calls++;c->last=*e;
 if(c->fail_once){c->fail_once=0;return -7;}
 return 0;
}

int main(void){
 kt_term_geometry_event_state state;
 kt_term_geometry_event_ops ops={on_geometry};
 capture c={0};
 const uint8_t p80x25[4]={0,80,0,25};
 const uint8_t p132x43[4]={0,132,0,43};
 const uint8_t zero_cols[4]={0,0,0,25};
 const uint8_t zero_rows[4]={0,80,0,0};

 kt_term_geometry_event_state_reset(&state);
 assert(kt_term_geometry_from_telnet_naws(&state,&ops,&c,p80x25,4)==0);
 assert(c.calls==1);
 assert(c.last.source==KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
 assert(c.last.cols==80&&c.last.rows==25);

 assert(kt_term_geometry_from_telnet_naws(&state,&ops,&c,p80x25,4)==1);
 assert(c.calls==1);

 c.fail_once=1;
 assert(kt_term_geometry_from_telnet_naws(&state,&ops,&c,p132x43,4)==-7);
 assert(c.calls==2);
 assert(state.cols[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==80);
 assert(state.rows[KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS]==25);
 assert(kt_term_geometry_from_telnet_naws(&state,&ops,&c,p132x43,4)==0);
 assert(c.calls==3&&c.last.cols==132&&c.last.rows==43);

 assert(kt_term_geometry_from_telnet_naws(&state,&ops,&c,p80x25,3)==-1);
 assert(kt_term_geometry_from_telnet_naws(&state,&ops,&c,zero_cols,4)==-2);
 assert(kt_term_geometry_from_telnet_naws(&state,&ops,&c,zero_rows,4)==-2);
 assert(kt_term_geometry_from_telnet_naws(&state,&ops,&c,0,4)==-1);
 assert(kt_term_geometry_from_telnet_naws(0,&ops,&c,p80x25,4)==-1);
 return 0;
}
