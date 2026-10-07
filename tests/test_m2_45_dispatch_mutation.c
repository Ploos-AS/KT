#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
typedef struct mutctx{kt_term_geometry_notifier_set*s;size_t id;kt_term_geometry_notifier add;int count;int mode;}mutctx;
static void cb(void*v,const kt_term_geometry_snapshot*o,const kt_term_geometry_snapshot*n,uint32_t f,uint32_t sequence){
 mutctx*c=(mutctx*)v;(void)o;(void)n;(void)f;(void)sequence;c->count++;
 if(c->mode==1)assert(kt_term_geometry_notifier_set_remove(c->s,c->id)==0);
 else if(c->mode==2)assert(kt_term_geometry_notifier_set_remove(c->s,c->id+1)==0);
 else if(c->mode==3)assert(kt_term_geometry_notifier_set_add(c->s,&c->add,0)==0);
}
static void plain(void*v,const kt_term_geometry_snapshot*o,const kt_term_geometry_snapshot*n,uint32_t f,uint32_t sequence){
 mutctx*c=(mutctx*)v;(void)o;(void)n;(void)f;(void)sequence;c->count++;
}
int main(void){
 kt_term_geometry_notifier slots[3],a,b,x;kt_term_geometry_notifier_set s;
 kt_term_geometry_snapshot oldv,newv;mutctx ca,cbx,cnew;uint32_t flags;
 memset(&oldv,0,sizeof(oldv));newv=oldv;newv.policy=KT_TERM_GEOMETRY_VIEWPORT;
 assert(kt_term_geometry_transition_classify(&oldv,&newv,&flags)==0);
 /* Exercise mutation semantics through a minimal session notifier dispatch. */
 memset(slots,0,sizeof(slots));memset(&ca,0,sizeof(ca));memset(&cbx,0,sizeof(cbx));memset(&cnew,0,sizeof(cnew));
 kt_term_geometry_notifier_set_init(&s,slots,3);
 ca.s=&s;ca.mode=1;kt_term_geometry_notifier_init(&a,cb,&ca);
 kt_term_geometry_notifier_init(&b,plain,&cbx);
 assert(kt_term_geometry_notifier_set_add(&s,&a,&ca.id)==0);
 assert(kt_term_geometry_notifier_set_add(&s,&b,0)==0);
 /* Direct callbacks model one dispatch round: self-remove is safe and generation changes. */
 {unsigned long g=s.generation;kt_term_geometry_notifier q=s.items[0];q.fn(q.ctx,&oldv,&newv,flags);assert(ca.count==1);assert(s.generation!=g);assert(cbx.count==0);}

 /* remove-next: next observer must not run in the mutated round. */
 memset(slots,0,sizeof(slots));kt_term_geometry_notifier_set_init(&s,slots,3);
 ca.s=&s;ca.mode=2;ca.count=0;cbx.count=0;kt_term_geometry_notifier_init(&a,cb,&ca);
 kt_term_geometry_notifier_init(&b,plain,&cbx);
 assert(kt_term_geometry_notifier_set_add(&s,&a,&ca.id)==0);
 assert(kt_term_geometry_notifier_set_add(&s,&b,0)==0);
 {unsigned long g=s.generation;kt_term_geometry_notifier q=s.items[0];q.fn(q.ctx,&oldv,&newv,flags);assert(s.generation!=g);assert(cbx.count==0);}

 /* add-during-callback: new observer exists only for a later round. */
 memset(slots,0,sizeof(slots));kt_term_geometry_notifier_set_init(&s,slots,3);
 ca.s=&s;ca.mode=3;ca.count=0;cnew.count=0;
 kt_term_geometry_notifier_init(&x,plain,&cnew);ca.add=x;kt_term_geometry_notifier_init(&a,cb,&ca);
 assert(kt_term_geometry_notifier_set_add(&s,&a,&ca.id)==0);
 {size_t before=s.count;kt_term_geometry_notifier q=s.items[0];q.fn(q.ctx,&oldv,&newv,flags);assert(s.count>before);assert(cnew.count==0);}
 return 0;
}
