#include <assert.h>
#include "kt/terminal_geometry_adapter.h"

typedef struct capture { unsigned calls; kt_term_geometry_event last; int fail_once; } capture;
static int on_geometry(void *ctx,const kt_term_geometry_event *e){
 capture *c=(capture *)ctx;c->calls++;c->last=*e;
 if(c->fail_once){c->fail_once=0;return -9;}return 0;
}
int main(void){
 kt_term_geometry_event_state state;
 kt_term_geometry_event_ops ops={on_geometry};
 capture c={0};
 const uint8_t p80x25[4]={0,80,0,25};
 const uint8_t p132x43[4]={0,132,0,43};
 const uint8_t zero[4]={0,0,0,43};
 kt_term_geometry_event_state_reset(&state);
 assert(kt_term_geometry_from_ssh_pty(&state,&ops,&c,p80x25,4)==0);
 assert(c.calls==1&&c.last.source==KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
 assert(c.last.cols==80&&c.last.rows==25);
 assert(kt_term_geometry_from_ssh_pty(&state,&ops,&c,p80x25,4)==1&&c.calls==1);
 c.fail_once=1;
 assert(kt_term_geometry_from_ssh_pty(&state,&ops,&c,p132x43,4)==-9);
 assert(state.cols[KT_TERM_GEOMETRY_SOURCE_SSH_PTY]==80);
 assert(kt_term_geometry_from_ssh_pty(&state,&ops,&c,p132x43,4)==0);
 assert(c.last.cols==132&&c.last.rows==43);
 assert(kt_term_geometry_from_ssh_pty(&state,&ops,&c,p80x25,3)==-1);
 assert(kt_term_geometry_from_ssh_pty(&state,&ops,&c,zero,4)==-2);
 return 0;
}
