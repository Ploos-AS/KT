#include "kt/terminal_geometry_session.h"

int kt_term_geometry_session_init(kt_term_geometry_session*g,
                                  kt_term_session*s,
                                  kt_term_present_scheduler*sched,
                                  uint8_t cell_height,uint16_t fb_height,
                                  kt_term_resize_result*result){
 if(!g||!s||!s->geometry||!s->screen||!cell_height||!fb_height)return -1;
 kt_term_geometry_event_state_reset(&g->events);
 kt_term_geometry_resize_adapter_init(&g->resize,s,sched,cell_height,fb_height,result);
 return 0;
}
void kt_term_geometry_session_reset(kt_term_geometry_session*g){
 if(g)kt_term_geometry_event_state_reset(&g->events);
}
int kt_term_geometry_session_telnet_naws(kt_term_geometry_session*g,
                                         const uint8_t*data,size_t len){
 if(!g)return -1;
 return kt_term_geometry_from_telnet_naws(&g->events,
        kt_term_geometry_resize_adapter_ops(),&g->resize,data,len);
}
int kt_term_geometry_session_ssh_pty(kt_term_geometry_session*g,
                                     const uint8_t*data,size_t len){
 if(!g)return -1;
 return kt_term_geometry_from_ssh_pty(&g->events,
        kt_term_geometry_resize_adapter_ops(),&g->resize,data,len);
}
int kt_term_geometry_session_viewport(kt_term_geometry_session*g,
                                      const kt_term_viewport*v){
 if(!g)return -1;
 return kt_term_geometry_from_viewport(&g->events,
        kt_term_geometry_resize_adapter_ops(),&g->resize,v);
}
int kt_term_geometry_session_disconnect(kt_term_geometry_session*g,
                                        kt_term_geometry_source source){
 unsigned i=(unsigned)source;
 int rc;
 if(!g||!g->resize.session||i>2u||source==KT_TERM_GEOMETRY_SOURCE_LOCAL)return -1;
 rc=kt_term_resize_release_remote(g->resize.session,g->resize.scheduler,source,
                                  g->resize.cell_height,g->resize.fb_height,
                                  g->resize.result);
 if(rc==0)g->events.valid[i]=0;
 return rc;
}

int kt_term_geometry_session_activate_remote(kt_term_geometry_session*g,
                                              kt_term_geometry_source source){
 kt_term_geometry *geom;
 if(!g||!g->resize.session||!g->resize.session->geometry)return -1;
 if(source!=KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS&&
    source!=KT_TERM_GEOMETRY_SOURCE_SSH_PTY)return -1;
 geom=g->resize.session->geometry;
 if(!geom->remote_valid||!geom->remote_source_valid||
    geom->remote_source!=(uint8_t)source)return -2;
 return kt_term_resize_set_policy(g->resize.session,g->resize.scheduler,
        KT_TERM_GEOMETRY_REMOTE,g->resize.cell_height,g->resize.fb_height,
        g->resize.result);
}
int kt_term_geometry_session_connect_telnet(kt_term_geometry_session*g,
                                            const uint8_t*data,size_t len){
 int rc=kt_term_geometry_session_telnet_naws(g,data,len);
 if(rc<0)return rc;
 return kt_term_geometry_session_activate_remote(g,KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS);
}
int kt_term_geometry_session_connect_ssh(kt_term_geometry_session*g,
                                         const uint8_t*data,size_t len){
 int rc=kt_term_geometry_session_ssh_pty(g,data,len);
 if(rc<0)return rc;
 return kt_term_geometry_session_activate_remote(g,KT_TERM_GEOMETRY_SOURCE_SSH_PTY);
}
