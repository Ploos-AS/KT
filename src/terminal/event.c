#include "kt/terminal_event.h"
#include "kt/terminal_key_encode.h"
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

int kt_term_key_output_flush(kt_term_key_output_adapter*a){
 int n;if(!a||!a->output||!a->output->write)return -1;
 while(a->pending_off<a->pending_len){
  n=a->output->write(a->output_ctx,a->pending+a->pending_off,a->pending_len-a->pending_off);
  if(n<0){a->last_error=n;return n;} if(n==0)return 1;
  if((size_t)n>a->pending_len-a->pending_off){a->last_error=-2;return -2;}
  a->pending_off+=(size_t)n;
 }
 a->pending_len=0u;a->pending_off=0u;a->last_error=0;return 0;
}
static void output_key(void*p,uint32_t key,uint8_t mods){
 kt_term_key_output_adapter*a=p;size_t n;if(!a||a->pending_len)return;
 n=kt_term_encode_key(key,mods,a->pending,sizeof a->pending);
 if(!n)return;a->pending_len=n;a->pending_off=0u;(void)kt_term_key_output_flush(a);
}
static const kt_term_input_ops key_output_ops={0,0,0,output_key,0};
void kt_term_key_output_adapter_init(kt_term_key_output_adapter*a,const kt_term_output_ops*o,void*c){
 if(a){a->output=o;a->output_ctx=c;a->pending_len=0u;a->pending_off=0u;a->last_error=0;}
}
const kt_term_input_ops *kt_term_key_output_adapter_ops(void){return &key_output_ops;}
