#ifndef KT_TERMINAL_VIEWPORT_H
#define KT_TERMINAL_VIEWPORT_H
#include <stdint.h>
typedef enum kt_term_viewport_anchor {
 KT_TERM_VIEWPORT_TOP_LEFT=0,
 KT_TERM_VIEWPORT_CENTER=1,
 KT_TERM_VIEWPORT_BOTTOM_RIGHT=2
} kt_term_viewport_anchor;
typedef struct kt_term_viewport {
 uint16_t pixel_width,pixel_height;
 uint8_t cell_width,cell_height;
 uint16_t cols,rows;
 uint16_t used_width,used_height;
 uint16_t offset_x,offset_y;
 kt_term_viewport_anchor anchor;
} kt_term_viewport;
int kt_term_viewport_init(kt_term_viewport *,uint16_t,uint16_t,uint8_t,uint8_t);
int kt_term_viewport_resize(kt_term_viewport *,uint16_t,uint16_t);
int kt_term_viewport_set_anchor(kt_term_viewport *,kt_term_viewport_anchor);
#endif
