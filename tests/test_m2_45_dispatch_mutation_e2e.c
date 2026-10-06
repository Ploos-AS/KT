#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
typedef struct ctx{kt_term_geometry_notifier_set*s;size_t self_id;kt_term_geometry_notifier add;int count;int action;}ctx;
static void mut(void*v,const kt_term_geometry_snapshot*o,const kt_term_geometry_snapshot*n,uint32_t f){
 ctx*c=(ctx*)v;(void)o;(void)n;(void)f;c->count++;
 if(c->action==1)assert(kt_term_geometry_notifier_set_remove(c->s,c->self_id)==0);
 else if(c->action==2)assert(kt_term_geometry_notifier_set_remove(c->s,c->self_id+1)==0);
 else if(c->action==3)assert(kt_term_geometry_notifier_set_add(c->s,&c->add,0)==0);
}
static void hit(void*v,const kt_term_geometry_snapshot*o,const kt_term_geometry_snapshot*n,uint32_t f){
 ctx*c=(ctx*)v;(void)o;(void)n;(void)f;c->count++;
}
static void setup(kt_term_geometry_session*gs,kt_term_geometry*g,kt_term_screen*screen,
 kt_term_session*session,kt_term_cell*cells,kt_term_present_scheduler*sched,kt_term_resize_result*r){
 memset(screen,0,sizeof(*screen));assert(kt_term_geometry_init(g,80,25)==0);
 assert(kt_term_session_init(session,g,screen,cells,132*43)==0);kt_term_present_scheduler_reset(sched);
 assert(kt_term_geometry_session_init(gs,session,sched,16,688,r)==0);
}
int main(void){
 kt_term_geometry g;kt_term_screen screen;kt_term_session session;kt_term_cell cells[132*43];
 kt_term_present_scheduler sched;kt_term_resize_result r;kt_term_geometry_session gs;
 kt_term_geometry_notifier slots[3],a,b,x;kt_term_geometry_notifier_set set;ctx ca,cb,cx;
 const uint8_t tel[4]={0,100,0,40};

 /* Self-remove stops current fan-out; removed callback stays gone on activation commit. */
 memset(slots,0,sizeof(slots));memset(&ca,0,sizeof(ca));memset(&cb,0,sizeof(cb));setup(&gs,&g,&screen,&session,cells,&sched,&r);
 kt_term_geometry_notifier_set_init(&set,slots,3);ca.s=&set;ca.action=1;
 kt_term_geometry_notifier_init(&a,mut,&ca);kt_term_geometry_notifier_init(&b,hit,&cb);
 assert(kt_term_geometry_notifier_set_add(&set,&a,&ca.self_id)==0);assert(kt_term_geometry_notifier_set_add(&set,&b,0)==0);
 kt_term_geometry_session_bind_notifier_set(&gs,&set);assert(kt_term_geometry_session_connect_telnet(&gs,tel,4)==0);
 assert(ca.count==1);assert(cb.count==1); /* b skipped first commit, receives activation */

 /* Remove-next prevents next observer in first commit and thereafter. */
 memset(slots,0,sizeof(slots));memset(&ca,0,sizeof(ca));memset(&cb,0,sizeof(cb));setup(&gs,&g,&screen,&session,cells,&sched,&r);
 kt_term_geometry_notifier_set_init(&set,slots,3);ca.s=&set;ca.action=2;
 kt_term_geometry_notifier_init(&a,mut,&ca);kt_term_geometry_notifier_init(&b,hit,&cb);
 assert(kt_term_geometry_notifier_set_add(&set,&a,&ca.self_id)==0);assert(kt_term_geometry_notifier_set_add(&set,&b,0)==0);
 kt_term_geometry_session_bind_notifier_set(&gs,&set);assert(kt_term_geometry_session_connect_telnet(&gs,tel,4)==0);
 assert(ca.count==2);assert(cb.count==0);

 /* Add during first commit: new observer must not receive that commit, but receives activation. */
 memset(slots,0,sizeof(slots));memset(&ca,0,sizeof(ca));memset(&cx,0,sizeof(cx));setup(&gs,&g,&screen,&session,cells,&sched,&r);
 kt_term_geometry_notifier_set_init(&set,slots,3);ca.s=&set;ca.action=3;
 kt_term_geometry_notifier_init(&x,hit,&cx);ca.add=x;kt_term_geometry_notifier_init(&a,mut,&ca);
 assert(kt_term_geometry_notifier_set_add(&set,&a,&ca.self_id)==0);
 kt_term_geometry_session_bind_notifier_set(&gs,&set);assert(kt_term_geometry_session_connect_telnet(&gs,tel,4)==0);
 assert(ca.count==2);assert(cx.count==1);
 return 0;
}
