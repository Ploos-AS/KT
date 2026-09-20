#include "kt/terminal_platform.h"
int kt_term_platform_init(kt_term_platform_adapter*a,kt_term_platform p,
 kt_term_frontend*f,const kt_term_platform_ops*o,void*c){
 if(!a||!f||!o)return -1;a->platform=p;a->frontend=f;a->ops=o;a->ctx=c;return 0;
}
int kt_term_platform_key_event(kt_term_platform_adapter*a,uint32_t raw,uint32_t rawmods){
 kt_term_platform_key k;if(!a||!a->frontend||!a->ops||!a->ops->normalize_key)return -1;
 if(a->ops->normalize_key(a->ctx,raw,rawmods,&k)!=0)return -2;
 return kt_term_frontend_key(a->frontend,k.code,k.mods);
}
int kt_term_platform_present(kt_term_platform_adapter*a){
 int r;if(!a||!a->frontend)return -1;r=kt_term_frontend_render(a->frontend);if(r)return r;
 if(a->ops&&a->ops->present)return a->ops->present(a->ctx);return 0;
}
