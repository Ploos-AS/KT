#ifndef KT_TERMINAL_SCREEN_H
#define KT_TERMINAL_SCREEN_H
#include "kt/terminal.h"
#include <stddef.h>
#include <stdint.h>
typedef struct kt_term_cell { uint8_t ch; kt_term_attr attr; } kt_term_cell;
typedef struct kt_term_screen {
    uint16_t width,height,x,y;
    kt_term_cell *cells;
    size_t cell_count;
} kt_term_screen;
int kt_term_screen_init(kt_term_screen *,uint16_t,uint16_t,kt_term_cell *,size_t);
void kt_term_screen_clear(kt_term_screen *);
const kt_term_ops *kt_term_screen_ops(void);

/* Renderer-facing Unicode scalar for a stored byte/profile pair.
   CP437 currently guarantees the M2.17 box-drawing corpus subset. */
uint32_t kt_term_decode_codepoint(uint8_t profile,uint8_t ch);

#define KT_TERM_PETSCII_UPPER_GRAPHICS 0u
#define KT_TERM_PETSCII_LOWER_UPPER    1u
uint32_t kt_term_decode_petscii(uint8_t ch,uint8_t charset);

typedef struct kt_term_glyph {
 uint32_t codepoint;
 uint8_t source_byte;
 uint8_t profile;
 uint8_t charset;
} kt_term_glyph;
kt_term_glyph kt_term_resolve_glyph(uint8_t profile,uint8_t ch,uint8_t charset);
#endif
