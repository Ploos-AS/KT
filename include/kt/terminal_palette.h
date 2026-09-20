#ifndef KT_TERMINAL_PALETTE_H
#define KT_TERMINAL_PALETTE_H
#include <stdint.h>

typedef struct kt_term_rgb { uint8_t r,g,b; } kt_term_rgb;

#define KT_TERM_PALETTE_ANSI16 0u
#define KT_TERM_PALETTE_C64    1u

int kt_term_palette_lookup(uint8_t palette,uint8_t index,kt_term_rgb *out);
uint8_t kt_term_palette_size(uint8_t palette);

#endif
