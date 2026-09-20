#include "kt/terminal_key_encode.h"
#include <string.h>
static size_t emit(const char*s,uint8_t*out,size_t cap){size_t n=strlen(s);if(!out||cap<n)return 0;memcpy(out,s,n);return n;}
size_t kt_term_encode_key(uint32_t key,uint8_t mods,uint8_t*out,size_t cap){
 unsigned m=1u+(mods&KT_TERM_MOD_SHIFT?1u:0u)+(mods&KT_TERM_MOD_ALT?2u:0u)+(mods&KT_TERM_MOD_CTRL?4u:0u)+(mods&KT_TERM_MOD_META?8u:0u);
 char b[16];const char*s=0;unsigned code=0;
 if(!mods)switch(key){
 case KT_TERM_KEY_UP:s="\x1b[A";break;case KT_TERM_KEY_DOWN:s="\x1b[B";break;
 case KT_TERM_KEY_RIGHT:s="\x1b[C";break;case KT_TERM_KEY_LEFT:s="\x1b[D";break;
 case KT_TERM_KEY_HOME:s="\x1b[H";break;case KT_TERM_KEY_END:s="\x1b[F";break;
 case KT_TERM_KEY_INSERT:s="\x1b[2~";break;case KT_TERM_KEY_DELETE:s="\x1b[3~";break;
 case KT_TERM_KEY_PAGE_UP:s="\x1b[5~";break;case KT_TERM_KEY_PAGE_DOWN:s="\x1b[6~";break;
 case KT_TERM_KEY_F1:s="\x1bOP";break;case KT_TERM_KEY_F2:s="\x1bOQ";break;
 case KT_TERM_KEY_F3:s="\x1bOR";break;case KT_TERM_KEY_F4:s="\x1bOS";break;
 default:break;} if(s)return emit(s,out,cap);
 switch(key){
 case KT_TERM_KEY_UP:code='A';break;case KT_TERM_KEY_DOWN:code='B';break;case KT_TERM_KEY_RIGHT:code='C';break;case KT_TERM_KEY_LEFT:code='D';break;
 case KT_TERM_KEY_HOME:code='H';break;case KT_TERM_KEY_END:code='F';break;default:return 0;}
 if(!out||cap<6u)return 0;b[0]=27;b[1]='[';b[2]='1';b[3]=';';b[4]=(char)('0'+m);b[5]=(char)code;memcpy(out,b,6u);return 6u;
}
