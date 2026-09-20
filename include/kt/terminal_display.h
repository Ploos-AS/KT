#ifndef KT_TERMINAL_DISPLAY_H
#define KT_TERMINAL_DISPLAY_H
#include <stddef.h>
#include <stdint.h>

#define KT_TERM_DISPLAY_INDEXED8 0u
#define KT_TERM_DISPLAY_RGB24    1u

typedef struct kt_term_display_frame {
 const uint8_t *pixels;
 uint16_t width,height;
 size_t stride;
 uint8_t format;
 uint8_t palette;
} kt_term_display_frame;

typedef struct kt_term_display_ops {
 int (*begin)(void *,const kt_term_display_frame *);
 int (*present_rows)(void *,const kt_term_display_frame *,uint16_t,uint16_t);
 int (*end)(void *,const kt_term_display_frame *);
} kt_term_display_ops;

int kt_term_display_present(const kt_term_display_ops *,void *,const kt_term_display_frame *);
int kt_term_display_present_rows(const kt_term_display_ops *,void *,
                                 const kt_term_display_frame *,uint16_t,uint16_t);
#endif
