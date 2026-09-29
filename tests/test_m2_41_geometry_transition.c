#include <assert.h>
#include <string.h>
#include "kt/terminal_geometry_session.h"
static uint32_t diff(const kt_term_geometry_snapshot*a,
                     const kt_term_geometry_snapshot*b){
 uint32_t f=0;assert(kt_term_geometry_transition_classify(a,b,&f)==0);return f;
}
int main(void){
 kt_term_geometry_snapshot a,b;
 memset(&a,0,sizeof(a));memset(&b,0,sizeof(b));
 a.policy=b.policy=KT_TERM_GEOMETRY_FIXED;
 a.screen_cols=b.screen_cols=80;a.screen_rows=b.screen_rows=25;
 assert(diff(&a,&b)==KT_TERM_GEOMETRY_TRANSITION_NONE);

 /* FIXED -> VIEWPORT 80x30 with stored viewport update. */
 b.policy=KT_TERM_GEOMETRY_VIEWPORT;b.screen_rows=30;
 b.viewport_cols=80;b.viewport_rows=30;
 assert(diff(&a,&b)==(KT_TERM_GEOMETRY_TRANSITION_POLICY|
                      KT_TERM_GEOMETRY_TRANSITION_SCREEN|
                      KT_TERM_GEOMETRY_TRANSITION_VIEWPORT));
 a=b;

 /* Remote geometry/ownership may commit without changing active policy. */
 b.remote_cols=100;b.remote_rows=40;b.remote_valid=1;
 b.remote_source_valid=1;b.remote_source=KT_TERM_GEOMETRY_SOURCE_TELNET_NAWS;
 assert(diff(&a,&b)==(KT_TERM_GEOMETRY_TRANSITION_REMOTE|
                      KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP));
 a=b;

 /* REMOTE activation changes policy and screen only. */
 b.policy=KT_TERM_GEOMETRY_REMOTE;b.screen_cols=100;b.screen_rows=40;
 assert(diff(&a,&b)==(KT_TERM_GEOMETRY_TRANSITION_POLICY|
                      KT_TERM_GEOMETRY_TRANSITION_SCREEN));
 a=b;

 /* Owner disconnect clears remote state/ownership and falls back FIXED. */
 b.policy=KT_TERM_GEOMETRY_FIXED;b.screen_cols=80;b.screen_rows=25;
 b.remote_cols=0;b.remote_rows=0;b.remote_valid=0;b.remote_source_valid=0;
 assert(diff(&a,&b)==(KT_TERM_GEOMETRY_TRANSITION_POLICY|
                      KT_TERM_GEOMETRY_TRANSITION_SCREEN|
                      KT_TERM_GEOMETRY_TRANSITION_REMOTE|
                      KT_TERM_GEOMETRY_TRANSITION_OWNERSHIP|
                      KT_TERM_GEOMETRY_TRANSITION_DISCONNECT));

 assert(kt_term_geometry_transition_classify(0,&b,(uint32_t*)&a)==-1);
 assert(kt_term_geometry_transition_classify(&a,0,(uint32_t*)&b)==-1);
 assert(kt_term_geometry_transition_classify(&a,&b,0)==-1);
 return 0;
}
