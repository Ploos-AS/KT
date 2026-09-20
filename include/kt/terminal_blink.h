#ifndef KT_TERMINAL_BLINK_H
#define KT_TERMINAL_BLINK_H
#include <stdint.h>
#include "kt/terminal_screen.h"
#include "kt/terminal_dirty.h"
int kt_term_blink_dirty_rows(const kt_term_screen *,uint8_t cell_height,uint16_t fb_height,kt_term_dirty_rows *);
#endif
