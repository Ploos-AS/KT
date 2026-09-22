#ifndef KT_TERMINAL_GEOMETRY_EVENT_H
#define KT_TERMINAL_GEOMETRY_EVENT_H
#include <stdint.h>
#include "kt/terminal_geometry.h"

typedef struct kt_term_geometry_event {
 kt_term_geometry_source source;
 uint16_t cols,rows;
} kt_term_geometry_event;

typedef struct kt_term_geometry_event_state {
 uint16_t cols[3],rows[3];
 uint8_t valid[3];
} kt_term_geometry_event_state;

typedef struct kt_term_geometry_event_ops {
 int (*on_geometry)(void *,const kt_term_geometry_event *);
} kt_term_geometry_event_ops;

void kt_term_geometry_event_state_reset(kt_term_geometry_event_state *);
int kt_term_geometry_event_emit(kt_term_geometry_event_state *,
                                const kt_term_geometry_event_ops *,void *,
                                kt_term_geometry_source,uint16_t,uint16_t);
#endif
