#include "kt/terminal_geometry_session.h"

int kt_term_geometry_session_init(kt_term_geometry_session*g,
                                  kt_term_session*s,
                                  kt_term_present_scheduler*sched,
                                  uint8_t cell_height,uint16_t fb_height,
                                  kt_term_resize_result*result){
 if(!g||!s||!s->geometry||!s->screen||!cell_height||!fb_height)return -1;
 kt_term_geometry_event_state_reset(&g->events);
 g->notifier=0;
 kt_term_geometry_resize_adapter_init(&g->resize,s,sched,cell_height,fb_height,result);
 return 0;
}
void kt_term_geometry_session_reset(kt_term_geometry_session*g){
 if(g)kt_term_geometry_event_state_reset(&g->events);
}
void kt_term_geometry_session_bind_notifier(kt_term_geometry_session*g,
                                            kt_term_geometry_notifier*n){
 if(g)g->notifier=n;
}
static int notify_after(kt_term_geometry_session*g,
                        const kt_term_geometry_snapshot*old,int rc){
 if(rc<0||!g||!g->notifier)return rc;
 (void)kt_term_geometry_session_notify_commit(g,old,g->notifier);
 return rc;
}
int kt_term_geometry_session_telnet_naws(kt_term_geometry_session*g,
                                         const uint8_t*data,size_t len){
 kt_term_geometry_snapshot old;int rc;
 if(!g||kt_term_geometry_session_snapshot(g,&old)!=0)return -1;
 rc=kt_term_geometry_from_telnet_naws(&g->events,
        kt_term_geometry_resize_adapter_ops(),&g->resize,data,len);
 return notify_after(g,&old,rc);
}
int kt_term_geometry_session_ssh_pty(kt_term_geometry_session*g,
                                     const uint8_t*data,size_t len){
 kt_term_geometry_snapshot old;int rc;
 if(!g||kt_term_geometry_session_snapshot(g,&old)!=0)return -1;
 rc=kt_term_geometry_from_ssh_pty(&g->events,
        kt_term_geometry_resize_adapter_ops(),&g->resize,data,len);
 return notify_after(g,&old,rc);
}
int kt_term_geometry_session_viewport(kt_term_geometry_session*g,
                                      const kt_term_viewport*v){
 kt_term_geometry_snapshot old;int rc;
 if(!g||kt_term_geometry_session_snapshot(g,&old)!=0)return -1;
 rc=kt_term_geometry_from_viewport(&g->events,
        kt_term_geometry_resize_adapter_ops(),&g->resize,v);
 return notify_after(g,&old,rc);
}
int kt_term_geometry_session_disconnect(kt_term_geometry_session*g,
                                        kt_term_geometry_source source){
 unsigned i=(unsigned)source;int rc;kt_term_geometry_snapshot before;
 if(!g||!g->resize.session||i>2u||source==KT_TERM_GEOMETRY_SOURCE_LOCAL)return -1;
 if(kt_term_geometry_session_snapshot(g,&before)!=0)return -1;
 rc=kt_term_resize_release_remote(g->resize.session,g->resize.scheduler,source,
                                  g->resize.cell_height,g->resize.fb_height,
                                  g->resize.result);
 if(rc==0)g->events.valid[i]=0;
 return notify_after(g,&before,rc);
}

int kt_term_geometry_session_activate_remote(kt_term_geometry_session*g,
                                              kt_term_geometry_source source){
 kt_term_geometry *geom;kt_term_geometry_snapshot before;int rc;
 if(!g||!g->resize.session||!g->resize.session->geometry)return -1;
 if(source!=KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS&&
    source!=KT_TERM_GEOMETRY_SOURCE_SSH_PTY)return -1;
 geom=g->resize.session->geometry;
 if(!geom->remote_valid||!geom->remote_source_valid||
    geom->remote_source!=(uint8_t)source)return -2;
 if(kt_term_geometry_session_snapshot(g,&before)!=0)return -1;
 rc=kt_term_resize_set_policy(g->resize.session,g->resize.scheduler,
        KT_TERM_GEOMETRY_REMOTE,g->resize.cell_height,g->resize.fb_height,
        g->resize.result);
 return notify_after(g,&before,rc);
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

int kt_term_geometry_session_activate_viewport(kt_term_geometry_session*g){
 kt_term_geometry *geom;kt_term_geometry_snapshot before;int rc;
 if(!g||!g->resize.session||!g->resize.session->geometry)return -1;
 geom=g->resize.session->geometry;
 if(!geom->viewport_cols||!geom->viewport_rows)return -2;
 if(kt_term_geometry_session_snapshot(g,&before)!=0)return -1;
 rc=kt_term_resize_set_policy(g->resize.session,g->resize.scheduler,
        KT_TERM_GEOMETRY_VIEWPORT,g->resize.cell_height,g->resize.fb_height,
        g->resize.result);
 return notify_after(g,&before,rc);
}
int kt_term_geometry_session_connect_viewport(kt_term_geometry_session*g,
                                              const kt_term_viewport*v){
 int rc=kt_term_geometry_session_viewport(g,v);
 if(rc<0)return rc;
 return kt_term_geometry_session_activate_viewport(g);
}

int kt_term_geometry_session_snapshot(const kt_term_geometry_session*g,
                                      kt_term_geometry_snapshot*out){
 const kt_term_session *s;
 const kt_term_geometry *geom;
 if(!g||!out||!g->resize.session)return -1;
 s=g->resize.session;
 if(!s->geometry||!s->screen)return -1;
 geom=s->geometry;
 out->policy=geom->policy;
 out->screen_cols=s->screen->width;
 out->screen_rows=s->screen->height;
 out->viewport_cols=geom->viewport_cols;
 out->viewport_rows=geom->viewport_rows;
 out->remote_cols=geom->remote_cols;
 out->remote_rows=geom->remote_rows;
 out->remote_valid=geom->remote_valid;
 out->remote_source_valid=geom->remote_source_valid;
 out->remote_source=geom->remote_source_valid?
  (kt_term_geometry_source)geom->remote_source:KT_TERM_GEOMETRY_SOURCE_LOCAL;
 return 0;
}

void kt_term_geometry_notifier_init(kt_term_geometry_notifier*n,
                                    kt_term_geometry_notify_fn fn,void*ctx){
 if(n){n->fn=fn;n->ctx=ctx;}
}
int kt_term_geometry_session_snapshot_changed(const kt_term_geometry_snapshot*a,
                                              const kt_term_geometry_snapshot*b){
 if(!a||!b)return -1;
 return a->policy!=b->policy||a->screen_cols!=b->screen_cols||
  a->screen_rows!=b->screen_rows||a->viewport_cols!=b->viewport_cols||
  a->viewport_rows!=b->viewport_rows||a->remote_cols!=b->remote_cols||
  a->remote_rows!=b->remote_rows||a->remote_valid!=b->remote_valid||
  a->remote_source_valid!=b->remote_source_valid||
  (a->remote_source_valid&&b->remote_source_valid&&
   a->remote_source!=b->remote_source);
}
int kt_term_geometry_session_notify_commit(const kt_term_geometry_session*g,
                                           const kt_term_geometry_snapshot*old,
                                           const kt_term_geometry_notifier*n){
 kt_term_geometry_snapshot now;
 int changed;
 if(!g||!old||!n)return -1;
 if(kt_term_geometry_session_snapshot(g,&now)!=0)return -1;
 changed=kt_term_geometry_session_snapshot_changed(old,&now);
 if(changed<0)return changed;
 if(changed&&n->fn)n->fn(n->ctx,old,&now);
 return changed;
}

int kt_term_geometry_transition_classify(const kt_term_geometry_snapshot*a,
                                         const kt_term_geometry_snapshot*b,
                                         uint32_t*flags){
 uint32_t f=0;
 if(!a||!b||!flags)return -1;
 if(a->policy!=b->policy)f|=KT_TERM_GEOMETRY_TRANSITION_POLICY;
 if(a->screen_cols!=b->screen_cols||a->screen_rows!=b->screen_rows)
  f|=KT_TERM_GEOMETRY_TRANSITION_SCREEN;
 if(a->viewport_cols!=b->viewport_cols||a->viewport_rows!=b->viewport_rows)
  f|=KT_TERM_GEOMETRY_TRANSITION_VIEWPORT;
 if(a->remote_cols!=b->remote_cols||a->remote_rows!=b->remote_rows||
    a->remote_valid!=b->remote_valid)
  f|=KT_TERM_GEOMETRY_TRANSITION_REMOTE;
 if(a->remote_source_valid!=b->remote_source_valid||
    (a->remote_source_valid&&b->remote_source_valid&&
     a->remote_source!=b->remote_source))
  f|=KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP;
 if(a->remote_source_valid&&!b->remote_source_valid)
  f|=KT_TERM_GEOMETRY_TRANSITION_DISCONNECT;
 *flags=f;
 return 0;
}
