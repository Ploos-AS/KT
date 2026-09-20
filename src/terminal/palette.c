#include "kt/terminal_palette.h"
static const kt_term_rgb ansi16[16]={
 {0,0,0},{170,0,0},{0,170,0},{170,85,0},{0,0,170},{170,0,170},{0,170,170},{170,170,170},
 {85,85,85},{255,85,85},{85,255,85},{255,255,85},{85,85,255},{255,85,255},{85,255,255},{255,255,255}
};
/* Pepto-derived C64 palette, stored as a deterministic reference mapping. */
static const kt_term_rgb c64[16]={
 {0,0,0},{255,255,255},{104,55,43},{112,164,178},{111,61,134},{88,141,67},{53,40,121},{184,199,111},
 {111,79,37},{67,57,0},{154,103,89},{68,68,68},{108,108,108},{154,210,132},{108,94,181},{149,149,149}
};
uint8_t kt_term_palette_size(uint8_t p){return (p==KT_TERM_PALETTE_ANSI16||p==KT_TERM_PALETTE_C64)?16u:0u;}
int kt_term_palette_lookup(uint8_t p,uint8_t i,kt_term_rgb*out){const kt_term_rgb*t;if(!out||i>=16u)return -1;if(p==KT_TERM_PALETTE_ANSI16)t=ansi16;else if(p==KT_TERM_PALETTE_C64)t=c64;else return -1;*out=t[i];return 0;}

int kt_term_palette_for_profile(uint8_t profile,uint8_t*out){
 if(!out)return -1;
 if(profile==KT_TERM_PROFILE_PETSCII)*out=KT_TERM_PALETTE_C64;
 else if(profile==KT_TERM_PROFILE_ANSI||profile==KT_TERM_PROFILE_CP437)*out=KT_TERM_PALETTE_ANSI16;
 else return -1;
 return 0;
}

int kt_term_palette_convert_rgb24(uint8_t p,const uint8_t*src,uint16_t w,uint16_t h,size_t ss,uint8_t*dst,size_t ds){
 uint16_t x,y;if(!src||!dst||!w||!h||ss<w||ds<(size_t)w*3u||!kt_term_palette_size(p))return -1;
 for(y=0;y<h;y++)for(x=0;x<w;x++){kt_term_rgb c;uint8_t i=src[(size_t)y*ss+x];if(kt_term_palette_lookup(p,i,&c)!=0)return -2;dst[(size_t)y*ds+(size_t)x*3u]=c.r;dst[(size_t)y*ds+(size_t)x*3u+1u]=c.g;dst[(size_t)y*ds+(size_t)x*3u+2u]=c.b;}
 return 0;
}
