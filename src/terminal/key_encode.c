#include "kt/terminal_key_encode.h"
#include <stdio.h>
#include <string.h>
static size_t emit(const char*s,uint8_t*out,size_t cap){size_t n=strlen(s);if(!out||cap<n)return 0;memcpy(out,s,n);return n;}
static unsigned modparam(uint8_t mods){return 1u+(mods&KT_TERM_MOD_SHIFT?1u:0u)+(mods&KT_TERM_MOD_ALT?2u:0u)+(mods&KT_TERM_MOD_CTRL?4u:0u)+(mods&KT_TERM_MOD_META?8u:0u);}
size_t kt_term_encode_key(uint32_t key,uint8_t mods,uint8_t*out,size_t cap){
 char b[20];unsigned m=modparam(mods),n=0u,final=0u,tilde=0u;
 switch(key){
 case KT_TERM_KEY_UP:final='A';break;case KT_TERM_KEY_DOWN:final='B';break;case KT_TERM_KEY_RIGHT:final='C';break;case KT_TERM_KEY_LEFT:final='D';break;
 case KT_TERM_KEY_HOME:final='H';break;case KT_TERM_KEY_END:final='F';break;
 case KT_TERM_KEY_INSERT:tilde=2;break;case KT_TERM_KEY_DELETE:tilde=3;break;case KT_TERM_KEY_PAGE_UP:tilde=5;break;case KT_TERM_KEY_PAGE_DOWN:tilde=6;break;
 case KT_TERM_KEY_F1:if(!mods)return emit("\x1bOP",out,cap);final='P';break;
 case KT_TERM_KEY_F2:if(!mods)return emit("\x1bOQ",out,cap);final='Q';break;
 case KT_TERM_KEY_F3:if(!mods)return emit("\x1bOR",out,cap);final='R';break;
 case KT_TERM_KEY_F4:if(!mods)return emit("\x1bOS",out,cap);final='S';break;
 case KT_TERM_KEY_F5:tilde=15;break;case KT_TERM_KEY_F6:tilde=17;break;case KT_TERM_KEY_F7:tilde=18;break;case KT_TERM_KEY_F8:tilde=19;break;
 case KT_TERM_KEY_F9:tilde=20;break;case KT_TERM_KEY_F10:tilde=21;break;case KT_TERM_KEY_F11:tilde=23;break;case KT_TERM_KEY_F12:tilde=24;break;
 default:return 0;
 }
 if(final){if(!mods){b[0]=27;b[1]='[';b[2]=(char)final;n=3;}else n=(unsigned)snprintf(b,sizeof b,"\x1b[1;%u%c",m,(char)final);}
 else {if(!mods)n=(unsigned)snprintf(b,sizeof b,"\x1b[%u~",tilde);else n=(unsigned)snprintf(b,sizeof b,"\x1b[%u;%u~",tilde,m);}
 if(!out||n>=sizeof b||cap<n)return 0;memcpy(out,b,n);return n;
}
