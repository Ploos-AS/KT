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
 {uint8_t src[6]={0,15,0xaa,2,1,0xbb};uint8_t mem[16],*dst=&mem[1];unsigned i;for(i=0;i<16;i++)mem[i]=0xcc;
  assert(kt_term_palette_convert_rgb24(KT_TERM_PALETTE_ANSI16,src,2,2,3,dst,7)==0);
  assert(dst[0]==0&&dst[1]==0&&dst[2]==0&&dst[3]==255&&dst[4]==255&&dst[5]==255);
  assert(dst[7]==0&&dst[8]==170&&dst[9]==0&&dst[10]==170&&dst[11]==0&&dst[12]==0);
  assert(mem[0]==0xcc&&mem[15]==0xcc&&dst[6]==0xcc&&dst[13]==0xcc);
  src[0]=16;assert(kt_term_palette_convert_rgb24(KT_TERM_PALETTE_ANSI16,src,2,2,3,dst,7)==-2);
  assert(kt_term_palette_convert_rgb24(KT_TERM_PALETTE_ANSI16,src,2,2,1,dst,7)==-1);
  assert(kt_term_palette_convert_rgb24(KT_TERM_PALETTE_ANSI16,src,2,2,3,dst,5)==-1);
 }
 return 0;
}
