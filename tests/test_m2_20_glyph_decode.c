#include "kt/terminal_screen.h"
#include <assert.h>
int main(void){
 static const uint8_t b[]={0xDAu,0xC4u,0xBFu,0xB3u,0xC0u,0xD9u,0xC3u,0xB4u,0xC2u,0xC1u,0xC5u};
 static const uint32_t u[]={0x250Cu,0x2500u,0x2510u,0x2502u,0x2514u,0x2518u,0x251Cu,0x2524u,0x252Cu,0x2534u,0x253Cu};
 unsigned i;
 for(i=0;i<sizeof b;i++)assert(kt_term_decode_codepoint(KT_TERM_PROFILE_CP437,b[i])==u[i]);
 assert(kt_term_decode_codepoint(KT_TERM_PROFILE_ANSI,'A')==0x41u);
 assert(kt_term_decode_codepoint(KT_TERM_PROFILE_PETSCII,0x41u)==0x41u);
 assert(kt_term_decode_codepoint(KT_TERM_PROFILE_PETSCII,0x5Au)==0x5Au);
 assert(kt_term_decode_codepoint(KT_TERM_PROFILE_PETSCII,0x61u)==0x41u);
 assert(kt_term_decode_codepoint(KT_TERM_PROFILE_PETSCII,0x7Au)==0x5Au);
 assert(kt_term_decode_codepoint(KT_TERM_PROFILE_PETSCII,'0')==0x30u);
 assert(kt_term_decode_petscii(0x41u,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x41u);
 assert(kt_term_decode_petscii(0x41u,KT_TERM_PETSCII_LOWER_UPPER)==0x61u);
 assert(kt_term_decode_petscii(0x5Au,KT_TERM_PETSCII_LOWER_UPPER)==0x7Au);
 assert(kt_term_decode_petscii(0x61u,KT_TERM_PETSCII_LOWER_UPPER)==0x41u);
 assert(kt_term_decode_petscii(0xA0u,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x00A0u);
 assert(kt_term_decode_petscii(0xA1u,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x258Cu);
 assert(kt_term_decode_petscii(0xA2u,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x2584u);
 assert(kt_term_decode_petscii(0xA6u,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x2592u);
 assert(kt_term_decode_petscii(0xA7u,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x2595u);
 assert(kt_term_decode_petscii(0xA8u,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x1FB8Fu);
 assert(kt_term_decode_petscii(0xABu,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x251Cu);
 assert(kt_term_decode_petscii(0xB0u,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x250Cu);
 assert(kt_term_decode_petscii(0xBBu,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x2596u);
 assert(kt_term_decode_petscii(0xBFu,KT_TERM_PETSCII_UPPER_GRAPHICS)==0x259Au);
 assert(kt_term_decode_petscii(0xA1u,KT_TERM_PETSCII_LOWER_UPPER)==0xFFFDu);
 assert(kt_term_decode_codepoint(KT_TERM_PROFILE_CP437,0xFFu)==0xFFFDu);
 return 0;
}
