#include "kt/terminal.h"

enum { ST_GROUND=0, ST_ESC=1, ST_CSI=2 };
static void defaults(kt_term_attr *a){ a->fg=7u; a->bg=0u; a->flags=0u; }
static uint16_t param(const kt_term *t,uint8_t n,uint16_t d){ return n<t->csi_count && t->csi_params[n] ? t->csi_params[n] : d; }
static void csi_push(kt_term *t){ if(t->csi_count<KT_TERM_CSI_MAX_PARAMS)t->csi_params[t->csi_count++]=t->csi_have_value?t->csi_value:0u; t->csi_value=0u;t->csi_have_value=0u; }
static void sgr(kt_term *t){
 uint8_t i; if(!t->csi_count){defaults(&t->attr);return;}
 for(i=0;i<t->csi_count;i++){uint16_t p=t->csi_params[i];
  if(p==0)defaults(&t->attr); else if(p==1)t->attr.flags|=KT_TERM_ATTR_BOLD;
  else if(p==4)t->attr.flags|=KT_TERM_ATTR_UNDERLINE; else if(p==5)t->attr.flags|=KT_TERM_ATTR_BLINK;
  else if(p==7)t->attr.flags|=KT_TERM_ATTR_INVERSE; else if(p>=30&&p<=37)t->attr.fg=(uint8_t)(p-30);
  else if(p>=40&&p<=47)t->attr.bg=(uint8_t)(p-40);
 }
}
static void dispatch(kt_term *t,uint8_t ch){
 int16_t n=(int16_t)param(t,0,1u);
 switch(ch){
 case 'A': if(t->ops->cursor_move)t->ops->cursor_move(t->ctx,0,(int16_t)-n); break;
 case 'B': if(t->ops->cursor_move)t->ops->cursor_move(t->ctx,0,n); break;
 case 'C': if(t->ops->cursor_move)t->ops->cursor_move(t->ctx,n,0); break;
 case 'D': if(t->ops->cursor_move)t->ops->cursor_move(t->ctx,(int16_t)-n,0); break;
 case 'H': case 'f': if(t->ops->cursor_position)t->ops->cursor_position(t->ctx,param(t,0,1u),param(t,1,1u)); break;
 case 'J': if(t->ops->erase_display)t->ops->erase_display(t->ctx,(uint8_t)param(t,0,0u)); break;
 case 'm': sgr(t); break;
 }
}
void kt_term_set_profile(kt_term *t,uint8_t p){if(t)t->profile=p;}
void kt_term_reset(kt_term *t){uint8_t i;if(!t)return;defaults(&t->attr);t->parser_state=ST_GROUND;t->csi_count=0;t->csi_have_value=0;t->csi_value=0;for(i=0;i<KT_TERM_CSI_MAX_PARAMS;i++)t->csi_params[i]=0;}
void kt_term_init(kt_term *t,const kt_term_ops *o,void *x){if(!t)return;t->ops=o;t->ctx=x;t->profile=KT_TERM_PROFILE_ANSI;kt_term_reset(t);}
void kt_term_feed(kt_term *t,const uint8_t *d,size_t l){size_t i;if(!t||!d||!t->ops)return;
 for(i=0;i<l;i++){uint8_t ch=d[i];
  if(t->profile==KT_TERM_PROFILE_PETSCII){
   if(ch==0x93u){if(t->ops->erase_display)t->ops->erase_display(t->ctx,2u);continue;}
   if(ch==0x13u){if(t->ops->cursor_position)t->ops->cursor_position(t->ctx,1u,1u);continue;}
   if(ch==0x12u){t->attr.flags|=KT_TERM_ATTR_INVERSE;continue;}
   if(ch==0x92u){t->attr.flags&=(uint8_t)~KT_TERM_ATTR_INVERSE;continue;}
  }
  if(t->parser_state==ST_ESC){t->parser_state=(ch=='[')?ST_CSI:ST_GROUND;continue;}
  if(t->parser_state==ST_CSI){
   if(ch>='0'&&ch<='9'){t->csi_value=(uint16_t)(t->csi_value*10u+(ch-'0'));t->csi_have_value=1u;continue;}
   if(ch==';'){csi_push(t);continue;}
   csi_push(t);dispatch(t,ch);t->parser_state=ST_GROUND;continue;
  }
  if(ch==0x1bu){t->parser_state=ST_ESC;continue;}
  switch(ch){case '\r':if(t->ops->carriage_return)t->ops->carriage_return(t->ctx);break;case '\n':if(t->ops->line_feed)t->ops->line_feed(t->ctx);break;case '\b':if(t->ops->backspace)t->ops->backspace(t->ctx);break;case '\a':if(t->ops->bell)t->ops->bell(t->ctx);break;default:if(ch>=0x20u&&t->ops->put_cell)t->ops->put_cell(t->ctx,ch,&t->attr);break;}
 }
}
