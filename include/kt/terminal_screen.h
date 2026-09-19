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
#endif
