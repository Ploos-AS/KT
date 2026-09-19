#ifndef KT_TERMINAL_H
#define KT_TERMINAL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KT_TERM_ABI_VERSION 1u

typedef struct kt_term_attr {
    uint8_t fg;
    uint8_t bg;
    uint8_t flags;
} kt_term_attr;

enum {
    KT_TERM_ATTR_BOLD      = 1u << 0,
    KT_TERM_ATTR_UNDERLINE = 1u << 1,
    KT_TERM_ATTR_BLINK     = 1u << 2,
    KT_TERM_ATTR_INVERSE   = 1u << 3
};

typedef struct kt_term_ops {
    void (*put_cell)(void *ctx, uint8_t ch, const kt_term_attr *attr);
    void (*carriage_return)(void *ctx);
    void (*line_feed)(void *ctx);
    void (*backspace)(void *ctx);
    void (*bell)(void *ctx);
} kt_term_ops;

typedef struct kt_term {
    const kt_term_ops *ops;
    void *ctx;
    kt_term_attr attr;
    uint8_t parser_state;
} kt_term;

void kt_term_init(kt_term *term, const kt_term_ops *ops, void *ctx);
void kt_term_reset(kt_term *term);
void kt_term_feed(kt_term *term, const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif
