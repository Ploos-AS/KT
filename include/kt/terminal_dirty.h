#ifndef KT_TERMINAL_DIRTY_H
#define KT_TERMINAL_DIRTY_H
#include <stdint.h>
typedef struct kt_term_dirty_rows { uint16_t first,count; uint8_t valid; } kt_term_dirty_rows;
void kt_term_dirty_rows_reset(kt_term_dirty_rows *);
int kt_term_dirty_rows_add_cell(kt_term_dirty_rows *,uint16_t cell_y,uint8_t cell_height,uint16_t fb_height);
int kt_term_dirty_rows_add_cursor(kt_term_dirty_rows *,uint16_t old_y,uint16_t new_y,uint8_t cell_height,uint16_t fb_height);
#endif
