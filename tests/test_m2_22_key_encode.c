#include "kt/terminal_key_encode.h"
#include <assert.h>
#include <string.h>
int main(void){uint8_t b[16];size_t n;
 n=kt_term_encode_key(KT_TERM_KEY_UP,0,b,sizeof b);assert(n==3u&&!memcmp(b,"\x1b[A",3));
 n=kt_term_encode_key(KT_TERM_KEY_F1,0,b,sizeof b);assert(n==3u&&!memcmp(b,"\x1bOP",3));
 n=kt_term_encode_key(KT_TERM_KEY_DELETE,0,b,sizeof b);assert(n==4u&&!memcmp(b,"\x1b[3~",4));
 n=kt_term_encode_key(KT_TERM_KEY_UP,KT_TERM_MOD_SHIFT|KT_TERM_MOD_CTRL,b,sizeof b);
 assert(n==6u&&!memcmp(b,"\x1b[1;6A",6));
 assert(kt_term_encode_key(KT_TERM_KEY_UP,0,b,2u)==0u);
 assert(kt_term_encode_key(0xdeadbeefu,0,b,sizeof b)==0u);
 return 0;}
