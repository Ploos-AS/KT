#include "kt/terminal_event.h"
int kt_term_dispatch_event(const kt_term_event*e,const kt_term_input_ops*o,void*c){
 if(!e||!o)return -1;
 switch(e->type){
 case KT_TERM_EVENT_BYTE: if(!o->on_byte)return -2;o->on_byte(c,e->byte);break;
 case KT_TERM_EVENT_PASTE: if(!o->on_paste)return -2;o->on_paste(c,e->data,e->data_len);break;
 case KT_TERM_EVENT_RESIZE: if(!o->on_resize)return -2;o->on_resize(c,e->width,e->height);break;
 case KT_TERM_EVENT_KEY: if(!o->on_key)return -2;o->on_key(c,e->key,e->mods);break;
 case KT_TERM_EVENT_MOUSE: if(!o->on_mouse)return -2;o->on_mouse(c,e->width,e->height,e->byte,e->mods);break;
 default:return -3;
 }
 return 0;
}

static void feed_byte(void*p,uint8_t b){kt_term_feed_adapter*a=p;if(a&&a->term)kt_term_feed(a->term,&b,1u);}
static void feed_paste(void*p,const uint8_t*d,uint16_t n){kt_term_feed_adapter*a=p;if(a&&a->term&&d)kt_term_feed(a->term,d,n);}
static const kt_term_input_ops feed_ops={feed_byte,feed_paste,0,0,0};
void kt_term_feed_adapter_init(kt_term_feed_adapter*a,kt_term*t){if(a)a->term=t;}
const kt_term_input_ops *kt_term_feed_adapter_ops(void){return &feed_ops;}
