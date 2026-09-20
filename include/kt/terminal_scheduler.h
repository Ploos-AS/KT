#ifndef KT_TERMINAL_SCHEDULER_H
#define KT_TERMINAL_SCHEDULER_H
#include <stdint.h>
#include "kt/terminal_dirty.h"
#include "kt/terminal_display.h"

typedef struct kt_term_present_scheduler { uint8_t pending,immediate,blink_pending; uint16_t first_row,count; } kt_term_present_scheduler;
void kt_term_present_scheduler_reset(kt_term_present_scheduler *);
int kt_term_present_scheduler_add_rows(kt_term_present_scheduler *,const kt_term_dirty_rows *);
void kt_term_present_scheduler_request_blink(kt_term_present_scheduler *);
int kt_term_present_scheduler_take(kt_term_present_scheduler *,kt_term_dirty_rows *);
int kt_term_present_scheduler_present(kt_term_present_scheduler *,const kt_term_display_ops *,void *,const kt_term_display_frame *);
#endif
