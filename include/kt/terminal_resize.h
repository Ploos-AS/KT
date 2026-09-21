#ifndef KT_TERMINAL_RESIZE_H
#define KT_TERMINAL_RESIZE_H
#include <stdint.h>
#include "kt/terminal_session.h"
#include "kt/terminal_dirty.h"
#include "kt/terminal_scheduler.h"

typedef struct kt_term_resize_result {
 uint16_t old_cols,old_rows;
 uint16_t new_cols,new_rows;
 kt_term_dirty_rows dirty;
 uint8_t changed;
} kt_term_resize_result;

int kt_term_resize_apply(kt_term_session *,kt_term_present_scheduler *,
                         uint8_t,uint16_t,kt_term_resize_result *);
int kt_term_resize_set_policy(kt_term_session *,kt_term_present_scheduler *,
                              kt_term_geometry_policy,uint8_t,uint16_t,
                              kt_term_resize_result *);
int kt_term_resize_negotiate(kt_term_session *,kt_term_present_scheduler *,
                             kt_term_geometry_source,uint16_t,uint16_t,
                             uint8_t,uint16_t,kt_term_resize_result *);
int kt_term_resize_release_remote(kt_term_session *,kt_term_present_scheduler *,
                                  kt_term_geometry_source,uint8_t,uint16_t,
                                  kt_term_resize_result *);

#endif
