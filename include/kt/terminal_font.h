#ifndef KT_TERMINAL_FONT_H
#define KT_TERMINAL_FONT_H
#include <stddef.h>
#include <stdint.h>
#include "kt/terminal_screen.h"

typedef struct kt_term_bitmap_glyph {
 uint8_t width;
 uint8_t height;
 uint8_t stride;
 const uint8_t *bitmap;
 size_t bitmap_size;
} kt_term_bitmap_glyph;

typedef struct kt_term_font_ops {
 int (*lookup_legacy)(void *,uint8_t profile,uint8_t charset,uint8_t source_byte,
                      kt_term_bitmap_glyph *);
 int (*lookup_codepoint)(void *,uint32_t codepoint,kt_term_bitmap_glyph *);
} kt_term_font_ops;

/* Prefer exact legacy-byte identity for pixel-faithful CP437/PETSCII.
   Fall back to Unicode codepoint lookup when no exact glyph is available. */
int kt_term_bitmap_glyph_valid(const kt_term_bitmap_glyph *);
/* Resolution order: exact legacy identity, Unicode fallback, U+FFFD replacement. */
int kt_term_font_resolve(const kt_term_font_ops *,void *,
                         const kt_term_glyph *,kt_term_bitmap_glyph *);
#endif
