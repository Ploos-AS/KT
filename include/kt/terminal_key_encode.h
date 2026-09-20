#ifndef KT_TERMINAL_KEY_ENCODE_H
#define KT_TERMINAL_KEY_ENCODE_H
#include <stddef.h>
#include <stdint.h>
#include "kt/terminal_event.h"

/* Encode a normalized key as an ANSI/VT byte sequence.
   Returns encoded length, or 0 when unsupported/buffer too small. */
size_t kt_term_encode_key(uint32_t key,uint8_t mods,uint8_t *out,size_t cap);
#endif
