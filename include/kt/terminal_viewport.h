#ifndef KT_TERMINAL_VIEWPORT_H
#define KT_TERMINAL_VIEWPORT_H
#include <stdint.h>
typedef struct kt_term_viewport {
 uint16_t pixel_width,pixel_height;
 uint8_t cell_width,cell_height;
 uint16_t cols,rows;
 uint16_t used_width,used_height;
} kt_term_viewport;
int kt_term_viewport_init(kt_term_viewport *,uint16_t,uint16_t,uint8_t,uint8_t);
int kt_term_viewport_resize(kt_term_viewport *,uint16_t,uint16_t);
#endif
