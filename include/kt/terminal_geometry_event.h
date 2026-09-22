#ifndef KT_TERMINAL_GEOMETRY_EVENT_H
#define KT_TERMINAL_GEOMETRY_EVENT_H
#include <stdint.h>
#include "kt/terminal_geometry.h"
#include "kt/terminal_resize.h"

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
typedef struct kt_term_geometry_resize_adapter {
 kt_term_session *session;
 kt_term_present_scheduler *scheduler;
 uint8_t cell_height;
 uint16_t fb_height;
 kt_term_resize_result *result;
} kt_term_geometry_resize_adapter;

void kt_term_geometry_resize_adapter_init(kt_term_geometry_resize_adapter *,
                                          kt_term_session *,
                                          kt_term_present_scheduler *,
                                          uint8_t,uint16_t,
                                          kt_term_resize_result *);
const kt_term_geometry_event_ops *kt_term_geometry_resize_adapter_ops(void);

int kt_term_geometry_event_emit(kt_term_geometry_event_state *,
                                const kt_term_geometry_event_ops *,void *,
                                kt_term_geometry_source,uint16_t,uint16_t);
#endif
