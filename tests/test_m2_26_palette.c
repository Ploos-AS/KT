#include "kt/terminal_palette.h"
#include <assert.h>
int main(void){
 kt_term_rgb c;uint8_t p;
 assert(kt_term_palette_size(KT_TERM_PALETTE_ANSI16)==16u);
 assert(kt_term_palette_size(KT_TERM_PALETTE_C64)==16u);
 assert(kt_term_palette_size(99u)==0u);
 assert(kt_term_palette_lookup(KT_TERM_PALETTE_ANSI16,0,&c)==0&&c.r==0&&c.g==0&&c.b==0);
 assert(kt_term_palette_lookup(KT_TERM_PALETTE_ANSI16,15,&c)==0&&c.r==255&&c.g==255&&c.b==255);
 assert(kt_term_palette_lookup(KT_TERM_PALETTE_C64,2,&c)==0&&c.r==104&&c.g==55&&c.b==43);
 assert(kt_term_palette_lookup(KT_TERM_PALETTE_C64,16,&c)==-1);
 assert(kt_term_palette_lookup(99u,0,&c)==-1);
 assert(kt_term_palette_for_profile(KT_TERM_PROFILE_ANSI,&p)==0&&p==KT_TERM_PALETTE_ANSI16);
 assert(kt_term_palette_for_profile(KT_TERM_PROFILE_CP437,&p)==0&&p==KT_TERM_PALETTE_ANSI16);
 assert(kt_term_palette_for_profile(KT_TERM_PROFILE_PETSCII,&p)==0&&p==KT_TERM_PALETTE_C64);
 assert(kt_term_palette_for_profile(99u,&p)==-1);
 return 0;
}
