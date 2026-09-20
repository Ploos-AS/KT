#ifndef KT_TERMINAL_SCHEDULER_H
#define KT_TERMINAL_SCHEDULER_H
#include <stdint.h>
#include "kt/terminal_dirty.h"

typedef struct kt_term_present_scheduler {
 uint8_t pending;
 uint8_t immediate;
 uint8_t blink_pending;
 uint16_t first_row,count;
} kt_term_present_scheduler;

void kt_term_present_scheduler_reset(kt_term_present_scheduler *);
int kt_term_present_scheduler_add_rows(kt_term_present_scheduler *,const kt_term_dirty_rows *);
void kt_term_present_scheduler_request_blink(kt_term_present_scheduler *);
int kt_term_present_scheduler_take(kt_term_present_scheduler *,kt_term_dirty_rows *);
#endif
