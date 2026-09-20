#ifndef KT_TERMINAL_PALETTE_H
#define KT_TERMINAL_PALETTE_H
#include <stdint.h>
#include <stddef.h>
#include "kt/terminal.h"

typedef struct kt_term_rgb { uint8_t r,g,b; } kt_term_rgb;

#define KT_TERM_PALETTE_ANSI16 0u
#define KT_TERM_PALETTE_C64    1u

int kt_term_palette_lookup(uint8_t palette,uint8_t index,kt_term_rgb *out);
uint8_t kt_term_palette_size(uint8_t palette);
int kt_term_palette_for_profile(uint8_t profile,uint8_t *palette);
int kt_term_palette_convert_rgb24(uint8_t palette,const uint8_t *src,
                                  uint16_t width,uint16_t height,size_t src_stride,
                                  uint8_t *dst,size_t dst_stride);

#endif
