#ifndef KT_TERMINAL_GEOMETRY_H
#define KT_TERMINAL_GEOMETRY_H
#include <stdint.h>
#include "kt/terminal_viewport.h"

typedef enum kt_term_geometry_policy {
 KT_TERM_GEOMETRY_FIXED=0,
 KT_TERM_GEOMETRY_VIEWPORT=1,
 KT_TERM_GEOMETRY_REMOTE=2
} kt_term_geometry_policy;

typedef struct kt_term_geometry {
 uint16_t cols,rows;
 uint16_t fixed_cols,fixed_rows;
 uint16_t viewport_cols,viewport_rows;
 uint16_t remote_cols,remote_rows;
 uint8_t remote_valid;
 kt_term_geometry_policy policy;
} kt_term_geometry;

int kt_term_geometry_init(kt_term_geometry *,uint16_t,uint16_t);
int kt_term_geometry_set_policy(kt_term_geometry *,kt_term_geometry_policy);
int kt_term_geometry_set_fixed(kt_term_geometry *,uint16_t,uint16_t);
int kt_term_geometry_set_viewport(kt_term_geometry *,uint16_t,uint16_t);
int kt_term_geometry_from_viewport(kt_term_geometry *,const kt_term_viewport *);
int kt_term_geometry_set_remote(kt_term_geometry *,uint16_t,uint16_t);
void kt_term_geometry_clear_remote(kt_term_geometry *);

#endif
