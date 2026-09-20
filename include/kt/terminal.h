#ifndef KT_TERMINAL_H
#define KT_TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KT_TERM_ABI_VERSION 1u
#define KT_TERM_CSI_MAX_PARAMS 8u

#define KT_TERM_PROFILE_ANSI 0u
#define KT_TERM_PROFILE_CP437 1u
#define KT_TERM_PROFILE_PETSCII 2u

typedef struct kt_term_attr { uint8_t fg, bg, flags; } kt_term_attr;
enum {
    KT_TERM_ATTR_BOLD=1u<<0, KT_TERM_ATTR_UNDERLINE=1u<<1,
    KT_TERM_ATTR_BLINK=1u<<2, KT_TERM_ATTR_INVERSE=1u<<3
};
typedef struct kt_term_ops {
    void (*put_cell)(void *, uint8_t, const kt_term_attr *);
    void (*carriage_return)(void *); void (*line_feed)(void *);
    void (*backspace)(void *); void (*bell)(void *);
    void (*cursor_move)(void *, int16_t, int16_t);
    void (*cursor_position)(void *, uint16_t, uint16_t);
    void (*erase_display)(void *, uint8_t);
    void (*set_charset)(void *, uint8_t);
} kt_term_ops;
typedef struct kt_term {
    const kt_term_ops *ops; void *ctx; kt_term_attr attr;
    uint8_t parser_state, csi_count, csi_have_value, profile, charset;
    uint16_t csi_params[KT_TERM_CSI_MAX_PARAMS], csi_value;
} kt_term;
void kt_term_init(kt_term *, const kt_term_ops *, void *);
void kt_term_reset(kt_term *);
void kt_term_set_profile(kt_term *, uint8_t);
void kt_term_feed(kt_term *, const uint8_t *, size_t);
#ifdef __cplusplus
}
#endif
#endif
