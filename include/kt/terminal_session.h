#ifndef KT_TERMINAL_SESSION_H
#define KT_TERMINAL_SESSION_H
#include <stddef.h>
#include <stdint.h>
#include "kt/terminal_geometry.h"
#include "kt/terminal_screen.h"

typedef struct kt_term_session {
 kt_term_geometry *geometry;
 kt_term_screen *screen;
 kt_term_cell *cells;
 size_t cell_capacity;
} kt_term_session;

int kt_term_session_init(kt_term_session *,kt_term_geometry *,kt_term_screen *,
                         kt_term_cell *,size_t);
int kt_term_session_apply_geometry(kt_term_session *);

#endif
